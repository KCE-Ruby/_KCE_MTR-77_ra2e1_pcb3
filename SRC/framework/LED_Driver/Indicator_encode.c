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
#define DISPLAY_MAX_DIGITS 999    //此顯示器最大上限值為三位數

/* Private macro ----------------------------------------------------------*/
static const uint8_t _7seg_LookupTable[][8] = {
  //a,b,c,d,e,f,g,dp
  {1,1,1,1,1,1,0,0},     // diplay number '0'
  {0,1,1,0,0,0,0,0},     // diplay number '1'
  {1,1,0,1,1,0,1,0},     // diplay number '2'
  {1,1,1,1,0,0,1,0},     // diplay number '3'
  {0,1,1,0,0,1,1,0},     // diplay number '4'
  {1,0,1,1,0,1,1,0},     // diplay number '5'
  {0,0,1,1,1,1,1,0},     // diplay number '6'
  {1,1,1,0,0,1,0,0},     // diplay number '7'
  {1,1,1,1,1,1,1,0},     // diplay number '8'
  {1,1,1,1,0,1,1,0},     // diplay number '9'
  {0,0,0,0,0,0,0,0},     // don't display  'x'
  {0,0,0,0,0,1,0,0},     // display symbol '-'
  {1,1,1,1,1,1,1,1},     // diplay all '8.'  FF
};

static const uint8_t _char_LookupTable[][8] = {
  {1,1,1,1,1,1,0,0},     // diplay number '0'
  {0,1,1,0,0,0,0,0},     // diplay number '1'
  {1,1,0,1,1,0,1,0},     // diplay number '2'
  {1,1,1,1,0,0,1,0},     // diplay number '3'
  {0,1,1,0,0,1,1,0},     // diplay number '4'
  {1,0,1,1,0,1,1,0},     // diplay number '5'
  {0,0,1,1,1,1,1,0},     // diplay number '6'
  {1,1,1,0,0,1,0,0},     // diplay number '7'
  {1,1,1,1,1,1,1,0},     // diplay number '8'
  {1,1,1,1,0,1,1,0},     // diplay number '9'

  {1,1,1,0,1,1,1,0},     // diplay number 'A'
  {0,0,1,1,1,1,1,0},     // diplay number 'b'
  {1,0,0,1,1,1,0,0},     // diplay number 'C'
  {0,0,0,1,1,0,1,0},     // diplay number 'c'
  {0,1,1,1,1,0,1,0},     // diplay number 'd'
  {1,0,0,1,1,1,1,0},     // diplay number 'E'
  {1,0,0,0,1,1,1,0},     // diplay number 'F'
  {1,0,1,1,1,1,1,0},     // diplay number 'G'
  {0,1,1,0,1,1,1,0},     // diplay number 'H'
  {0,1,1,0,0,0,0,0},     // diplay number 'I'
  {0,0,1,0,0,0,0,0},     // diplay number 'i'
  {0,0,0,1,1,1,0,0},     // diplay number 'L'
  {1,1,1,0,1,1,0,0},     // diplay number 'M'
  {0,0,1,0,1,0,1,0},     // diplay number 'n'
  {1,1,1,1,1,1,0,0},     // diplay number 'O'
  {0,0,1,1,1,0,1,0},     // diplay number 'o'
  {1,1,0,0,1,1,1,0},     // diplay number 'P'
  {0,0,0,0,1,0,1,0},     // diplay number 'r'
  {1,0,1,1,0,1,1,0},     // diplay number 'S'
  {0,0,0,1,1,1,1,0},     // diplay number 't'
  {0,1,1,1,1,1,0,0},     // diplay number 'U'
  {0,0,1,1,1,0,0,0},     // diplay number 'u'
  {0,1,1,1,0,1,1,0},     // diplay number 'Y'
  
  {0,0,0,0,0,0,0,0},     // don't display  'x'
  {1,1,1,1,1,1,1,1},     // all   display  '8.'
  {0,0,0,0,0,0,1,0},     // display symbol '-'
  {0,0,0,1,0,0,0,0},     // display symbol '_'
};

static const uint8_t ByteTable[][3] = {
  {dig_off,   dig_off,    dig_off},    // 0 xxx
  //7.1 調節參數
  {dig_H,       dig_y,    dig_off},    // 1 Hy 
  {dig_L,       dig_S,    dig_off},    // 2 LS 
  {dig_U,       dig_S,    dig_off},    // 3 US 
  {dig_o,       dig_t,    dig_off},    // 4 ot 
  {dig_P,       dig_2,      dig_P},    // 5 P2P
  {dig_o,       dig_E,    dig_off},    // 6 oE 
  {dig_P,       dig_3,      dig_P},    // 7 P3P
  {dig_o,       dig_3,    dig_off},    // 8 o3 
  {dig_P,       dig_4,      dig_P},    // 9 P4P
  {dig_o,       dig_4,    dig_off},    //10 o4 
  {dig_o,       dig_d,      dig_S},    //11 odS
  {dig_A,       dig_C,    dig_off},    //12 AC 
  {dig_r,       dig_t,      dig_r},    //13 rtr
  {dig_C,       dig_C,      dig_t},    //14 CCt
  {dig_C,       dig_C,      dig_S},    //15 CCS
  {dig_C,       dig_o,      dig_n},    //16 Con
  {dig_C,       dig_o,      dig_F},    //17 CoF
  //7.2 顯示參數
  {dig_C,       dig_F,    dig_off},    // 1 CF 
  {dig_r,       dig_E,      dig_S},    // 2 rES
  {dig_L,       dig_o,      dig_d},    // 3 Lod
  {dig_r,       dig_E,      dig_d},    // 4 rEd   --TODO:影片中沒看到
  {dig_d,       dig_L,      dig_y},    // 5 dLy
  {dig_d,       dig_t,      dig_r},    // 6 dtr
  //7.3 融霜參數
  {dig_d,       dig_F,      dig_P},    // 1 dFP   --TODO:影片中與tdF換順序
  {dig_t,       dig_d,      dig_F},    // 2 tdF   --TODO:影片中與dFP換順序
  {dig_d,       dig_t,      dig_E},    // 3 dtE
  {dig_i,       dig_d,      dig_F},    // 4 idF
  // {dig_d,       dig_i,      dig_d},    // 5 did   --TODO:影片中沒看到
  // {dig_n,       dig_P,      dig_S},    // 6 nPS   --TODO:影片中沒看到
  // {dig_O,       dig_d,      dig_C},    // 7 OdC   --TODO:影片中沒看到
  // {dig_r,       dig_r,      dig_d},    // 8 rrd   --TODO:影片中沒看到
  // {dig_H,       dig_E,      dig_S},    // 9 HES   --TODO:影片中沒看到
  {dig_M,       dig_d,      dig_F},    //10 MdF
  {dig_d,       dig_S,      dig_d},    //11 dSd
  {dig_d,       dig_F,      dig_d},    //12 dFd
  {dig_d,       dig_A,      dig_d},    //13 dAd
  {dig_F,       dig_d,      dig_t},    //14 Fdt
  {dig_d,       dig_P,      dig_o},    //15 dPo
  {dig_d,       dig_A,      dig_F},    //16 dAF
  //7.4 蒸發器風扇參數
  {dig_F,       dig_n,      dig_C},    // 1 FnC
  {dig_F,       dig_n,      dig_d},    // 2 Fnd
  {dig_F,       dig_C,      dig_t},    // 3 FCt
  {dig_F,       dig_S,      dig_t},    // 4 FSt
  {dig_F,       dig_o,      dig_n},    // 5 Fon
  {dig_F,       dig_O,      dig_F},    // 6 FOF
  {dig_F,       dig_A,      dig_P},    // 7 FAP
  //7.5 報警參數
  {dig_A,       dig_L,      dig_C},    // 1 ALC
  {dig_A,       dig_L,      dig_U},    // 2 ALU
  {dig_A,       dig_L,      dig_L},    // 3 ALL
  {dig_A,       dig_F,      dig_H},    // 4 AFH
  {dig_A,       dig_L,      dig_d},    // 5 ALd
  {dig_d,       dig_A,      dig_o},    // 6 dAo
  //7.6 冷凝溫度報警
  {dig_A,       dig_P,      dig_2},    // 1 AP2
  {dig_A,       dig_L,      dig_2},    // 2 AL2
  {dig_A,       dig_U,      dig_2},    // 3 AU2
  {dig_A,       dig_H,      dig_2},    // 4 AH2
  {dig_A,       dig_d,      dig_2},    // 5 Ad2
  {dig_d,       dig_A,      dig_2},    // 6 dA2
  {dig_b,       dig_L,      dig_L},    // 7 bLL
  {dig_A,       dig_C,      dig_2},    // 8 AC2
  //7.7 數字輸入參數
  {dig_i,       dig_1,      dig_P},    // 1 i1P
  {dig_i,       dig_1,      dig_F},    // 2 i1F
  {dig_d,       dig_i,      dig_d},    //63 did
  {dig_n,       dig_P,      dig_S},    //64 nPS
  {dig_O,       dig_d,      dig_C},    //65 OdC
  {dig_r,       dig_r,      dig_d},    //66 rrd
  {dig_H,       dig_E,      dig_S},    //67 HES
  //7.8 其他參數
  {dig_A,       dig_d,      dig_r},    // 1 Adr
  {dig_P,       dig_b,      dig_C},    // 2 PbC
  {dig_O,       dig_n,      dig_F},    // 3 OnF
  {dig_d,       dig_P,      dig_1},    // 4 dp1
  {dig_d,       dig_P,      dig_2},    // 5 dp2
  {dig_d,       dig_P,      dig_3},    // 6 dp3
  {dig_d,       dig_P,      dig_4},    // 7 dp4
  {dig_r,       dig_S,      dig_E},    // 8 rSE
  {dig_r,       dig_E,      dig_L},    // 9 rEL
  {dig_P,       dig_t,      dig_b},    //10 Ptb
};
/* extern variables -----------------------------------------------------------------*/
extern __IO r_tmr tmr;
extern __IO s_Var System;

/* variables -----------------------------------------------------------------*/
__IO LED_SCAN1 Scan1Led;
__IO LED_SCAN2 Scan2Led;
__IO LED_SCAN3 Scan3Led;
__IO ICON_SCAN4 Scan4Icon;
__IO ICON_SCAN5 Scan5Icon;

__IO LED_SCAN1 Scan1temp;
__IO LED_SCAN2 Scan2temp;
__IO LED_SCAN3 Scan3temp;
__IO ICON_SCAN4 Scan4temp;
__IO ICON_SCAN5 Scan5temp;

/* static private function protocol -----------------------------------------------*/
static void main_M1(uint16_t num, bool flag);
static void main_M2(uint16_t num);
static void main_M3(uint16_t num);
static void char_M1(uint8_t _char);
static void char_M2(uint8_t _char);
static void char_M3(uint8_t _char);

/* static Private Function definitions ------------------------------------------------------*/
static void main_M1(uint16_t num, bool flag)
{
  //M1為最右邊的數字, 但是dp被當作負號使用
  uint8_t i, digi;
  digi = (uint8_t)(num % 10); //只取個位數

  for(i=0; i<7;i++)
  {
    switch (i)
    {
      case 0:
        Scan1temp.scan1.M1_a = _7seg_LookupTable[digi][i];
      break;
      case 1:
        Scan1temp.scan1.M1_b = _7seg_LookupTable[digi][i];
      break;
      case 2:
        Scan1temp.scan1.M1_c = _7seg_LookupTable[digi][i];
      break;
      case 3:
        Scan1temp.scan1.M1_d = _7seg_LookupTable[digi][i];
      break;
      case 4:
        Scan1temp.scan1.M1_e = _7seg_LookupTable[digi][i];
      break;
      case 5:
        Scan1temp.scan1.M1_f = _7seg_LookupTable[digi][i];
      break;
      case 6:
        Scan1temp.scan1.M1_g = _7seg_LookupTable[digi][i];
      break;
    }
  }
  // //若負號成立則亮燈
  Scan1temp.scan1.M1_minus = flag? true:false;
}

static void main_M2(uint16_t num)
{
  //M2為中間的數字
  uint8_t i, digi;
  digi = (uint8_t)((num / 10) % 10); //只取十位數

  //若為0, 則需判斷是否要顯示0或不顯示
  digi = (num<100 && digi==0)? (uint8_t)10:digi;

  //若數字為2位數則亮燈
  Scan2temp.scan2.M2_dp = (System.decimalIndex == DECIMAL_AT_1)? 1 : 0;

  for(i=0; i<8;i++)
  {
    switch (i)
    {
      case 0:
        Scan2temp.scan2.M2_a = _7seg_LookupTable[digi][i];
      break;
      case 1:
        Scan2temp.scan2.M2_b = _7seg_LookupTable[digi][i];
      break;
      case 2:
        Scan2temp.scan2.M2_c = _7seg_LookupTable[digi][i];
      break;
      case 3:
        Scan2temp.scan2.M2_d = _7seg_LookupTable[digi][i];
      break;
      case 4:
        Scan2temp.scan2.M2_e = _7seg_LookupTable[digi][i];
      break;
      case 5:
        Scan2temp.scan2.M2_f = _7seg_LookupTable[digi][i];
      break;
      case 6:
        Scan2temp.scan2.M2_g = _7seg_LookupTable[digi][i];
      break;
      case 7:
        Scan2temp.scan2.M2_dp = _7seg_LookupTable[digi][i];
      break;
    }
  }
}

static void main_M3(uint16_t num)
{
  //M3為最左邊的數字
  uint8_t i, digi;
  digi = (uint8_t)((num / 100) % 10); //只取百位數

  //若為0, 則需判斷是否要顯示0
  digi = (num<100 && digi==0)? (uint8_t)10 : digi;

  for(i=0; i<8;i++)
  {
    switch (i)
    {
      case 0:
        Scan3temp.scan3.M3_a = _7seg_LookupTable[digi][i];
      break;
      case 1:
        Scan3temp.scan3.M3_b = _7seg_LookupTable[digi][i];
      break;
      case 2:
        Scan3temp.scan3.M3_c = _7seg_LookupTable[digi][i];
      break;
      case 3:
        Scan3temp.scan3.M3_d = _7seg_LookupTable[digi][i];
      break;
      case 4:
        Scan3temp.scan3.M3_e = _7seg_LookupTable[digi][i];
      break;
      case 5:
        Scan3temp.scan3.M3_f = _7seg_LookupTable[digi][i];
      break;
      case 6:
        Scan3temp.scan3.M3_g = _7seg_LookupTable[digi][i];
      break;
      case 7:
        Scan3temp.scan3.M3_dp = _7seg_LookupTable[digi][i];
      break;
    }
  }
}

static void char_M1(uint8_t _char)
{
  uint8_t i;
  for(i=0; i<7;i++)
  {
    switch (i)
    {
      case 0:
        Scan1temp.scan1.M1_a = _char_LookupTable[_char][i];
      break;
      case 1:
        Scan1temp.scan1.M1_b = _char_LookupTable[_char][i];
      break;
      case 2:
        Scan1temp.scan1.M1_c = _char_LookupTable[_char][i];
      break;
      case 3:
        Scan1temp.scan1.M1_d = _char_LookupTable[_char][i];
      break;
      case 4:
        Scan1temp.scan1.M1_e = _char_LookupTable[_char][i];
      break;
      case 5:
        Scan1temp.scan1.M1_f = _char_LookupTable[_char][i];
      break;
      case 6:
        Scan1temp.scan1.M1_g = _char_LookupTable[_char][i];
      break;
    }
  }
}

static void char_M2(uint8_t _char)
{
  uint8_t i;
  for(i=0; i<8;i++)
  {
    switch (i)
    {
      case 0:
        Scan2temp.scan2.M2_a = _char_LookupTable[_char][i];
      break;
      case 1:
        Scan2temp.scan2.M2_b = _char_LookupTable[_char][i];
      break;
      case 2:
        Scan2temp.scan2.M2_c = _char_LookupTable[_char][i];
      break;
      case 3:
        Scan2temp.scan2.M2_d = _char_LookupTable[_char][i];
      break;
      case 4:
        Scan2temp.scan2.M2_e = _char_LookupTable[_char][i];
      break;
      case 5:
        Scan2temp.scan2.M2_f = _char_LookupTable[_char][i];
      break;
      case 6:
        Scan2temp.scan2.M2_g = _char_LookupTable[_char][i];
      break;
      case 7:
        Scan2temp.scan2.M2_dp = _char_LookupTable[_char][i];
      break;
    }
  }
}

static void char_M3(uint8_t _char)
{
  uint8_t i;
  for(i=0; i<8;i++)
  {
    switch (i)
    {
      case 0:
        Scan3temp.scan3.M3_a = _char_LookupTable[_char][i];
      break;
      case 1:
        Scan3temp.scan3.M3_b = _char_LookupTable[_char][i];
      break;
      case 2:
        Scan3temp.scan3.M3_c = _char_LookupTable[_char][i];
      break;
      case 3:
        Scan3temp.scan3.M3_d = _char_LookupTable[_char][i];
      break;
      case 4:
        Scan3temp.scan3.M3_e = _char_LookupTable[_char][i];
      break;
      case 5:
        Scan3temp.scan3.M3_f = _char_LookupTable[_char][i];
      break;
      case 6:
        Scan3temp.scan3.M3_g = _char_LookupTable[_char][i];
      break;
      case 7:
        Scan3temp.scan3.M3_dp = _char_LookupTable[_char][i];
      break;
    }
  }
}

/* Function definitions ------------------------------------------------------*/
void NumToDisplay(int16_t temp)
{
  //收進來的值無小數點, 已放大10倍計算, ex. (12.5)->(125), (-55.9)->(-559)
  static bool minus_flag;
  minus_flag = (temp < 0)? true:false;
  temp = (temp < 0) ? (int16_t)(-temp) : temp;

  //轉換的數值不能超過最大值, 且恆為正數
  temp = (temp > DISPLAY_MAX_DIGITS)? DISPLAY_MAX_DIGITS : temp;

  main_M1((uint16_t)temp, minus_flag);
  main_M2((uint16_t)temp);
  main_M3((uint16_t)temp);
}

void CharToDisplay(uint8_t _char)
{
  char_M1(ByteTable[_char][2]);
  char_M2(ByteTable[_char][1]);
  char_M3(ByteTable[_char][0]);
}
