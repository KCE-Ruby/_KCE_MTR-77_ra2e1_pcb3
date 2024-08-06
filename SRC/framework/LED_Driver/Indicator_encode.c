/**
 ******************************************************************************
 * @file    Indicator_encode.c
 * @author  KCE R&D Team
 * @date    2024-07-09
 * @author  Ruby
 * MCU與LED腳位控制硬體層設定
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/LED_Driver/Indicator_encode.h"


/* Private defines ----------------------------------------------------------*/

/* Private macro ----------------------------------------------------------*/
const uint8_t digitMap[10] = {
    0x3F, // 0: A B C D E F
    0x06, // 1: B C
    0x5B, // 2: A B D E G
    0x4F, // 3: A B C D G
    0x66, // 4: B C F G
    0x6D, // 5: A C D F G
    0x7D, // 6: A C D E F G
    0x07, // 7: A B C
    0x7F, // 8: A B C D E F G
    0x6F  // 9: A B C D F G
};

const char _7seg_LookupTable[][8] = {
  {1,1,1,1,0,0,1,1},     // diplay number '0'  F3
  {1,0,0,0,0,0,0,1},     // diplay number '1'  81
  {0,1,1,0,1,0,1,1},     // diplay number '2'  6B
  {1,1,0,0,1,0,1,1},     // diplay number '3'  CB
  {1,0,0,1,1,0,1,1},     // diplay number '4'  99
  {1,1,0,1,1,0,1,0},     // diplay number '5'  DA
  {1,1,1,1,1,0,1,0},     // diplay number '6'  FA
  {1,0,0,0,0,0,1,1},     // diplay number '7'  83
  {1,1,1,1,1,0,1,1},     // diplay number '8'  FB
  {1,1,0,1,1,0,1,1},     // diplay number '9'  DB
};

const char _char_LookupTable[][8] = {   //TODO: 未校正
  {1,1,1,1,0,1,1,0},     // diplay number 'A'  BB
  {1,1,1,1,0,1,1,0},     // diplay number 'b'  F8
  {1,1,1,1,0,1,1,0},     // diplay number 'C'  72
  {1,1,1,1,0,1,1,0},     // diplay number 'c'  68
  {1,1,1,1,0,1,1,0},     // diplay number 'd'  E9
  {1,1,1,1,0,1,1,0},     // diplay number 'E'  7A
  {1,1,1,1,0,1,1,0},     // diplay number 'F'  3A

  {1,1,1,1,0,1,1,0},     // diplay number 'G'  F2
  {1,1,1,1,0,1,1,0},     // diplay number 'H'  B9
  {1,1,1,1,0,1,1,0},     // diplay number 'i'  80
  {1,1,1,1,0,1,1,0},     // diplay number 'J'  E1
  {1,1,1,1,0,1,1,0},     // diplay number 'K'  79
  {1,1,1,1,0,1,1,0},     // diplay number 'L'  70
  {1,1,1,1,0,1,1,0},     // diplay number 'M'  B3
  {1,1,1,1,0,1,1,0},     // diplay number 'n'  A8
  {1,1,1,1,0,1,1,0},     // diplay number 'o'  E8
  {1,1,1,1,0,1,1,0},     // diplay number 'P'  3B
  {1,1,1,1,0,1,1,0},     // diplay number 'r'  28
  {1,1,1,1,0,1,1,0},     // diplay number 't'  78
  {1,1,1,1,0,1,1,0},     // diplay number 'U'  F1
  {1,1,1,1,0,1,1,0},     // diplay number 'u'  E0
  {1,1,1,1,0,1,1,0},     // diplay number 'V'  51
  {1,1,1,1,0,1,1,0},     // diplay number 'W'  E2
  {1,1,1,1,0,1,1,0},     // diplay number 'Y'  D9
  {1,1,1,1,0,1,1,0},     // diplay number 'S'  3A

  {0,0,0,0,1,0,0,0},     // display symbol '-'  08
  {0,1,0,0,0,0,0,0},     // display symbol '_'  40
  {0,0,0,0,0,0,0,0},     // don't display  'x'  00
};

/* extern variables -----------------------------------------------------------------*/
extern __IO r_tmr tmr;

/* variables -----------------------------------------------------------------*/
__IO LED_SCAN1 Scan1Led;
__IO LED_SCAN2 Scan2Led;
__IO LED_SCAN3 Scan3Led;
__IO ICON_SCAN4 Scan4Icon;
__IO ICON_SCAN5 Scan5Icon;

/* static private function protocol -----------------------------------------------*/
static void set_led_segments(uint8_t segments);
static void display_digit(uint8_t digit, uint8_t position);

/* static Private Function definitions ------------------------------------------------------*/
static void main_M1(int8_t num)
{
  //M1為個位數, 但是dp被當作負號使用
  uint8_t i, digi, minus_flag;
  
  minus_flag = (num < 0)? true:false;
  num = (num > 99)? (num-=100):num;
  digi = num % 10;

  //若負號成立則亮燈
  Scan3Led.scan3.M1_minus = minus_flag? _char_LookupTable[dig_minus][4]:0;

  for(i=0; i<7;i++)
  {
    switch (i)
    {
      case 0:
        Scan3Led.scan3.M1_a = _7seg_LookupTable[digi][i];
      break;
      case 1:
        Scan3Led.scan3.M1_b = _7seg_LookupTable[digi][i];
      break;
      case 2:
        Scan3Led.scan3.M1_c = _7seg_LookupTable[digi][i];
      break;
      case 3:
        Scan3Led.scan3.M1_d = _7seg_LookupTable[digi][i];
      break;
      case 4:
        Scan3Led.scan3.M1_e = _7seg_LookupTable[digi][i];
      break;
      case 5:
        Scan3Led.scan3.M1_f = _7seg_LookupTable[digi][i];
      break;
      case 6:
        Scan3Led.scan3.M1_g = _7seg_LookupTable[digi][i];
      break;
    }
  }
}

static void main_M2(uint8_t num)
{
  uint8_t i, digi, value=num;
  value = (num > 99)? (value-=100):num;
  digi = value / 10;
  digi = (num<100 && digi==0)? (digi=10):digi;


  for(i=0; i<7;i++)
  {
    switch (i)
    {
      case 0:
        Scan2Led.scan2.M2_a = _7seg_LookupTable[digi][i];
      break;
      case 1:
        Scan2Led.scan2.M2_b = _7seg_LookupTable[digi][i];
      break;
      case 2:
        Scan2Led.scan2.M2_c = _7seg_LookupTable[digi][i];
      break;
      case 3:
        Scan2Led.scan2.M2_d = _7seg_LookupTable[digi][i];
      break;
      case 4:
        Scan2Led.scan2.M2_e = _7seg_LookupTable[digi][i];
      break;
      case 5:
        Scan2Led.scan2.M2_f = _7seg_LookupTable[digi][i];
      break;
      case 6:
        Scan2Led.scan2.M2_g = _7seg_LookupTable[digi][i];
      break;
    }
  }
}

static void main_M3(uint8_t num)
{
  uint8_t i, digi, value=num;
  value = (num > 99)? (value-=100):num;
  digi = value / 10;
  digi = (num<100 && digi==0)? (digi=10):digi;


  for(i=0; i<7;i++)
  {
    switch (i)
    {
      case 0:
        Scan1Led.scan1.M3_a = _7seg_LookupTable[digi][i];
      break;
      case 1:
        Scan1Led.scan1.M3_b = _7seg_LookupTable[digi][i];
      break;
      case 2:
        Scan1Led.scan1.M3_c = _7seg_LookupTable[digi][i];
      break;
      case 3:
        Scan1Led.scan1.M3_d = _7seg_LookupTable[digi][i];
      break;
      case 4:
        Scan1Led.scan1.M3_e = _7seg_LookupTable[digi][i];
      break;
      case 5:
        Scan1Led.scan1.M3_f = _7seg_LookupTable[digi][i];
      break;
      case 6:
        Scan1Led.scan1.M3_g = _7seg_LookupTable[digi][i];
      break;
    }
  }
}

/* Function definitions ------------------------------------------------------*/
void TempNumber(int16_t temp)
{
  main_M1(temp);
  main_M2(temp);
  main_M3(temp);
}




/*
  顯示邏輯說明及測試步驟, **需確認595推0的時候為Enable還是推1為Enable**, (硬體設計邏輯)
  以後如果還有595重新開發, 建議使用spi串口, 硬體執行效率更佳
  1. -8.8.8, 共3個7段顯示器使用3個scan, 11個icon使用2個scan, 共使用5組scan
  2. a~dp共接了8隻腳
  3. 設定map值(應用層), 轉換成對應腳位更換buffer
  4. 顯示掃描每1ms持續進行

//以下測試以硬體設計1為Enable執行
  測試1: 點亮Index=0(最右的8)
  測試2: 顯示3個6
  測試3: 以正規模式顯示數字(1234等等)
  測試4: 以正規模式顯示字節
*/
