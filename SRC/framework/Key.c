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
static bsp_io_level_t key_sta_debounced[6];
static uint8_t key_mode = 0;
__IO uint8_t disp_level;
__IO Key_Manager KeyUp, KeyDown, KeyStandby, KeyLimp, KeyDefrost, KeySet;

/* Private function protocol -----------------------------------------------*/
static Key_Manager key_detect(Key_Manager key);

/* Function definitions ------------------------------------------------------*/
void Key_main(void)
{
  /*
  * [上鍵功能]
  * Home狀態下, 單擊可查看最大值
  * Menu狀態下, 單擊向下(參數表)瀏覽參數或增加參數值
  * 
  * [下鍵功能]
  * Home狀態下, 單擊可查看最小值
  * Menu狀態下, 單擊向上(參數表)瀏覽參數或減少參數值
  * 
  * [待機鍵功能]
  * 在參數 onF=oFF時, 單擊進入待機功能;
  * 在參數 onF=ES時, 單擊進入節能運行功能;
  * 
  * [燈開關鍵功能]
  * 此鍵不可用(無作用)
  * 
  * [融霜鍵功能]
  * Home狀態下, 單擊啟動一次手動融霜
  * Menu狀態下, 無作用
  * 
  * [設定鍵功能]
  * 
  * 
  * [組合鍵功能]
  * [上+下] = 鎖定or解鎖鍵盤
  * [設定+下] = 進入Menu模式
  * [設定+上] = 退出Menu回Home
  */

}

void Key_debounce(void)
{
  switch (tmr.COM_Port)
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

    case key_limp:
      KeyLimp = key_detect(KeyLimp);
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
  key_sta = pin_sta[tmr.COM_Port];

  //按下時, 計數器++
  if(key_sta)
    key.Cnt++;
  // else
  // {
  //   //放開時, 判斷計數器被按下多久, 分成長按以及短按
  //   if(key.Cnt>=KEY_DEBOUNCE_300)
  //     key.LongPressed++;

  //   if(key.Cnt>=KEY_DEBOUNCE_30)
  //   {
  //     key.shortPressed++;
  //     if(key.shortPressed > KEY_cnt_max)
  //       key.shortPressed = KEY_cnt_min;
  //   }
  //   key.Cnt=0;
  // }

  return key;
}


