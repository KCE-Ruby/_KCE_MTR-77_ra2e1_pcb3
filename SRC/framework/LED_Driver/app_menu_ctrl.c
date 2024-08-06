/**
 ******************************************************************************
 * @file    app_menu_ctrl.c
 * @author  KCE R&D Team
 * @date    2024-07-11
 * @author  Ruby
 * 面板顯示的應用層, 包含溫度以及設定層
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/LED_Driver/app_menu_ctrl.h"

/* Private defines ----------------------------------------------------------*/
void Check_Min_Pv(void);
void Check_Max_Pv(void);


/* extern variables -----------------------------------------------------------------*/



/* variables -----------------------------------------------------------------*/
// static const uint8_t LED_0F[] =    // show字節
// { 
//   //dp, g, f, e, d, c, b, a (7段顯示器bit值排序轉換16進制)

//   // 0     1     2     3     4     5     6     7     8     9   
//     0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
//   //A(10) b(11) C(12) d(13) E(14) F(15) 16off 17on  18dp
//     0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00, 0xFF, 0x80,
//   //G(19) H(20) K(21) L(22) n(23) o(24) P(25) r(26) t(27)
//     0x3D, 0x76, 0x38, 0x38, 0x54, 0x5C, 0x73, 0x50, 0x78,
//   //U(28) V(29) Y(30) -(31) i(32) W(33) J(34) _(35) c(36)
//     0x3E, 0x1C, 0x6E, 0x40, 0x30, 0x3E, 0x0E, 0x08, 0x58,
//   //d(37) u(38) M(39)
//     0x5E, 0x1C, 0x37
// };

// static const uint8_t ByteTable[][3] = 
// {
//   {dig_off,   dig_off,    dig_off},    // 0 xxx
//   //7.1 調節參數
//   {dig_H,     dig_y,      dig_off},    // 1 Hy 
//   {dig_L,     dig_S,      dig_off},    // 2 LS 
//   {dig_U,     dig_S,      dig_off},    // 3 US 
//   {dig_o,     dig_t,      dig_off},    // 4 ot 
//   {dig_P,       dig_2,      dig_P},    // 5 P2P
//   {dig_o,     dig_E,      dig_off},    // 6 oE 
//   {dig_P,       dig_3,      dig_P},    // 7 P3P
//   {dig_o,     dig_3,      dig_off},    // 8 o3 
//   {dig_P,       dig_4,      dig_P},    // 9 P4P
//   {dig_o,     dig_4,      dig_off},    //10 o4 
//   {dig_o,       dig_d,      dig_S},    //11 odS
//   {dig_A,     dig_C,      dig_off},    //12 AC 
//   {dig_r,       dig_t,      dig_r},    //13 rtr
//   {dig_C,       dig_C,      dig_t},    //14 CCt
//   {dig_C,       dig_C,      dig_S},    //15 CCS
//   {dig_C,       dig_o,      dig_n},    //16 Con
//   {dig_C,       dig_o,      dig_F},    //17 CoF
//   //7.2 顯示參數
//   {dig_C,     dig_F,      dig_off},    // 1 CF 
//   {dig_r,       dig_E,      dig_S},    // 2 rES
//   {dig_L,       dig_o,      dig_d},    // 3 Lod
//   {dig_r,       dig_E,      dig_d},    // 4 rEd   --TODO:影片中沒看到
//   {dig_d,       dig_L,      dig_y},    // 5 dLy
//   {dig_d,       dig_t,      dig_r},    // 6 dtr
//   //7.3 融霜參數
//   {dig_d,       dig_F,      dig_P},    // 1 dFP   --TODO:影片中與tdF換順序
//   {dig_t,       dig_d,      dig_F},    // 2 tdF   --TODO:影片中與dFP換順序
//   {dig_d,       dig_t,      dig_E},    // 3 dtE
//   {dig_i,       dig_d,      dig_F},    // 4 idF
//   // {dig_d,       dig_i,      dig_d},    // 5 did   --TODO:影片中沒看到
//   // {dig_n,       dig_P,      dig_S},    // 6 nPS   --TODO:影片中沒看到
//   // {dig_O,       dig_d,      dig_C},    // 7 OdC   --TODO:影片中沒看到
//   // {dig_r,       dig_r,      dig_d},    // 8 rrd   --TODO:影片中沒看到
//   // {dig_H,       dig_E,      dig_S},    // 9 HES   --TODO:影片中沒看到
//   {dig_M,       dig_d,      dig_F},    //10 MdF
//   {dig_d,       dig_S,      dig_d},    //11 dSd
//   {dig_d,       dig_F,      dig_d},    //12 dFd
//   {dig_d,       dig_A,      dig_d},    //13 dAd
//   {dig_F,       dig_d,      dig_t},    //14 Fdt
//   {dig_d,       dig_P,      dig_o},    //15 dPo
//   {dig_d,       dig_A,      dig_F},    //16 dAF
//   //7.4 蒸發器風扇參數
//   {dig_F,       dig_n,      dig_C},    // 1 FnC
//   {dig_F,       dig_n,      dig_d},    // 2 Fnd
//   {dig_F,       dig_C,      dig_t},    // 3 FCt
//   {dig_F,       dig_S,      dig_t},    // 4 FSt
//   {dig_F,       dig_o,      dig_n},    // 5 Fon
//   {dig_F,       dig_O,      dig_F},    // 6 FOF
//   {dig_F,       dig_A,      dig_P},    // 7 FAP
//   //7.5 報警參數
//   {dig_A,       dig_L,      dig_C},    // 1 ALC
//   {dig_A,       dig_L,      dig_U},    // 2 ALU
//   {dig_A,       dig_L,      dig_L},    // 3 ALL
//   {dig_A,       dig_F,      dig_H},    // 4 AFH
//   {dig_A,       dig_L,      dig_d},    // 5 ALd
//   {dig_d,       dig_A,      dig_o},    // 6 dAo
//   //7.6 冷凝溫度報警
//   {dig_A,       dig_P,      dig_2},    // 1 AP2
//   {dig_A,       dig_L,      dig_2},    // 2 AL2
//   {dig_A,       dig_U,      dig_2},    // 3 AU2
//   {dig_A,       dig_H,      dig_2},    // 4 AH2
//   {dig_A,       dig_d,      dig_2},    // 5 Ad2
//   {dig_d,       dig_A,      dig_2},    // 6 dA2
//   {dig_b,       dig_L,      dig_L},    // 7 bLL
//   {dig_A,       dig_C,      dig_2},    // 8 AC2
//   //7.7 數字輸入參數
//   {dig_i,       dig_1,      dig_P},    // 1 i1P
//   {dig_i,       dig_1,      dig_F},    // 2 i1F
//   {dig_d,       dig_i,      dig_d},    //63 did
//   {dig_n,       dig_P,      dig_S},    //64 nPS
//   {dig_O,       dig_d,      dig_C},    //65 OdC
//   {dig_r,       dig_r,      dig_d},    //66 rrd
//   {dig_H,       dig_E,      dig_S},    //67 HES
//   //7.8 其他參數
//   {dig_A,       dig_d,      dig_r},    // 1 Adr
//   {dig_P,       dig_b,      dig_C},    // 2 PbC
//   {dig_O,       dig_n,      dig_F},    // 3 OnF
//   {dig_d,       dig_P,      dig_1},    // 4 dp1
//   {dig_d,       dig_P,      dig_2},    // 5 dp2
//   {dig_d,       dig_P,      dig_3},    // 6 dp3
//   {dig_d,       dig_P,      dig_4},    // 7 dp4
//   {dig_r,       dig_S,      dig_E},    // 8 rSE
//   {dig_r,       dig_E,      dig_L},    // 9 rEL
//   {dig_P,       dig_t,      dig_b},    //10 Ptb
// };


/* static private function protocol -----------------------------------------------*/



/* static Private Function definitions ------------------------------------------------------*/



/* Function definitions ------------------------------------------------------*/
void Check_Min_Pv(void)
{

}

void Check_Max_Pv(void)
{

}

void Clear_onRecord_LimitPv(void)
{
  
}







