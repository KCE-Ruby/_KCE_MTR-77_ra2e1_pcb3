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
#include "INC/framework/datapool.h"
#include "INC/framework/LED_Driver/app_menu_ctrl.h"
#include "INC/framework/LED_Driver/Indicator_encode.h"

/* Private defines ----------------------------------------------------------*/
#define DISPLAY_MAX_DIGITS 999    //此顯示器最大上限值為三位數

/* Private macro ----------------------------------------------------------*/
static const uint8_t _7seg_LookupTable[][7] = {
  //a,b,c,d,e,f,g,dp
  {1,1,1,1,1,1,0},     // diplay number '0'
  {0,1,1,0,0,0,0},     // diplay number '1'
  {1,1,0,1,1,0,1},     // diplay number '2'
  {1,1,1,1,0,0,1},     // diplay number '3'
  {0,1,1,0,0,1,1},     // diplay number '4'
  {1,0,1,1,0,1,1},     // diplay number '5'
  {0,0,1,1,1,1,1},     // diplay number '6'
  {1,1,1,0,0,1,0},     // diplay number '7'
  {1,1,1,1,1,1,1},     // diplay number '8'
  {1,1,1,1,0,1,1},     // diplay number '9'
  {0,0,0,0,0,0,0},     // don't display  'x'
  {0,0,0,0,0,1,0},     // display symbol '-'
  {1,1,1,1,1,1,1},     // diplay all '8.'  FF
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
  {dig_off,   dig_off,    dig_off},    // xxx
  {dig_off,   dig_off,    dig_off},    // Set
  //7.1 調節參數
  {dig_H,       dig_y,    dig_off},    // Hy 
  {dig_L,       dig_S,    dig_off},    // LS 
  {dig_U,       dig_S,    dig_off},    // US 
  {dig_o,       dig_t,    dig_off},    // ot 
  {dig_P,       dig_2,      dig_P},    // P2P
  {dig_o,       dig_E,    dig_off},    // oE 
  {dig_P,       dig_3,      dig_P},    // P3P
  {dig_o,       dig_3,    dig_off},    // o3 
  {dig_P,       dig_4,      dig_P},    // P4P
  {dig_o,       dig_4,    dig_off},    // o4 
  {dig_o,       dig_d,      dig_S},    // odS
  {dig_A,       dig_C,    dig_off},    // AC 
  {dig_r,       dig_t,      dig_r},    // rtr
  {dig_C,       dig_C,      dig_t},    // CCt
  {dig_C,       dig_C,      dig_S},    // CCS
  {dig_C,       dig_o,      dig_n},    // Con
  {dig_C,       dig_o,      dig_F},    // CoF
  //7.2 顯示參數
  {dig_C,       dig_F,    dig_off},    // CF 
  {dig_r,       dig_E,      dig_S},    // rES
  {dig_L,       dig_o,      dig_d},    // Lod
  {dig_r,       dig_E,      dig_d},    // rEd   --TODO:影片中沒看到
  {dig_d,       dig_L,      dig_y},    // dLy
  {dig_d,       dig_t,      dig_r},    // dtr
  //7.3 融霜參數
  {dig_t,       dig_d,      dig_F},    // tdF   --TODO:影片中與dFP換順序
  {dig_d,       dig_F,      dig_P},    // dFP   --TODO:影片中與tdF換順序
  {dig_d,       dig_t,      dig_E},    // dtE
  {dig_i,       dig_d,      dig_F},    // idF
  // {dig_d,       dig_i,      dig_d},    // 5 did   --TODO:影片中沒看到
  // {dig_n,       dig_P,      dig_S},    // 6 nPS   --TODO:影片中沒看到
  // {dig_O,       dig_d,      dig_C},    // 7 OdC   --TODO:影片中沒看到
  // {dig_r,       dig_r,      dig_d},    // 8 rrd   --TODO:影片中沒看到
  // {dig_H,       dig_E,      dig_S},    // 9 HES   --TODO:影片中沒看到
  {dig_M,       dig_d,      dig_F},    // MdF
  {dig_d,       dig_S,      dig_d},    // dSd
  {dig_d,       dig_F,      dig_d},    // dFd
  {dig_d,       dig_A,      dig_d},    // dAd
  {dig_F,       dig_d,      dig_t},    // Fdt
  {dig_d,       dig_P,      dig_o},    // dPo
  {dig_d,       dig_A,      dig_F},    // dAF
  //7.4 蒸發器風扇參數
  {dig_F,       dig_n,      dig_C},    // FnC
  {dig_F,       dig_n,      dig_d},    // Fnd
  {dig_F,       dig_C,      dig_t},    // FCt
  {dig_F,       dig_S,      dig_t},    // FSt
  {dig_F,       dig_o,      dig_n},    // Fon
  {dig_F,       dig_O,      dig_F},    // FOF
  {dig_F,       dig_A,      dig_P},    // FAP
  //7.5 報警參數
  {dig_A,       dig_L,      dig_C},    // ALC
  {dig_A,       dig_L,      dig_U},    // ALU
  {dig_A,       dig_L,      dig_L},    // ALL
  {dig_A,       dig_F,      dig_H},    // AFH
  {dig_A,       dig_L,      dig_d},    // ALd
  {dig_d,       dig_A,      dig_o},    // dAo
  //7.6 冷凝溫度報警
  {dig_A,       dig_P,      dig_2},    // AP2
  {dig_A,       dig_L,      dig_2},    // AL2
  {dig_A,       dig_U,      dig_2},    // AU2
  {dig_A,       dig_H,      dig_2},    // AH2
  {dig_A,       dig_d,      dig_2},    // Ad2
  {dig_d,       dig_A,      dig_2},    // dA2
  {dig_b,       dig_L,      dig_L},    // bLL
  {dig_A,       dig_C,      dig_2},    // AC2
  //7.7 數字輸入參數
  {dig_i,       dig_1,      dig_P},    // i1P
  {dig_i,       dig_1,      dig_F},    // i1F
  {dig_d,       dig_i,      dig_d},    // did
  {dig_n,       dig_P,      dig_S},    // nPS
  {dig_O,       dig_d,      dig_C},    // OdC
  {dig_r,       dig_r,      dig_d},    // rrd
  {dig_H,       dig_E,      dig_S},    // HES
  //7.8 其他參數
  {dig_A,       dig_d,      dig_r},    // Adr
  {dig_P,       dig_b,      dig_C},    // PbC
  {dig_O,       dig_n,      dig_F},    // OnF
  {dig_d,       dig_P,      dig_1},    // dp1
  {dig_d,       dig_P,      dig_2},    // dp2
  {dig_d,       dig_P,      dig_3},    // dp3
  {dig_d,       dig_P,      dig_4},    // dp4
  {dig_r,       dig_S,      dig_E},    // rSE
  {dig_r,       dig_E,      dig_L},    // rEL
  {dig_P,       dig_t,      dig_b},    // Ptb
  {dig_P,       dig_r,      dig_2},    // Pr2
};
/* extern variables -----------------------------------------------------------------*/
extern __IO r_tmr tmr;
extern __IO s_Var System;
extern __IO s_Flag sFlag;
extern __IO ByteSettingTable bytetable[End];
extern __IO uint8_t bytetable_pr2[End];

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
static void main_M2(uint16_t num, bool dot);
static void main_M3(uint16_t num, bool dot);
static void char_M1(uint8_t _char);
static void char_M2(uint8_t _char);
static void char_M3(uint8_t _char);

/* static Private Function definitions ------------------------------------------------------*/
static void main_M1(uint16_t num, bool flag)
{
  //M1為最右邊的數字, 但是dp被當作負號使用
  uint8_t i, digi;
  if(num == CLEARALL)
  {
    digi = notshow; //不顯示數字
    flag = false;   //不顯示負號
  }
  else
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

static void main_M2(uint16_t num, bool dot)
{
  //M2為中間的數字
  uint8_t i, digi;
  if(num == CLEARALL)
    digi = notshow;
  else
  {
    digi = (uint8_t)((num / 10) % 10); //只取十位數
    //若不顯示小數點, 且num小於10.0, 中間值為0, 則需判斷是否要顯示0或不顯示
    if(dot==false)
      digi = (num<100 && digi==0)? (uint8_t)notshow:digi;
  }

  for(i=0; i<7;i++)
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
    }
  }

  if((num==CLEARALL) || (dot==false))
  {
    //若在設定層或離開時需閃爍則不顯示小數點
    Scan2temp.scan2.M2_dp = false;
  }
  else
  {
    //若數字為2位數則亮燈 
    Scan2temp.scan2.M2_dp = (System.decimalIndex == DECIMAL_AT_1)? true:false;
  }
}

static void main_M3(uint16_t num, bool dot)
{
  //M3為最左邊的數字
  uint8_t i, digi;
  if(num == CLEARALL)
    digi = notshow;
  else
  {
    digi = (uint8_t)((num / 100) % 10); //只取百位數
    //若為0, 則需判斷是否要顯示0
    digi = (num<100 && digi==0)? (uint8_t)10 : digi;
  }

  for(i=0; i<7;i++)
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
    }
  }
  
  if((num==CLEARALL) || (dot==false))
  {
    //若在設定層或離開時需閃爍則不顯示小數點
    Scan3temp.scan3.M3_dp = false;
  }
  else
  {
    //若數字為1位數則亮燈
    Scan3temp.scan3.M3_dp = (System.decimalIndex == DECIMAL_AT_2)? true:false;
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
  Scan1temp.scan1.M1_minus = 0; //字節的負號恆不亮
}

static void char_M2(uint8_t _char)
{
  uint8_t i;
  int8_t pr2_index;
  for(i=0; i<7;i++)
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
    }
  }
  if((System.mode==level1Mode) || (System.mode==historyMode))
  {
    //若在設定/歷史值層或離開時需閃爍則不顯示小數點
    Scan2temp.scan2.M2_dp = false;
  }
  else if(System.mode == level2Mode)
  {
    //TODO:要在第一層也有的才會有小數點
    pr2_index = bytetable_pr2[System.level2_index];
    if(bytetable[pr2_index].Mode == Pr1)
      Scan2temp.scan2.M2_dp = true;
    else
      Scan2temp.scan2.M2_dp = false;
  }
}

static void char_M3(uint8_t _char)
{
  uint8_t i;
  for(i=0; i<7;i++)
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
    }
  }

  if(System.mode == level1Mode)
  {
    //若在設定層或離開時需閃爍則不顯示小數點
    Scan3temp.scan3.M3_dp = false;
  }
  else if(System.mode == level2Mode)
  {
    //要在第一層有的才會有點
    Scan3temp.scan3.M3_dp = (System.decimalIndex == DECIMAL_AT_2)? true:false;
  }
}

/* Function definitions ------------------------------------------------------*/
void NumToDisplay(int16_t temp)
{
  //收進來的值無小數點, 已放大10倍計算, ex. (12.5)->(125), (-55.9)->(-559)
  bool minus_flag=false, dot=true;
  minus_flag = (temp < 0)? true:false;
  temp = (temp < 0) ? (int16_t)(-temp) : temp;

  //轉換的數值不能超過最大值, 且恆為正數
  if(temp!=CLEARALL)
  {
    //某些參數要已整數型態顯示其數值
    if(sFlag.Vvalue_int==true)
      dot = false;

    //若為三位數則不顯示小數點
    if(temp > DISPLAY_MAX_DIGITS)
    {
      temp = temp/10;
      dot = false;
    }
  }

  main_M1((uint16_t)temp, minus_flag);
  main_M2((uint16_t)temp, dot);
  main_M3((uint16_t)temp, dot);
}

void CharToDisplay(uint8_t _char)
{
  char_M1(ByteTable[_char][2]);
  char_M2(ByteTable[_char][1]);
  char_M3(ByteTable[_char][0]);
}

void HiToDisplay(void)
{
  char_M1(dig_i);
  char_M2(dig_H);
  char_M3(dig_off);
}

void LoToDisplay(void)
{
  char_M1(dig_o);
  char_M2(dig_L);
  char_M3(dig_off);
}

void rStToDisplay(void)
{
  char_M1(dig_t);
  char_M2(dig_S);
  char_M3(dig_r);
}

void nyToDisplay(bool i)
{
  if(i==1)
  {
    char_M1(dig_y);
    char_M2(dig_off);
    char_M3(dig_off);
  }
  else
  {
    char_M1(dig_n);
    char_M2(dig_off);
    char_M3(dig_off);
  }
}