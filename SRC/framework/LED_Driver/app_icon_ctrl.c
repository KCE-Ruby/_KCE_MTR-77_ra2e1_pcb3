/**
 ******************************************************************************
 * @file    app_icon_ctrl.c
 * @author  KCE R&D Team
 * @date    2024-07-09
 * @author  Ruby
 * 面板顯示的應用層, ICON的開關控制
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/LED_Driver/app_icon_ctrl.h"
#include "INC/framework/LED_Driver/Indicator_encode.h"
#include "INC/framework/LED_Driver/app_menu_ctrl.h"
#include "INC/framework/datapool.h"

/* Private defines ----------------------------------------------------------*/
#define ICON_FLASH_FREQUENCY   1   //default: 1Hz

/* extern variables -----------------------------------------------------------------*/
extern __IO r_tmr tmr;
extern __IO ICON_SCAN4 Scan4temp;
extern __IO ICON_SCAN5 Scan5temp;
extern __IO s_Var System;

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

//離開設定層需先閃爍三次
uint8_t ICON_LeaveSet_Flashing(uint8_t flash_cnt)
{
  static bool flag_pre=false;
  bool flag=false;  //頻率為1Hz or 2Hz
  flag = Flash_timer_setting();
  //溫度icon, 系統處於設定模式中, 閃爍中, 預設為:1Hz 500ms亮, 500ms滅
  if(flag == true)
  {
    //離開設定層時, 數字&依據溫度單位閃爍燈號3次
    NumToDisplay(System.value[Set]);
    if(System.value[CF] == degree_C)
    {
      ICON_degrees_Celsius_ON();
      ICON_degrees_Fahrenheit_OFF();
    }
    else if(System.value[CF] == degree_F)
    {
      ICON_degrees_Fahrenheit_ON();
      ICON_degrees_Celsius_OFF();
    }
  }
  else
  {
    NumToDisplay(CLEARALL);
    ICON_degrees_Celsius_OFF();
    ICON_degrees_Fahrenheit_OFF();
  }
  
  if(flag_pre != flag)
  {
    flash_cnt++;
    flag_pre = flag;
  }
  return flash_cnt;
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

//溫度單位icon各種型態, 開, 關; 兩個一起閃爍中
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
    if((System.mode == level1Mode) || (System.mode == settingMode))
    {
      //在用戶層及設定層內, 依據溫度單位閃爍燈號
      if(System.value[CF] == degree_C)
      {
        ICON_degrees_Celsius_ON();
        ICON_degrees_Fahrenheit_OFF();
      }
      else if(System.value[CF] == degree_F)
      {
        ICON_degrees_Fahrenheit_ON();
        ICON_degrees_Celsius_OFF();
      }
    }
    else
    {
      //在隱藏層內, 攝氏跟華氏的單位燈號皆閃爍
      ICON_degrees_Celsius_ON();
      ICON_degrees_Fahrenheit_ON();
    }
  }
  else
  {
    ICON_degrees_Celsius_OFF();
    ICON_degrees_Fahrenheit_OFF();
  }
}

void ICON_degrees_API(void)
{
  if(System.value[CF] == degree_C)
  {
    ICON_degrees_Celsius_ON();
    ICON_degrees_Fahrenheit_OFF();
  }
  else if(System.value[CF] == degree_F)
  {
    ICON_degrees_Fahrenheit_ON();
    ICON_degrees_Celsius_OFF();
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


/* 規格書尚未定義, 但是硬體有, 目前在邏輯上不會亮 ------------------------------------------------------*/
//燈泡燈號
void ICON_Bulb_ON(void)
{
  //燈泡icon, 動作啟動, 長亮
  Scan4temp.scan4.bulb = true;
}

void ICON_Bulb_OFF(void)
{
  //燈泡icon, 動作關閉, 熄滅
  Scan4temp.scan4.bulb = false;
}

//Aux燈號
void ICON_Aux_ON(void)
{
  //Aux icon, 動作啟動, 長亮
  Scan4temp.scan4.aux = true;
}

void ICON_Aux_OFF(void)
{
  //Aux icon, 動作關閉, 熄滅
  Scan4temp.scan4.aux = false;
}

//計時器燈號
void ICON_Clock_ON(void)
{
  //計時器icon, 動作啟動, 長亮
  Scan4temp.scan4.clock = true;
}

void ICON_Clock_OFF(void)
{
  //計時器icon, 動作關閉, 熄滅
  Scan4temp.scan4.clock = false;
}

//rST重置歷史極值的字符
bool rStToDisplay_Flashing(void)
{
  static uint8_t flash_cnt=0;
  static bool pre_flag=false;
  bool ret=false;
  bool flag;  //頻率為1Hz or 2Hz
  flag = Flash_timer_setting();
  //溫度icon, 系統處於設定模式中, 閃爍中, 預設為:1Hz 500ms亮, 500ms滅
  if(flag == true)
  {
    rStToDisplay();
    pre_flag = flag;
  }
  else
  {
    CharToDisplay(xxx);
    if(pre_flag==true)
    {
      //計算切換了幾次
      flash_cnt++;
      pre_flag = flag;
    }
  }
  if(flash_cnt>3)
  {
    flash_cnt = 0;
    ret = true;
    pre_flag=false;
  }
  return ret;
}

