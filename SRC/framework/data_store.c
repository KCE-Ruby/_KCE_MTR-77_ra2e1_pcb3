/**
 ******************************************************************************
 * @file    data_store.c
 * @author  KCE R&D Team
 * @date    2024-11-29
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 * 程式碼的數據轉換區
 * 
 * 使用者設定陣列(原始) -> float User_original[End].defult/.rangelow/.rangehigh
 * 使用者設定陣列(放大10倍) -> int16_t User_int[End] (過渡成rst, 放全部的使用者設定的預設值且經過shift最小值)
 * 原廠設定陣列(eeprom用) -> User_reset[SPIAddr_End] (index對應eeprom的address)
 * eeprom讀出陣列(讀出為uint8_t) -> eep_read[SPIAddr_End] (uint8_t)
 * 系統運算數值陣列(已放大10倍, 有正負值, 可直接顯示於儀表) -> systable[End] (int16_t)
 * 
 * void original_to_reset(void), 將使用者設定值轉換成原廠設定值
 * void eepread_to_systable(void), 開機時將eeprom值讀出並整理成系統運算值(須加上最小值shift)
 * void systable_to_eeprom(void), 需要將系統值寫入eeprom內
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/eep_api.h"
#include "INC/framework/data_store.h"


/* Private defines ----------------------------------------------------------*/

/* extern variables -----------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/
__IO ByteSettingTable User_original[End] = 
{

  {xxx,             0,             1,           1,     NaN}, //對齊參數用的而已
  {Set,           -50,           110,        15.0,     NaN},
  //參數字元,  下限值,        上限值,        預設值,   權限層
  { Hy,           0.1,          25.5,          2.0,     Pr1},
  { LS,         -50.0,           110,        -50.0,     Pr2},
  { US,         -50.0,           110,        110.0,     Pr2},
  { Ot,           -12,            12,            0,     Pr2},
  {P2P,             0,           0.1,          0.1,     Pr2}, //n=不存在; y=存在
  { OE,           -12,            12,            0,     Pr2},
  {P3P,             0,           0.1,            0,     Pr2}, //n=不存在; y=存在
  { O3,           -12,            12,            0,     Pr2},
  {P4P,             0,           0.1,            0,     Pr2}, //n=不存在; y=存在
  { O4,           -12,            12,            0,     Pr2},
  {OdS,             0,           255,            1,     Pr1},
  { AC,             0,            50,            5,     Pr1},
  {rtr,             0,           100,          100,     Pr2}, //P1=100, P2=0
  {CCt,           0.0,          24.0,         0.1,     Pr2}, //精度為0.1hour = 6min
  {CCS,         -55.0,         150.0,         5.0,     Pr1},
  {COn,             0,           255,           2,     Pr1},
  {COF,             0,           255,           1,     Pr1},
  { CF,             0,           0.1,           0,     Pr1}, //攝氏=C, 華氏=F
  {rES,             0,           0.1,         0.1,     Pr2}, //小數=dE=DECIMAL_AT_1, 整數=in=DECIMAL_AT_0
  {Lod,       disp_P1,      disp_dtr,      disp_P1,     Pr2},
  {rEd,       disp_P1,      disp_dtr,      disp_P1,     NaN},
  {dLY,             0,          20.0,            0,     Pr2}, //單位:0~20.0分鐘, 分辨率10秒
  {dtr,             0,           100,           50,     Pr2}, //P1=100, P2=0
  {tdF,       type_EL,       type_in,      type_EL,     Pr1},
  {dFP,    defrost_np,    defrost_p4,   defrost_p2,     Pr2},
  {dtE,           -50,            50,            8,     Pr1},
  {IdF,             1,           120,            6,     Pr2},
  {MdF,             0,           255,           30,     Pr2}, //0的時候不融霜
  {dSd,             0,            99,            0,     Pr2},
  {dFd,    defrost_rt,   defrost_dEF,   defrost_it,     Pr2},
  {dAd,             0,           255,           30,     Pr2},
  {Fdt,             0,           120,            0,     Pr1},
  {dPo,       work_no,       work_yes,     work_no,     Pr2}, //n=不存在; y=存在
  {dAF,             0,          23.5,            0,     Pr2},
  {FnC,       fan_c_n,       fan_o_y,      fan_o_n,     Pr1},
  {Fnd,             0,           255,           10,     Pr1},
  {Fct,             0,            50,           10,     Pr2}, //0的時候無此功能
  {FSt,           -50,            50,            2,     Pr1},
  {Fon,             0,            15,            0,     Pr1},
  {FoF,             0,            15,            0,     Pr1},
  {FAP,    defrost_np,    defrost_p4,   defrost_p2,     Pr2},
  {ALC,      alarm_rE,      alarm_Ab,     alarm_Ab,     Pr2},
  {ALU,           -50,           110,          110,     Pr2},
  {ALL,           -50,           110,          -50,     Pr2},
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
  {i1P,      polar_CL,      polar_OP,     polar_CL,     Pr2},
  {i1F,     alarm_EAL,      alarm_ES,    alarm_dor,     Pr2},
  {did,             0,           255,           15,     Pr2}, 
  {nPS,             0,            15,           15,     Pr2},
  {odc,      state_no,     state_F_C,    state_F_C,     Pr2},
  {rrd,       work_no,      work_yes,     work_yes,     Pr2},
  {HES,           -30,            30,            0,     Pr2},
  {Adr,             1,           244,            1,     Pr2},
  {PbC,      type_Ptc,      type_ntc,     type_ntc,     Pr2},
  {onF,     button_nu,     button_ES,    button_ES,     Pr2},
  {dp1,             0,             0,            0,     Pr1},
  {dp2,             0,             0,            0,     Pr1},
  {dp3,             0,             0,            0,     Pr2},
  {dp4,             0,             0,            0,     Pr2},
  {rSE,             0,             0,            0,     Pr1},
  {rEL,             0,           999,           10,     Pr2},
  {Ptb,             0,             0,            0,     Pr2},
};

uint8_t User_reset[SPIAddr_End] = {};
int16_t sys_table[End] = {};

void original_to_reset(void)
{
  static uint8_t max_index = onF;
  int16_t User_int[max_index] = {};
  uint8_t i = Set;
  uint8_t addr=SPIAddr_Start;

  while(i<max_index)
  {
    //把原始的使用者數值(小數點)轉換成10倍並shift成正整數
    User_int[i] = (User_original[i].DefaultValue - User_original[i].Range_Low)*10;

    if((addr==SPIAddr_Start)||(addr==SPIAddr_P2P)||(addr==SPIAddr_P3P)||(addr==SPIAddr_P4P)||(addr==SPIAddr_CF)|| \
      (addr==SPIAddr_rES)||(addr==SPIAddr_Lod)||(addr==SPIAddr_rEd)||(addr==SPIAddr_tdF)|| \
      (addr==SPIAddr_dFP)||(addr==SPIAddr_dFd)||(addr==SPIAddr_dPo)||(addr==SPIAddr_FnC)|| \
      (addr==SPIAddr_Fnd)||(addr==SPIAddr_FAP)||(addr==SPIAddr_ALC)||(addr==SPIAddr_AP2)|| \
      (addr==SPIAddr_bLL)||(addr==SPIAddr_AC2)||(addr==SPIAddr_i1P)||(addr==SPIAddr_i1F)|| \
      (addr==SPIAddr_odc)||(addr==SPIAddr_rrd)||(addr==SPIAddr_PbC)||(addr==SPIAddr_onF)
      )
    {
      //長度為1個byte的值
      User_reset[addr] = User_int[i];
      addr++;
    }
    else
    {
      //長度為2個byte的值
      User_reset[addr+1] = User_int[i] >> 8;    //取高位元
      User_reset[addr] = User_int[i] & 0xff;    //取低位元
      addr+=2;
    }
    i++;

    printf("User_int[%d]: %d\r\n",i , User_int[i]);
  }
}

void eepread_to_systable(void)
{
  uint8_t eep_read[SPIAddr_End] = {};
  uint8_t i = Set;

  //讀出eeprom的數值
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  I2C_EE_BufferRead(eep_read, 0x00, SPIAddr_End);
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);

  //組合成int16_t的值並shift成User值
  while(i)
  {
    sys_table[i] = (eep_read[i] + User_original[i].Range_Low)*10;
  }

}

void systable_to_eeprom(void)
{

}


