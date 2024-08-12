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
extern __IO bsp_io_level_t KeyPin;

/* variables -----------------------------------------------------------------*/
__IO bsp_io_level_t pin_sta[6];
static int key_valid[6];
static int key_cnt[6];
static int key_mode = 0, key_m = 0;
__IO uint8_t disp_level;

/* Private function protocol -----------------------------------------------*/
static void key_scan(void);
static void clear_key(void);

static void single_k1(uint8_t in);
/* Function definitions ------------------------------------------------------*/
void Key_main(void)
{
  Key_ReadPin();
  // key_scan();
}

static void single_k1(uint8_t in)
{
  // uint8_t i=6;
  // while(i>0)
  // {
  //   SMG_CLOSE(i);
  //   i--;
  // }
  // SMG_OPEN(key);

  // Key_ReadPin();
  // pin_sta[key] = KeyPin ? false : true;
}

static void key_scan(void)
{
  key_m = key_mode % 6;

  single_k1(key_m);
  key_mode++;

 if (pin_sta[key_m])
  key_cnt[key_m]++;
 else
  key_cnt[key_m] = 0;


 if (key_cnt[key_m] >= 30)
  key_valid[key_m]++;


 clear_key();
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
