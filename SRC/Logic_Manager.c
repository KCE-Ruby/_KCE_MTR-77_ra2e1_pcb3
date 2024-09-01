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
#include "eeprom/i2c_ee.h"
#include "INC/Logic_Manager.h"

/* Private defines ----------------------------------------------------------*/
#define BOOTonTIME               (20000)      //2s = 100us*10 = 1ms*2000 = 20000次
#define BOOToffTIME              (25000)      //2s = 100us*10 = 1ms*2000 = 20000次
#define ERROR_AD                 (-999)

/* extern variables -----------------------------------------------------------------*/
extern r_tmr tmr;
extern volatile uint8_t data;
extern __IO s_Var System;
extern __IO bool CLOSE_LED_FLAG;
extern __IO Key_Manager KeyUp, KeyDown, KeyStandby, KeyBulb, KeyDefrost, KeySet;

/* variables -----------------------------------------------------------------*/
__IO uint8_t I2c_Buf_Read[eep_end] = {};


/* Private Logic API funcitons protocol ------------------------------------*/
static void Update_History_value(void);
static void Update_Icon(void);

/* Private function protocol -----------------------------------------------*/
static void read_all_eeprom_data(void);
static void boot_control(void);
static void loop_200ms(void);
static void loop_100us(void);

// uint16_t bootled_cnt;

/* static Logic API funcitons ------------------------------------------------------*/
static void Update_History_value(void)
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

static void Update_Icon(void)
{
  // ICON_Fan_Flashing();
  // ICON_degrees_Flashing();
  // ICON_Refrigerate_Flashing();
  ICON_Defrost_Flashing();
}

/* Static Function definitions ------------------------------------------------------*/
static void boot_control(void)
{
  static bool bootled_En=true;
  static uint16_t bootled_cnt;

  if(bootled_En)
  {
    if(bootled_cnt < BOOTonTIME)
      ALL_LED_ON();
    else if(bootled_cnt > BOOToffTIME)
    {
      // read_all_eeprom_data();
      CLOSE_LED_FLAG = false;
      bootled_En = false;
    }
    else
      ALL_LED_OFF();
    bootled_cnt++;
  }
}

static void read_all_eeprom_data(void)
{
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  I2C_EE_BufferRead(I2c_Buf_Read, 0x00 , eep_end);
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  
  // 讀出eeprom後, 寫入mcu的對應值
  System.history_max = I2c_Buf_Read[eep_max_low]+I2c_Buf_Read[eep_max_high]<<8;
  System.history_min = I2c_Buf_Read[eep_min_low]+I2c_Buf_Read[eep_min_high]<<8;
}

static void loop_200ms(void)
{
  if(tmr.Flag_200ms)
  {
    ADC_Main();
    Key_main();   //按鍵相關邏輯

    if(System.pv != ERROR_AD)
      Update_History_value();

    get_Pv();

    if(System.mode == menuMode)
    {
      //menu顯示邏輯, 含menu設定的數值轉換
      CharToDisplay(LS);
    }
    else
    {
      //主頁顯示邏輯, 含最大最小值清除
      if(System.keymode.Max_flag)
        NumToDisplay(System.history_max);
      else if(System.keymode.Min_flag)
        NumToDisplay(System.history_min);
      else
        NumToDisplay(System.pv);

      //更新顯示icon
      Update_Icon();
    }

    tmr.Flag_200ms = false;
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
  static uint32_t Device_Version;

  const uint8_t Release = 0x00;
  const uint8_t dev     = 0x00;
  const uint8_t test    = 0x0F;
  Device_Version = Release*65536 + dev*256 + test;

  System_Init();
  printf("初始化完成\r\n");
  printf("軟體版本: %lu\r\n", Device_Version);
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
    loop_200ms();

    loop_100us();
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MICROSECONDS);
  }
}