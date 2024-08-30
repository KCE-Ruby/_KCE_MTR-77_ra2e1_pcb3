/**
 ******************************************************************************
 * @file    datapool.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 * 程式碼的數據轉換區
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/datapool.h"
#include "INC/framework/LED_Driver/app_menu_ctrl.h"

/* extern variables -----------------------------------------------------------------*/
extern __IO s_Var System;

/* variables -----------------------------------------------------------------*/
static ByteSettingTable bytetable[End] = 
{
  //參數字元,  上限值,        下限值,        預設值,   權限層
  {Set,           -50,           110,         -5.0,     NaN},
  { Hy,           0.1,          25.5,          2.0,     Pr1},
  { LS,         -50.0,           110,         -5.0,     Pr2},
  { US,         -50.0,           110,          110,     Pr2},
  { Ot,           -12,            12,            0,     Pr1},
  {P2P,       exist_n,       exist_y,      exist_n,     Pr1}, //n=不存在; y=存在
  { OE,           -12,            12,            0,     Pr2},
  {P3P,       exist_n,       exist_y,      exist_n,     Pr2}, //n=不存在; y=存在
  { O3,           -12,            12,            0,     Pr2},
  {P4P,       exist_n,       exist_y,      exist_n,     Pr2}, //n=不存在; y=存在
  { O4,           -12,            12,            0,     Pr2},
  {OdS,             0,           255,            0,     Pr2},
  { AC,             0,            50,            1,     Pr1},
  {rtr,             0,           100,          100,     Pr2}, //P1=100, P2=0
  {CCt,           0.0,          24.0,            0,     Pr2}, //精度為0.1hour = 6min
  {CCS,         -55.0,         150.0,           -5,     Pr2},
  {COn,             0,           255,           15,     Pr2},
  {COF,             0,           255,           30,     Pr2},
  { CF,      degree_C,      degree_F,     degree_C,     Pr2}, //攝氏=C, 華氏=F
  {rES,  DECIMAL_AT_1,  DECIMAL_AT_0, DECIMAL_AT_1,     Pr1}, //小數=dE=DECIMAL_AT_1, 整數=in=DECIMAL_AT_0
  {Lod,       disp_P1,      disp_dtr,      disp_P1,     Pr2},
  {rEd,       disp_P1,      disp_dtr,      disp_P1,     Pr2},
  {dLY,             0,          20.0,            0,     Pr2}, //單位:0~20.0分鐘, 分辨率10秒
  {dtr,             0,           100,           50,     Pr2}, //P1=100, P2=0
  {dFP,    defrost_np,    defrost_p4,   defrost_p2,     Pr2},
  {tdF,       type_EL,       type_in,      type_EL,     Pr1},
  {dtE,           -50,            50,            8,     Pr1},
  {IdF,             1,           120,            6,     Pr1},
  {MdF,             0,           255,           30,     Pr1}, //0的時候不融霜
  {dSd,             0,            99,            0,     Pr2},
  {dFd,    defrost_rt,   defrost_dEF,   defrost_it,     Pr2},
  {dAd,             0,           255,           30,     Pr2},
  {Fdt,             0,           120,            0,     Pr2},
  {dPo,       work_no,       work_yes,     work_no,     Pr2}, //n=不存在; y=存在
  {dAF,             0,          23.5,            0,     Pr2},
  {FnC,       fan_c_n,       fan_o_y,      fan_o_n,     Pr1},
  {Fnd,             0,           255,           10,     Pr1},
  {Fct,             0,            50,           10,     Pr2}, //0的時候無此功能
  {FSt,           -50,            50,            2,     Pr1},
  {Fon,             0,            15,            0,     Pr2},
  {FoF,             0,            15,            0,     Pr2},
  {FAP,    defrost_np,    defrost_p4,   defrost_p2,     Pr2},
  {ALC,      alarm_rE,      alarm_Ab,     alarm_Ab,     Pr2},
  {ALU,           -50,           110,          110,     Pr1},
  {ALL,           -50,           110,          -50,     Pr1},
  {AFH,           0.1,          25.5,            1,     Pr2},
  {ALd,             0,           255,           15,     Pr2},
  {dAO,           0.0,          23.5,          1.3,     Pr2},
  {AP2,    defrost_np,    defrost_p4,   defrost_p4,     Pr2},
  {AL2,           -55,           150,          -40,     Pr2},
  {Au2,           -55,           150,          110,     Pr2},
  {AH2,           0.1,          25.5,            5,     Pr2},
  {Ad2,             0,           255,           15,     Pr2},
  {dA2,             0,          23.5,          1.3,     Pr2}, //單位:小時, 分辨率10分鐘
  {bLL,       work_no,      work_yes,      work_no,     Pr2},
  {AC2,       work_no,      work_yes,      work_no,     Pr2},
  {i1P,      polar_CL,      polar_OP,     polar_CL,     Pr1},
  {i1F,     alarm_EAL,      alarm_ES,    alarm_dor,     Pr1},
  {did,             0,           255,           15,     Pr1},
  {nPS,             0,            15,           15,     Pr2},
  {odc,      state_no,     state_F_C,    state_F_C,     Pr2},
  {rrd,       work_no,      work_yes,     work_yes,     Pr2},
  {HES,           -30,            30,            0,     Pr2},
  {Adr,             1,           244,            1,     Pr2},
  {PbC,      type_Ptc,      type_ntc,     type_ntc,     Pr1},
  {onF,     button_nu,     button_ES,    button_ES,     Pr2},
  {dp1,             0,             0,            0,     Pr2},
  {dp2,             0,             0,            0,     Pr1},
  {dp3,             0,             0,            0,     Pr1},
  {dp4,             0,             0,            0,     Pr1},
  {rSE,             0,             0,            0,     Pr2},
  {rEL,             0,             0,            0,     Pr2},
  {Ptb,             0,             0,            0,     Pr2},
};


/* Private function protocol -----------------------------------------------*/

/* Function definitions ------------------------------------------------------*/