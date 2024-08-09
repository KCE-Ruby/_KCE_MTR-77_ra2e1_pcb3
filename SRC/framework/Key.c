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

/* extern variables -----------------------------------------------------------------*/


/* variables -----------------------------------------------------------------*/
__IO uint8_t KeyPin;
static int key_valid[6];
__IO uint8_t pin_sta[6];
static int key_cnt[6];
static int key_mode = 0, key_m = 0;
__IO uint8_t disp_level;

/* Private function protocol -----------------------------------------------*/
static void key_scan(void);
static void clear_key(void);

static single_k1(void);
/* Function definitions ------------------------------------------------------*/
void Key_main(void)
{
//  Key_Read;
//   key_scan();
  single_k1();
}

static single_k1(void)
{
  // 關閉當前段，打開下一段
  SMG_1_CLOSE;
  SMG_2_CLOSE;
  SMG_3_CLOSE;
  SMG_4_CLOSE;
  SMG_5_CLOSE;
  SMG_6_CLOSE;
  SMG_6_OPEN;
  Key_Read;
  // 假設 KeyPin 是一個函數或宏，返回當前按鍵狀態
  pin_sta[0] = KeyPin ? false : true;
}

static void key_scan(void)
{
  key_m = key_mode % 6;

  // 用數組來簡化開關的切換
  const int SMG_CLOSE[6] = {SMG_6_CLOSE, SMG_1_CLOSE, SMG_2_CLOSE, SMG_3_CLOSE, SMG_4_CLOSE, SMG_5_CLOSE};
  const int SMG_OPEN[6] = {SMG_1_OPEN, SMG_2_OPEN, SMG_3_OPEN, SMG_4_OPEN, SMG_5_OPEN, SMG_6_OPEN};

  // 關閉當前段，打開下一段
  SMG_CLOSE[key_m];
  SMG_OPEN[key_m];

  key_mode++;

  // 假設 KeyPin 是一個函數或宏，返回當前按鍵狀態
  pin_sta[key_m] = KeyPin ? false : true;

//  if (pin_sta[key_m])
//  {
//      key_cnt[key_m]++;
//  }
//  else
//  {
//      key_cnt[key_m] = 0;
//  }
//
//  if (key_cnt[key_m] >= 30)
//  {
//      key_valid[key_m]++;
//  }
//
//  clear_key();
}

static void clear_key(void)
{
  for (int i = 0; i < 6; ++i)
  {
    if (key_valid[i] > 0)
    {
      for (int j = 0; j < 6; ++j)
      {
        if (i != j)
        {
          key_valid[j] = 0;
        }
      }
      break; // 找到第一個大於0的值後就可以退出迴圈
    }
  }
}
