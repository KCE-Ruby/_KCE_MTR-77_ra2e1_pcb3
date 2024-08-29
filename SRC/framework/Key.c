/**
 ******************************************************************************
 * @file    Key.c
 * @author  KCE R&D Team
 * @date    2024-06-26
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/Key.h"
#include "INC/framework/Display.h"

/* Private defines ----------------------------------------------------------*/
#define KEY_DEBOUNCE_300              (300)      //(單位為:次), 短按
#define KEY_DEBOUNCE_30               (30)      //(單位為:次), 長按
#define KEY_cnt_max                   (300)
#define KEY_cnt_min                   (1)

/* extern variables -----------------------------------------------------------------*/
extern __IO bsp_io_level_t KeyPin, pin_sta[6];
extern __IO s_Var System;
extern __IO r_tmr tmr;

/* variables -----------------------------------------------------------------*/
__IO uint8_t disp_level;
__IO Key_Manager KeyUp, KeyDown, KeyStandby, KeyBulb, KeyDefrost, KeySet;

/* Private function protocol -----------------------------------------------*/
static Key_Manager key_detect(Key_Manager key);
static key_up_function(void);
static key_down_function(void);
static key_standby_function(void);
static key_bulb_function(void);
static key_defrost_function(void);
static key_set_function(void);

/* Function definitions ------------------------------------------------------*/
void Key_main(void)
{
  //[上鍵功能]
  key_up_function();

  //[下鍵功能]
  key_down_function();

  //[待機鍵功能]
  key_standby_function();

  //[燈開關鍵功能]
  key_bulb_function();

  //[融霜鍵功能]
  key_defrost_function();

  //[設定鍵功能]
  key_set_function();
}

void Key_debounce(void)
{
  switch (tmr.COM_Port+1)
  {
    case keyup:
      KeyUp = key_detect(KeyUp);
    break;

    case keydown:
      KeyDown = key_detect(KeyDown);
    break;

    case keystandby:
      KeyStandby = key_detect(KeyStandby);
    break;

    case key_bulb:
      KeyBulb = key_detect(KeyBulb);
    break;

    case key_forst:
      KeyDefrost = key_detect(KeyDefrost);
    break;

    case keyset:
      KeySet = key_detect(KeySet);
    break;
  }
}

static Key_Manager key_detect(Key_Manager key)
{
  // Key HW config, Release = High; Pressed = Low
  bool key_sta;
  key_sta = pin_sta[tmr.COM_Port+1];

  //按下時, 計數器++
  if(key_sta)
    key.Cnt++;
  else
  {
    //放開時, 判斷計數器被按下多久, 分成長按以及短按
    if(key.Cnt>=KEY_DEBOUNCE_300)
      key.LongPressed++;

    if(key.Cnt>=KEY_DEBOUNCE_30)
    {
      key.shortPressed++;
      if(key.shortPressed > KEY_cnt_max)
        key.shortPressed = KEY_cnt_min;
    }
    key.Cnt=0;
  }

  return key;
}

static key_up_function(void)
{
  /*[上鍵功能]
  * Home狀態下, 單擊可查看最大值
  * Menu狀態下, 單擊向下(參數表)瀏覽參數
  * Setting狀態下, 單擊增加參數值
  * [組合鍵功能]
  * [上+下] = 鎖定or解鎖鍵盤
  */

 if(KeyUp.shortPressed)
 {
    System.keymode.Max_flag = false;
    switch (System.mode)
    {
      case homeMode:
        System.keymode.Max_flag = true;
      break;

      case menuMode:
        //TODO:要先帶入現有的數值再++
        System.value++;
      break;

      case settingMode:
        System.keymode.index++;
      break;

      default:
        System.keymode.Max_flag = false;
        System.keymode.Min_flag = false;
      break;
    }
    KeyUp.shortPressed = 0;
 }
}

static key_down_function(void)
{
  /*[下鍵功能]
  * Home狀態下, 單擊可查看最小值
  * Menu狀態下, 單擊向上(參數表)瀏覽參數
  * Setting狀態下, 單擊減少參數值
  */

 if(KeyDown.shortPressed)
 {
    System.keymode.Min_flag = false;
    switch (System.mode)
    {
      case homeMode:
        System.keymode.Min_flag = true;
      break;

      case menuMode:
        //TODO:要先帶入現有的數值再++
        System.value++;
      break;

      case settingMode:
        System.keymode.index--;
      break;

      default:
        System.keymode.Max_flag = false;
        System.keymode.Min_flag = false;
      break;
    }
    KeyDown.shortPressed = 0;
 }
}

static key_standby_function(void)
{
  /*[待機鍵功能]
  * 在參數 onF=oFF時, 單擊進入待機功能;
  * 在參數 onF=ES時, 單擊進入節能運行功能;
  */

 if(KeyStandby.shortPressed)
 {
    //TODO: 按下按鍵後的功能
    KeyStandby.shortPressed = 0;
 }
}

static key_bulb_function(void)
{
  /*[燈開關鍵功能]
  * 此鍵不可用(無作用)
  */

 if(KeyBulb.shortPressed)
 {
    //TODO: 按下按鍵後的功能
    KeyBulb.shortPressed = 0;
 }
}

static key_defrost_function(void)
{
  /*[融霜鍵功能]
  * Home狀態下, 單擊啟動一次手動融霜
  * Menu狀態下, 無作用
  */

 if(KeyDefrost.shortPressed)
 {
    //TODO: 按下按鍵後的功能
    KeyDefrost.shortPressed = 0;
 }
}

static key_set_function(void)
{
  /*[設定鍵功能]
  * Home狀態下, 單擊顯示目標設定點(Pv)
  * Menu狀態下, 無作用
  * Setting狀態下, 單擊選擇參數或確認操作
  * [組合鍵功能]
  * [設定+下] = 進入Menu模式
  * [設定+上] = 退出Menu回Home
  */

 if(KeySet.shortPressed)
 {
    //TODO: 按下按鍵後的功能
    KeySet.shortPressed = 0;
 }
}

/*--------------------------- for test key funcitons ---------------------------*/
// uint8_t keyPressed[7];
// __IO uint16_t key_Cnt[7];
// void Key_test(void)
// {
//   //輪巡按鍵測試
//   static uint8_t t_key=1;
//   bool key_sta;
  
//   if(t_key > 6) t_key = 1;
//   SMG_CLOSE(1);
//   SMG_CLOSE(2);
//   SMG_CLOSE(3);
//   SMG_CLOSE(4);
//   SMG_CLOSE(5);
//   SMG_CLOSE(6);
//   SMG_OPEN(t_key);

//   R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_14, &KeyPin);
//   pin_sta[t_key] = KeyPin ? false : true;

//   key_sta = pin_sta[t_key];
//   if(key_sta)
//     key_Cnt[t_key]++;
//   else
//   {
//     //放開時, 判斷計數器是否有被按下
//     if(key_Cnt[t_key] > 0)
//     {
//       keyPressed[t_key]++;
//       key_Cnt[t_key]=0;
//     }
//   }
//   t_key++;
// }

// void Key_oneshot(void)
// {
//   //單擊測試, KeyPin->HIGH為放開, LOW為按下
//   // 1->右下(keystandby); 2->右上(keyup); 3->左上(key_limp)
//   // 4->左中(key_forst); 5->右中(keydown); 6->左下(keyset)
//   uint8_t t_key=6;
//   fsp_err_t err;
//   SMG_OPEN(t_key);
//   SMG_CLOSE(1);
//   SMG_CLOSE(2);
//   SMG_CLOSE(3);
//   SMG_CLOSE(4);
//   SMG_CLOSE(5);
//   err = R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_14, &KeyPin);
//   pin_sta[t_key] = KeyPin ? false : true;
// }
