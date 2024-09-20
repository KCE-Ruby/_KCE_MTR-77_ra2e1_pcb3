/**
 ******************************************************************************
 * @file    Logic_Manager.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/Display.h"
#include "INC/framework/ADC.h"
#include "INC/framework/Key.h"
#include "INC/framework/datapool.h"
#include "eeprom/i2c_ee.h"
#include "INC/Logic_Manager.h"

/* Private defines ----------------------------------------------------------*/
#define BOOTonTIME                         (4500)      //4.5 = 1ms*4500 = 4500次
#define BOOToffTIME                        (5000)      //+0.5s = 1ms*500 = 500次
#define ERROR_AD                           (-999)
#define KEYLEAVE_SETTIME                      (3)      //按鍵離開設定層的閃爍時間, 單位:秒
#define AUTOLEAVE_CHECKSETTIME               (12)      //檢查設定值多久後自動跳出, 單位:秒
#define AUTOLEAVE_CHANGESETTIME              (15)      //修改設定值多久後自動跳出, 單位:秒

/* extern variables -----------------------------------------------------------------*/
extern r_tmr tmr;
extern volatile uint8_t data;
extern __IO s_Var System;
extern __IO s_Flag sFlag;

extern __IO bool CLOSE_LED_FLAG;
extern __IO Key_Manager KeyUp, KeyDown, KeyStandby, KeyBulb, KeyDefrost, KeySet;
// extern __IO ByteSettingTable bytetable[End];
extern __IO uint8_t bytetable_pr1[Pr1_size];

/* variables -----------------------------------------------------------------*/
__IO uint8_t I2c_Buf_Read[eep_end] = {};
static bool bootled_En=true;

/* Private Logic API funcitons protocol ------------------------------------*/
static void update_history_value(void);
static void update_icon(void);
static void check_set_value(void);
static void change_set_value(void);
static void leave_settingMode(void);
static void update_display_message(void);
static void reset_eeprom(void);

/* Private function protocol -----------------------------------------------*/
static void read_all_eeprom_data(void);
static void boot_control(void);
static void loop_100ms(void);
static void loop_100us(void);

/* static Logic API funcitons ------------------------------------------------------*/
static void update_history_value(void)
{
  static bool clear_limit_flag=true;    //TODO: 清除極值的API還沒做

  //若要清除最大最小值, 需要按下SET鍵3秒以上, 顯示rst再按下SET一次即可
  if(clear_limit_flag)
  {
    clear_History_value();
    clear_limit_flag = false;
  }

  //取得目前Pv值的歷史最大最小值
  get_HistoryMax();
  get_HistoryMin();
}

static void update_icon(void)
{
  ICON_degrees_API();
  // ICON_Refrigerate_ON();
  // ICON_Fan_Flashing();
  // ICON_Defrost_Flashing();
  // ICON_Alarm_ON();
  // ICON_Enhanced_Cooling_ON();
  // ICON_Energy_Saving_ON();
  // ICON_Bulb_ON();
  // ICON_Aux_ON();
  // ICON_Clock_ON();
}

static void check_set_value(void)
{
  static uint32_t check_set_value_cnt=0;
  if(System.keymode.SET_value_flag)
  {
    NumToDisplay(System.set);
    if(check_set_value_cnt == 0)
      check_set_value_cnt = tmr.Cnt_1s;

    if(tmr.Cnt_1s >= (check_set_value_cnt + AUTOLEAVE_CHECKSETTIME))
      System.keymode.SET_value_flag = false;
  }
  else
  {
    check_set_value_cnt = 0;
    System.mode = homeMode;
  }
}

static void change_set_value(void)
{
  static uint32_t change_set_value_cnt=0;
  bool cnt_reset = (IsAnyKeyPressed())? true:false;

  if(System.keymode.SET_value_flag)
  {
    NumToDisplay(System.set);
    ICON_degrees_Flashing();
    if((change_set_value_cnt == 0) || cnt_reset)
      change_set_value_cnt = tmr.Cnt_1s;

    if(tmr.Cnt_1s >= (change_set_value_cnt + AUTOLEAVE_CHANGESETTIME))
      System.keymode.SET_value_flag = false;
  }
  else 
  {
    change_set_value_cnt = 0;
    System.mode = homeMode;
  }
}

static void leave_settingMode(void)
{
  static uint8_t flash_cnt=0;
  //閃爍三次畫面
  flash_cnt = ICON_LeaveSet_Flashing(flash_cnt);

  //離開set回到home
  if(flash_cnt > (KEYLEAVE_SETTIME*2-1))
  {
    System.mode = homeMode; //短按一次後回到home模式
    sFlag.leaveSet = false;
    flash_cnt = 0;
  }
}

static void update_display_message(void)
{
  uint8_t table = 0;
  //主要顯示控制區
  switch (System.mode)
  {
    case homeMode:
      //主頁顯示邏輯, 含最大最小值清除
      if(System.keymode.Max_flag)
        NumToDisplay(System.history_max);
      else if(System.keymode.Min_flag)
        NumToDisplay(System.history_min);
      else
        NumToDisplay(System.pv);

      //更新顯示icon
      update_icon();
    break;

    case level1Mode:
      ICON_degrees_Flashing();
      //TODO: 第一層
      table = bytetable_pr1[System.level1_index];
      if(sFlag.Level1_value == Vindex)
        CharToDisplay(table);
      else if(sFlag.Level1_value == Vvalue)
        NumToDisplay(System.value[table]);
    break;

    case level2Mode:
      //TODO: 第二層
      CharToDisplay(LS);
    break;

    case settingMode:
      if(sFlag.leaveSet)
        leave_settingMode();
      else
        change_set_value();
    break;

    case checkgMode:
      //查看設定值專區
      check_set_value();
    break;

    default:
      //模式錯誤: 理論上可以顯示ERROR
    break;
  }
}

int8_t I2c_Buf_Reset[End] = {};
uint16_t temp_value = 0;
static void reset_eeprom(void)
{
  uint8_t start_addr = 0x00;
  uint8_t length = End;
  // int8_t I2c_Buf_Reset[End] = {};
  uint8_t dataindex = Set;
  // uint16_t temp_value = 0;
  
  while(dataindex < End)
  {
    // if(bytetable[dataindex].DefaultValue < 0)
    // {
    //   temp_value = (uint16_t)(bytetable[dataindex].DefaultValue*-10);
    //   temp_value |= 0x8000;   //舉起eeprom內的負數旗標
    // }
    // else
    // {
    //   temp_value = (uint16_t)(bytetable[dataindex].DefaultValue*10);
    //   temp_value &= 0x7FFF;   //清除eeprom內的負數旗標
    // }

    switch (dataindex)
    {
      case Set:
        temp_value = 0x8032;
        I2c_Buf_Reset[eep_Set_low] = temp_value & 0xFF;
        I2c_Buf_Reset[eep_Set_high] = temp_value >> 8;
        dataindex++;
        break;
      
      default:
        dataindex = End;
        break;
    }
  }

  I2C_EE_BufferWrite( I2c_Buf_Reset, start_addr , length);
}

/* Static Function definitions ------------------------------------------------------*/
static void boot_control(void)
{
  // static bool bootled_En=true;
  // static uint16_t bootled_cnt;

  if(bootled_En)
  {
    if(tmr.Cnt_1ms < BOOTonTIME)
      ALL_LED_ON();
    else if(tmr.Cnt_1ms > BOOToffTIME)
    {
      // reset_eeprom();  //平常不開, 用來重置參數的API
      read_all_eeprom_data();
      CLOSE_LED_FLAG = false;
      bootled_En = false;
    }
    else
      ALL_LED_OFF();
  }
}

static void read_all_eeprom_data(void)
{
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  I2C_EE_BufferRead(I2c_Buf_Read, 0x00 , eep_end);
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  
  // 讀出eeprom後, 寫入mcu的對應值
  if((I2c_Buf_Read[eep_Set_high]&0x80) != 0)
  {
    I2c_Buf_Read[eep_Set_high] &= 0x7F; //移除負數旗標
    System.set = I2c_Buf_Read[eep_Set_low]+(I2c_Buf_Read[eep_Set_high]<<8);
    System.set *= (-1);
  }
  else
  {
    System.set = I2c_Buf_Read[eep_Set_low]+(I2c_Buf_Read[eep_Set_high]<<8);
  }


  System.hy = I2c_Buf_Read[eep_Hy_low]+(I2c_Buf_Read[eep_Hy_high]<<8);
  System.history_max = I2c_Buf_Read[eep_max_low]+(I2c_Buf_Read[eep_max_high]<<8);
  System.history_min = I2c_Buf_Read[eep_min_low]+(I2c_Buf_Read[eep_min_high]<<8);
  System.dte = 8;
}

static void loop_100ms(void)
{
  if(tmr.Flag_100ms)
  {
    ADC_Main();
    get_Pv();
    Key_main();   //按鍵相關邏輯
    
    //啟動融霜功能
    if(sFlag.Defrost)
      sFlag.Defrost = manual_defrost(sFlag.Defrost);

    //更新最大最小值
    if(System.pv != ERROR_AD)
      update_history_value();

    //主要顯示控制區, 切換顯示模式等等..
    update_display_message();

    tmr.Flag_100ms = false;
  }
}

static void loop_100us(void)
{
  LED_Display();
  Key_debounce();     //一定要在LED切換後做判斷
  WDT_Feed();
}


/* Function definitions ------------------------------------------------------*/
void Task_Main(void)
{
  uint32_t Device_Version;

  const uint8_t Release = 0x00;
  const uint8_t dev     = 0x00;
  const uint8_t test    = 0x27;
  Device_Version = Release*65536 + dev*256 + test;

  System_Init();
  printf("初始化完成\r\n");
  printf("軟體版本: %lu\r\n", Device_Version);
  tmr.Cnt_1ms = 0;
  while(1)
  {
    /*
    * 首頁顯示量測到的溫度, 有按鍵介入才換成顯示menu
    *
    * init - 系統初始化
    * init - eeprom取值
    * init - LED全亮全滅
    * 
    * 每200ms執行一次主程式邏輯
    * 主程式 - 更新AD值
    * 主程式 - Menu/Home切換顯示內容
    * 主程式 - 更新Icon顯示
    * 主程式 - 輸出邏輯判斷
    * 
    * 每100us執行一次系統控制
    * 系統控制 - High/Low IO腳控制
    * 系統控制 - 餵狗
    * 系統控制 - 按鍵掃描(基於IO腳切換, 共6顆, loop為6 * 1ms = 6ms)
    * 
    */
    boot_control();

    loop_100ms();
    loop_100us();
  }
}