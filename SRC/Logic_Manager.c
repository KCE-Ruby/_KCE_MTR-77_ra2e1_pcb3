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
#include "INC/framework/eep_api.h"
#include "INC/Logic_Manager.h"

/* Private defines ----------------------------------------------------------*/
#define BOOTonTIME                         (4500)      //4.5 = 1ms*4500 = 4500次
#define BOOToffTIME                        (5000)      //+0.5s = 1ms*500 = 500次
#define KEYLEAVE_SETTIME                      (3)      //按鍵離開設定層的閃爍時間, 單位:秒
#define AUTOLEAVE_CHECKSETTIME               (12)      //檢查設定值多久後自動跳出, 單位:秒
#define AUTOLEAVE_CHANGESETTIME              (15)      //修改設定值多久後自動跳出, 單位:秒

/* extern variables -----------------------------------------------------------------*/
extern r_tmr tmr;
extern volatile uint8_t data;
extern __IO s_Var Syscfg, Preload;
extern __IO s_Flag sFlag;
extern __IO ADC_TemperatureValue TempValue;

extern __IO bool CLOSE_LED_FLAG;
extern __IO Key_Manager KeyUp, KeyDown, KeyStandby, KeyBulb, KeyDefrost, KeySet;
// extern __IO ByteSettingTable bytetable[End];
extern __IO uint8_t bytetable_pr1[End];
extern __IO uint8_t bytetable_pr2[End];
extern __IO uint32_t catch_s[dly_end_sec];
extern __IO uint32_t catch_ms[dly_end_ms];

/* variables -----------------------------------------------------------------*/
__IO uint8_t I2c_Buf_Read[eep_end] = {};
__IO bool clear_Max_flag=0, clear_Min_flag=0;
__IO bool bootled_En=true;
uint8_t EE_Buf_Read[255];
uint8_t I2c_Buf_Reset[SPIAddr_End];

/* static update_display_message API Functions -------------------------------*/
static void homeModelogic(bool* fHigh, bool* fLow, bool* fLeave);
static void historyModelogic(bool* fHigh, bool* fLow, bool* fLeave);
static void level1Modelogic(void);
static void level2Modelogic(void);
static void valuetodisplay(uint8_t table);

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

/* static update_display_message API Functions -------------------------------*/
static void homeModelogic(bool* fHigh, bool* fLow, bool* fLeave)
{
  //顯示目前量測到的溫度
  if(TempValue.sensor1 == ERROR_AD)
    P1ToDisplay_Flashing();
  // else if (TempValue.sensor2 == ERROR_AD)
  //   P2ToDisplay();
  else
  {
    if(Syscfg.value[rES] == DECIMAL_AT_0)
      NumToDisplay(Syscfg.pv/10);
    else if(Syscfg.value[rES] == DECIMAL_AT_1)
      NumToDisplay(Syscfg.pv);
  }

  //更新顯示icon
  update_icon();

  //清除所有歷史極值相關旗標及參數
  *fHigh = false;
  *fLow = false;
  *fLeave = false;
  Syscfg.keymode.Max_flag = false;
  Syscfg.keymode.Min_flag = false;
  Syscfg.keymode.clear_flag = false;
  catch_ms[dly_fHigh] = 0;
  catch_ms[dly_fLow] = 0;
  catch_ms[dly_fLeave] = 0;
}

static void historyModelogic(bool* fHigh, bool* fLow, bool* fLeave)
{
  if(Syscfg.keymode.Max_flag)
  {
    clear_Max_flag = true;
    clear_Min_flag = false;
    *fLow = false; //更換最大最小值模式後清除另一個的顯示字符時間
    //顯示一秒後, 顯示歷史最大數值
    HiToDisplay();
    if(*fHigh == false)
      *fHigh = Mydelay_ms(dly_fHigh, 1000);
    else
    {
      //顯示數值, 5秒後離開這層
      if(Syscfg.value[rES] == DECIMAL_AT_0)
        NumToDisplay(Syscfg.history_max/10);
      else if(Syscfg.value[rES] == DECIMAL_AT_1)
        NumToDisplay(Syscfg.history_max);

      if(*fLeave == false)
        *fLeave = Mydelay_ms(dly_fLeave, 5000);
      else
        Syscfg.mode = homeMode;
    }
    // printf("HIS_dly_f_H = %d\r\n", *fHigh);
  }
  else if(Syscfg.keymode.Min_flag)
  {
    clear_Min_flag = true;
    clear_Max_flag = false;
    *fHigh = false; //更換最大最小值模式後清除另一個的顯示字符時間
    //顯示一秒後, 顯示歷史最小數值
    LoToDisplay();
    if(*fLow == false)
      *fLow = Mydelay_ms(dly_fLow, 1000);
    else
    {
      //顯示數值, 5秒後離開這層
      if(Syscfg.value[rES] == DECIMAL_AT_0)
        NumToDisplay(Syscfg.history_min/10);
      else if(Syscfg.value[rES] == DECIMAL_AT_1)
        NumToDisplay(Syscfg.history_min);

      if(*fLeave == false)
        *fLeave = Mydelay_ms(dly_fLeave, 5000);
      else
        Syscfg.mode = homeMode;
    }
    // printf("HIS_dly_f_L = %d\r\n", *fLow);
  }
  else  //非正在顯示極值時
  {
    if(Syscfg.keymode.clear_flag)
    {
      //代表長按SET超過3秒, 開始閃爍
      if(rStToDisplay_Flashing()==true)
      {
        //閃爍rSt3次後離開
        Syscfg.mode = homeMode;
        Syscfg.keymode.clear_flag = false;
      }
    }
    else  //長按SET的時候長亮rSt鍵
      rStToDisplay();
  }
}

static void level1Modelogic(void)
{
  uint8_t table = 0;
  ICON_degrees_Flashing();
  table = bytetable_pr1[Syscfg.level1_index];
  if(sFlag.Level1_value == Vindex)
    CharToDisplay(table);
  else if(sFlag.Level1_value == Vvalue)
    valuetodisplay(table);
  else if(sFlag.Level1_value == Pr2_symbol)
    CharToDisplay(pr2);
}

static void level2Modelogic(void)
{
  uint8_t table = 0;
  ICON_degrees_Flashing();
  table = bytetable_pr2[Syscfg.level2_index];
  if(sFlag.Level2_value == Vindex)
    CharToDisplay(table);
  else if(sFlag.Level2_value == Vvalue)
    valuetodisplay(table);
}

static void settingModelogic(void)
{
  if(sFlag.leaveSet)
    leave_settingMode();
  else
    change_set_value();
}

static void checkgModelogic(void)
{
  //查看設定值專區
  check_set_value();
}

/* static Logic API funcitons ------------------------------------------------------*/
static void valuetodisplay(uint8_t table)
{
  switch (table)
  {
    case P2P:
    case P3P:
    case P4P:
      nyToDisplay((bool)Preload.value[table]);
      break;

    case CF:
      CFToDisplay((bool)Preload.value[table]);
      break;

    case rES:
      rESToDisplay((bool)Preload.value[table]);
      break;
  
    default:
      if(Syscfg.value[rES] == DECIMAL_AT_0)
        NumToDisplay(Preload.value[table]/10);
      else if(Syscfg.value[rES] == DECIMAL_AT_1)
        NumToDisplay(Preload.value[table]);
      break;
  }

}

static void update_history_value(void)
{
  //取得目前Pv值的歷史最大最小值
  get_HistoryMax();
  get_HistoryMin();
}

static void update_icon(void)
{
  /* 更新icon狀態
  *  華/攝氏_燈號控制
  *  壓縮機啟動_燈號控制
  */
  ICON_degrees_API();
  ICON_Refrigerate_API();
  ICON_Enhanced_Cooling_API();
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
  if(Syscfg.keymode.SET_value_flag)
  {
    if(Syscfg.value[rES] == DECIMAL_AT_0)
      NumToDisplay(Syscfg.value[Set]/10);
    else if(Syscfg.value[rES] == DECIMAL_AT_1)
      NumToDisplay(Syscfg.value[Set]);

    if(check_set_value_cnt == 0)
      check_set_value_cnt = tmr.Cnt_1s;

    if(tmr.Cnt_1s >= (check_set_value_cnt + AUTOLEAVE_CHECKSETTIME))
      Syscfg.keymode.SET_value_flag = false;
  }
  else
  {
    check_set_value_cnt = 0;
    Syscfg.mode = homeMode;
  }
}

static void change_set_value(void)
{
  static uint32_t change_set_value_cnt=0;
  bool cnt_reset = (IsAnyKeyPressed())? true:false;

  if(Syscfg.keymode.SET_value_flag)
  {
    if(Syscfg.value[rES] == DECIMAL_AT_0)
      NumToDisplay(Preload.value[Set]/10);
    else if(Syscfg.value[rES] == DECIMAL_AT_1)
      NumToDisplay(Preload.value[Set]);

    ICON_degrees_Flashing();
    if((change_set_value_cnt == 0) || cnt_reset)
      change_set_value_cnt = tmr.Cnt_1s;

    if(tmr.Cnt_1s >= (change_set_value_cnt + AUTOLEAVE_CHANGESETTIME))
      Syscfg.keymode.SET_value_flag = false;
  }
  else 
  {
    change_set_value_cnt = 0;
    Syscfg.mode = homeMode;
  }
}

static void leave_settingMode(void)
{
  static uint8_t flash_cnt=0;

  upload_syscfg_data(Set);
  //閃爍三次畫面
  flash_cnt = ICON_LeaveSet_Flashing(flash_cnt);

  //離開set回到home
  if(flash_cnt > (KEYLEAVE_SETTIME*2-1))
  {
    Syscfg.mode = homeMode; //短按一次後回到home模式
    flash_cnt = 0;
    sFlag.leaveSet = false;
  }
}

static void update_display_message(void)
{
  static bool dly_flag_H=false, dly_flag_L=false, leave_flag=false;
  //主要顯示控制區
  switch (Syscfg.mode)
  {
    case homeMode:
      homeModelogic(&dly_flag_H, &dly_flag_L, &leave_flag);
      // printf("HOME_dly_f_H = %d, ", dly_flag_H);
      // printf("HOME_dly_f_L = %d, ", dly_flag_L);
      // printf("HOME_leave_f = %d\r\n", leave_flag);
    break;

    case historyMode:
      historyModelogic(&dly_flag_H, &dly_flag_L, &leave_flag);
    break;

    case level1Mode:      //用戶層-level 1
      level1Modelogic();
    break;

    case level2Mode:      //隱藏層-level 2
      level2Modelogic();
    break;

    case settingMode:
      settingModelogic();
    break;

    case checkgMode:
      checkgModelogic();
    break;

    default:
      //模式錯誤: 理論上可以顯示ERROR
    break;
  }
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
//      read_all_eeprom_data();
      CLOSE_LED_FLAG = false;
      bootled_En = false;
    }
    else
      ALL_LED_OFF();
  }
}

static void read_all_eeprom_data(void)
{
  // R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  // I2C_EE_BufferRead(I2c_Buf_Read, 0x00 , eep_end);
  // R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);

  // 讀出eeprom後, 寫入mcu的對應值
  offset_EEtoSYS();
}

static void loop_100ms(void)
{
  if(tmr.Flag_1ms)
  {
    Key_main();   //按鍵相關邏輯
    tmr.Flag_1ms = false;
  }
  if(tmr.Flag_100ms)
  {
    ADC_Main();
    get_Pv();
    // Key_main();   //按鍵相關邏輯
    Out_main();
    
    //啟動融霜功能
    if(sFlag.Defrost)
      sFlag.Defrost = manual_defrost(sFlag.Defrost);

    //更新最大最小值
    if(Syscfg.pv != ERROR_AD)
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
  //整個系統運算的值無小數點, 已放大10倍計算, ex. (12.5)->(125), (-55.9)->(-559)
  uint32_t Device_Version;

  const uint8_t Release = 0x00;
  const uint8_t dev     = 0x00;
  const uint8_t test    = 0x55;
  Device_Version = Release*65536 + dev*256 + test;

  EEPROM_TEST();   //單獨測試EEPROM寫入讀出功能

  System_Init();
  // UserTabletoSytem();
  // EEP_ResetALL();
  // I2C_EE_BufferRead(EE_Buf_Read, 0x00, 0xFF);
  // offset_EEtoSYS();

  printf("初始化完成\r\n");
  printf("軟體版本: 0x%02X\r\n", Device_Version);
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
