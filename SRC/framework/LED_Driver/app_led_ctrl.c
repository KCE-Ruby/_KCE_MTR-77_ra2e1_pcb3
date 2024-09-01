/**
 ******************************************************************************
 * @file    app_led_ctrl.c
 * @author  KCE R&D Team
 * @date    2024-07-09
 * @author  Ruby
 * 面板顯示的應用層, ICON的開關控制
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/LED_Driver/app_led_ctrl.h"
#include "INC/framework/LED_Driver/Indicator_encode.h"

/* Private defines ----------------------------------------------------------*/
#define ICON_FLASH_FREQUENCY   1   //default: 1Hz

/* extern variables -----------------------------------------------------------------*/
extern __IO r_tmr tmr;
extern __IO ICON_SCAN4 Scan4temp;
extern __IO ICON_SCAN5 Scan5temp;

/* variables -----------------------------------------------------------------*/
__IO bool ALL_LED_FLAG, CLOSE_LED_FLAG;

/* static private function protocol -----------------------------------------------*/
static bool Flash_timer_setting(void);

/* static Private Function definitions ------------------------------------------------------*/
static bool Flash_timer_setting(void)
{
  bool flag;  //icon閃爍頻率為1Hz or 2Hz
  switch (ICON_FLASH_FREQUENCY)
  {
    case 1:
      flag = tmr.FlashFlag_1Hz;
      break;
    case 2:
      flag = tmr.FlashFlag_2Hz;
      break;
    
    default:
      flag = false;
      break;
  }
  return flag;
}

/* Function definitions ------------------------------------------------------*/
void ALL_LED_ON(void)
{
  //TODO: S1~S5掃描全亮, a~dp全開
  ALL_LED_FLAG = true;
  CLOSE_LED_FLAG = false;
}

void ALL_LED_OFF(void)
{
  //TODO: S1~S5掃描全關, a~dp 75HC595掃描全關
  ALL_LED_FLAG = false;
  CLOSE_LED_FLAG = true;
}

//製冷icon三種型態, 開; 關; 閃爍中
void ICON_Refrigerate_ON(void)
{
  //TODO: 製冷icon, 動作啟動, 長亮
  Scan4temp.scan4.refrigerate = true;
}

void ICON_Refrigerate_OFF(void)
{
  //TODO: 製冷icon, 動作關閉, 熄滅
  Scan4temp.scan4.refrigerate = false;
}

void ICON_Refrigerate_Flashing(void)
{
  bool flag;  //頻率為1Hz or 2Hz
  flag = Flash_timer_setting();
  //製冷icon, 防頻啟動等待時間, 閃爍中, 預設為:1Hz 500ms亮, 500ms滅
  if(flag == true)
    ICON_Refrigerate_ON();
  else
    ICON_Refrigerate_OFF();
}

//融霜icon三種型態, 開; 關; 閃爍中
void ICON_Defrost_ON(void)
{
  //TODO: 融霜icon, 動作啟動, 長亮
  Scan4temp.scan4.defrost = true;
}

void ICON_Defrost_OFF(void)
{
  //TODO: 融霜icon, 動作關閉, 熄滅
  Scan4temp.scan4.defrost = false;
}

void ICON_Defrost_Flashing(void)
{
  bool flag;  //頻率為1Hz or 2Hz
  flag = Flash_timer_setting();
  //融霜icon, 融霜後滴水時間, 閃爍中, 預設為:1Hz 500ms亮, 500ms滅
  if(flag == true)
    ICON_Defrost_ON();
  else
    ICON_Defrost_OFF();
}

//風扇icon三種型態, 開; 關; 閃爍中
void ICON_Fan_ON(void)
{
  //TODO: 風扇icon, 動作啟動, 長亮
  Scan5temp.scan5.fan = true;
}

void ICON_Fan_OFF(void)
{
  //TODO: 風扇icon, 動作關閉, 熄滅
  Scan5temp.scan5.fan = false;
}

void ICON_Fan_Flashing(void)
{
  static bool flag=false;  //頻率為1Hz or 2Hz
  flag = Flash_timer_setting();
  //風扇icon, 融霜後啟動延時, 閃爍中, 預設為:1Hz 500ms亮, 500ms滅
  if(flag == true)
    ICON_Fan_ON();
  else
    ICON_Fan_OFF();
}

//溫度icon各種型態, 開, 關; 兩個一起閃爍中
void ICON_degrees_Celsius_ON(void)
{
  //TODO: 攝氏溫度icon, 選擇此單位時, 長亮
  Scan5temp.scan5.degress_c = true;
}

void ICON_degrees_Celsius_OFF(void)
{
  //TODO: 攝氏溫度icon, 關閉
  Scan5temp.scan5.degress_c = false;
}

void ICON_degrees_Fahrenheit_ON(void)
{
  //TODO: 華氏溫度icon, 選擇此單位時, 長亮
  Scan5temp.scan5.degress_f = true;
}

void ICON_degrees_Fahrenheit_OFF(void)
{
  //TODO: 華氏溫度icon, 關閉
  Scan5temp.scan5.degress_f = false;
}

void ICON_degrees_Flashing(void)
{
  bool flag;  //頻率為1Hz or 2Hz
  flag = Flash_timer_setting();
  //溫度icon, 系統處於設定模式中, 閃爍中, 預設為:1Hz 500ms亮, 500ms滅
  if(flag == true)
  {
    ICON_degrees_Celsius_ON();
    ICON_degrees_Fahrenheit_ON();
  }
  else
  {
    ICON_degrees_Celsius_OFF();
    ICON_degrees_Fahrenheit_OFF();
  }
}

//警報
void ICON_Alarm_ON(void)
{
  //TODO: 警報icon, 動作啟動, 長亮
  Scan4temp.scan4.alarm = true;
}

void ICON_Alarm_OFF(void)
{
  //TODO: 警報icon, 動作關閉, 熄滅
  Scan4temp.scan4.alarm = false;
}

//加強製冷
void ICON_Enhanced_Cooling_ON(void)
{
  //TODO: 加強製冷icon, 動作啟動, 長亮
  Scan4temp.scan4.enhanced_cooling = true;
}

void ICON_Enhanced_Cooling_OFF(void)
{
  //TODO: 加強製冷icon, 動作關閉, 熄滅
  Scan4temp.scan4.enhanced_cooling = false;
}

//節能運行
void ICON_Energy_Saving_ON(void)
{
  //TODO: 節能運行icon, 動作啟動, 長亮
  Scan4temp.scan4.energy_saving = true;
}

void ICON_Energy_Saving_OFF(void)
{
  //TODO: 節能運行icon, 動作關閉, 熄滅
  Scan4temp.scan4.energy_saving = false;
}









