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
 * 不須放大的參數設定值, 在eeprom內就是用原始資料紀錄
 * 
 * void original_to_reset(void), 將使用者設定值轉換成原廠設定值
 * void eepread_to_systable(void), 開機時將eeprom值讀出並整理成系統運算值(須加上最小值shift)
 * void systable_to_eeprom(void), 需要將系統值寫入eeprom內
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/eep_api.h"
#include "INC/framework/datapool.h"
#include "INC/framework/data_store.h"


/* Private defines ----------------------------------------------------------*/

/* extern variables -----------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/
__IO ByteSettingTable User_original[End] = 
{
  {xxx,             0,             1,            1,     NaN,       in}, //對齊參數用的而已
  {Set,           -50,         110.0,         -5.0,     NaN,       dE},
  //參數字元,  下限值,        上限值,        預設值,   權限層      位數
  { Hy,           0.1,          25.5,          2.0,     Pr1,       dE},
  { LS,         -50.0,         110.0,        -50.0,     Pr2,       dE},
  { US,         -50.0,         110.0,        110.0,     Pr2,       dE},
  { Ot,         -12.0,          12.0,          0.0,     Pr2,       dE},
  {P2P,      No_exist,     Yes_exist,    Yes_exist,     Pr2,       in},
  { OE,         -12.0,          12.0,          0.0,     Pr2,       dE},
  {P3P,      No_exist,     Yes_exist,     No_exist,     Pr2,       in},
  { O3,         -12.0,          12.0,          0.0,     Pr2,       dE},
  {P4P,      No_exist,     Yes_exist,     No_exist,     Pr2,       in},
  { O4,         -12.0,          12.0,          0.0,     Pr2,       dE},
  {OdS,             0,           255,            0,     Pr1,       in},
  { AC,             0,            50,            1,     Pr1,       in},
  {rtr,             0,           100,          100,     Pr2,       in}, //P1=100, P2=0
  {CCt,           0.0,          24.0,          0.0,     Pr2,       dE}, //精度為0.1hour = 6min
  {CCS,         -55.0,         150.0,         -5.0,     Pr1,       dE},
  {COn,             0,           255,           15,     Pr1,       in},
  {COF,             0,           255,           30,     Pr1,       in},
  { CF,      degree_C,      degree_F,     degree_C,     Pr1,       in}, //攝氏=C, 華氏=F
  {rES,            dE,            in,           dE,     Pr2,       in}, //小數=dE=DECIMAL_AT_1, 整數=in=DECIMAL_AT_0
  {Lod,       disp_P1,      disp_dtr,      disp_P1,     Pr2,       in},
  {rEd,       disp_P1,      disp_dtr,      disp_P1,     NaN,       in},
  {dLY,             0,          20.0,          0.0,     Pr2,       dE}, //單位:0~20.0分鐘, 分辨率10秒
  {dtr,             0,           100,           50,     Pr2,       in}, //P1=100, P2=0
  {tdF,       type_EL,       type_in,      type_EL,     Pr1,       in},
  {dFP,    defrost_np,    defrost_p4,   defrost_p2,     Pr2,       in},
  {dtE,         -50.0,          50.0,          8.0,     Pr1,       dE},
  {IdF,             1,           120,            6,     Pr2,       in},
  {MdF,             0,           255,           30,     Pr2,       in}, //0的時候不融霜
  {dSd,             0,            99,            0,     Pr2,       in},
  {dFd,    defrost_rt,   defrost_dEF,   defrost_it,     Pr2,       in},
  {dAd,             0,           255,           30,     Pr2,       in},
  {Fdt,             0,           120,            0,     Pr1,       in},
  {dPo,       work_no,       work_yes,     work_no,     Pr2,       in}, //n=不存在
  {dAF,             0,          23.5,          0.0,     Pr2,       dE},
  {FnC,       fan_c_n,       fan_o_y,      fan_o_n,     Pr1,       in},
  {Fnd,             0,           255,           10,     Pr1,       in},
  {Fct,             0,            50,           10,     Pr2,       in}, //0的時候無此功能
  {FSt,           -50,            50,            2,     Pr1,       in},
  {Fon,             0,            15,            0,     Pr1,       in},
  {FoF,             0,            15,            0,     Pr1,       in},
  {FAP,    defrost_np,    defrost_p4,   defrost_p2,     Pr2,       in},
  {ALC,      alarm_rE,      alarm_Ab,     alarm_Ab,     Pr2,       in}, //rE=相對於設定點; Ab=絕對溫度
  {ALU,         -50.0,         110.0,        110.0,     Pr2,       dE},
  {ALL,         -50.0,         110.0,        -50.0,     Pr2,       dE},
  {AFH,           0.1,          25.5,          1.0,     Pr2,       dE},
  {ALd,             0,           255,           15,     Pr2,       in},
  {dAO,           0.0,          23.5,          1.3,     Pr2,       dE},
  {AP2,    defrost_np,    defrost_p4,   defrost_p4,     Pr2,       in},
  {AL2,           -55,           150,          -40,     Pr2,       in},
  {Au2,           -55,           150,          110,     Pr2,       in},
  {AH2,           0.1,          25.5,          5.0,     Pr2,       dE},
  {Ad2,             0,           255,           15,     Pr2,       in},
  {dA2,             0,          23.5,          1.3,     Pr2,       dE}, //單位:小時, 分辨率10分鐘
  {bLL,       work_no,      work_yes,      work_no,     Pr2,       in},
  {AC2,       work_no,      work_yes,      work_no,     Pr2,       in},
  {i1P,      polar_CL,      polar_OP,     polar_CL,     Pr2,       in},
  {i1F,     alarm_EAL,      alarm_ES,    alarm_dor,     Pr2,       in},
  {did,             0,           255,           15,     Pr2,       in},
  {nPS,             0,            15,           15,     Pr2,       in},
  {odc,      state_no,     state_F_C,    state_F_C,     Pr2,       in},
  {rrd,       work_no,      work_yes,     work_yes,     Pr2,       in},
  {HES,           -30,            30,            0,     Pr2,       in},
  {Adr,             1,           244,            1,     Pr2,       in},
  {PbC,      type_Ptc,      type_ntc,     type_ntc,     Pr2,       in},
  {onF,     button_nu,     button_ES,    button_ES,     Pr2,       in},
  {dp1,             0,             0,            0,     Pr1,      NaNt},
  {dp2,             0,             0,            0,     Pr1,      NaNt},
  {dp3,             0,             0,            0,     Pr2,      NaNt},
  {dp4,             0,             0,            0,     Pr2,      NaNt},
  {rSE,             0,             0,            0,     Pr1,      NaNt},
  {rEL,             0,           999,           10,     Pr2,      NaNt},
  {Ptb,             0,             0,            0,     Pr2,      NaNt},
};

uint8_t User_reset[SPIAddr_End] = {};
int16_t sys_table[End] = {};

void original_to_reset(void)
{
  uint8_t i = xxx;
  uint8_t addr=SPIAddr_Start;
  int16_t i16_value;

  static uint8_t max_index = var_end;
  int16_t User_int[max_index] = {};

  while(i<=max_index)
  {
  //把原始的使用者數值(小數點)轉換成10倍並shift成正整數
    i16_value = User_original[i].DefaultValue - User_original[i].Range_Low;

    if(User_original[i].DataDigit == in)
      User_int[i] = i16_value; //整數值直接讀取
    else if(User_original[i].DataDigit == dE)
      User_int[i] = i16_value*10; //小數值需放大10倍

    printf("User_int[%d]: %d\r\n",i , User_int[i]);

  //把原始的使用者數值(小數點)轉換成10倍後放入準備寫入eeprom的陣列內
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
  }

// //寫入原廠設定
//   printf("開始恢復原廠值\r\n");
//   R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
//   I2C_EE_BufferWrite(User_reset, 0x00, SPIAddr_End);
//   R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
//   printf("完成恢復原廠值\r\n");
}

void eepread_to_systable(void)
{
  uint8_t i = xxx;
  uint8_t addr=SPIAddr_Start;

  static uint8_t max_index = End;
  uint8_t eep_read[SPIAddr_End] = {};
  int16_t i16_value;

  //讀出eeprom的數值
  // R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  // I2C_EE_BufferRead(eep_read, 0x00, SPIAddr_End);
  // R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);

//組合成int16_t的值並shift成User值
  while(i<=max_index)
  {
    if((addr==SPIAddr_Start)||(addr==SPIAddr_P2P)||(addr==SPIAddr_P3P)||(addr==SPIAddr_P4P)||(addr==SPIAddr_CF)|| \
      (addr==SPIAddr_rES)||(addr==SPIAddr_Lod)||(addr==SPIAddr_rEd)||(addr==SPIAddr_tdF)|| \
      (addr==SPIAddr_dFP)||(addr==SPIAddr_dFd)||(addr==SPIAddr_dPo)||(addr==SPIAddr_FnC)|| \
      (addr==SPIAddr_Fnd)||(addr==SPIAddr_FAP)||(addr==SPIAddr_ALC)||(addr==SPIAddr_AP2)|| \
      (addr==SPIAddr_bLL)||(addr==SPIAddr_AC2)||(addr==SPIAddr_i1P)||(addr==SPIAddr_i1F)|| \
      (addr==SPIAddr_odc)||(addr==SPIAddr_rrd)||(addr==SPIAddr_PbC)||(addr==SPIAddr_onF)
      )
    {
      //長度為1個byte的值
      // sys_table[i] = (eep_read[i] + User_original[i].Range_Low)*10;
      sys_table[i] = User_reset[addr] + (User_original[i].Range_Low*10);
      addr++;
      // printf("User_reset[%d]: %d\r\n",addr , User_reset[addr]);
      // printf("User_original[%d]: %d\r\n",i , User_original[i].Range_Low);
      printf("sys_table[%d]: %d\r\n",i , sys_table[i]);
    }
    else
    {
      //長度為2個byte的值
      // i16_value = (eep_read[addr+1]<<8) | eep_read[addr];
      i16_value = (User_reset[addr+1]<<8) | User_reset[addr];
      sys_table[i] = i16_value + (User_original[i].Range_Low*10);
      addr+=2;
      printf("sys_table[%d]: %d\r\n",i , sys_table[i]);
    }
    i++;
  }

//整個系統運算的值無小數點, 已放大10倍計算, ex. (12.5)->(125), (-55.9)->(-559)
  sys_table[rSE] = sys_table[Set];
  sys_table[rEL] = 10;           //v1.0
  printf("offset結束\r\n");
}

void systable_to_eeprom(uint8_t addr)
{
  uint8_t eep_write[2]={};
  uint8_t u8_length = 0x01;
  uint8_t u16_length = 0x02;
  uint16_t sys_value = sys_table[addr] - (User_original[addr].Range_Low*10);

  switch (addr)
  {
    case UserAddr_Start:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Start, u8_length);
      break;

    case UserAddr_Set:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Set_L, u16_length);
      break;

    case UserAddr_Hy:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Hy_L, u16_length);
      break;

    case UserAddr_LS:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_LS_L, u16_length);
      break;

    case UserAddr_US:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_US_L, u16_length);
      break;

    case UserAddr_Ot:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Ot_L, u16_length);
      break;

    case UserAddr_P2P:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_P2P, u8_length);
      break;

    case UserAddr_OE:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_OE_L, u16_length);
      break;

    case UserAddr_P3P:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_P3P, u8_length);
      break;

    case UserAddr_O3:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_O3_L, u16_length);
      break;

    case UserAddr_P4P:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_P4P, u8_length);
      break;

    case UserAddr_O4:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_O4_L, u16_length);
      break;

    case UserAddr_OdS:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_OdS_L, u16_length);
      break;

    case UserAddr_AC:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_AC_L, u16_length);
      break;

    case UserAddr_rtr:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_rtr_L, u16_length);
      break;

    case UserAddr_CCt:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_CCt_L, u16_length);
      break;

    case UserAddr_CCS:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_CCS_L, u16_length);
      break;

    case UserAddr_COn:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_COn_L, u16_length);
      break;

    case UserAddr_COF:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_COF_L, u16_length);
      break;

    case UserAddr_CF:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_CF, u8_length);
      break;

    case UserAddr_rES:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_rES, u8_length);
      break;

    case UserAddr_Lod:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Lod, u8_length);
      break;

    case UserAddr_rEd:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_rEd, u8_length);
      break;

    case UserAddr_dLY:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dLY_L, u16_length);
      break;

    case UserAddr_dtr:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dtr_L, u16_length);
      break;

    case UserAddr_tdF:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_tdF, u8_length);
      break;

    case UserAddr_dFP:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dFP, u8_length);
      break;

    case UserAddr_dtE:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dtE_L, u16_length);
      break;

    case UserAddr_IdF:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_IdF_L, u16_length);
      break;

    case UserAddr_MdF:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_MdF_L, u16_length);
      break;

    case UserAddr_dSd:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dSd_L, u16_length);
      break;

    case UserAddr_dFd:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dFd, u8_length);
      break;

    case UserAddr_dAd:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dAd_L, u16_length);
      break;

    case UserAddr_Fdt:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Fdt_L, u16_length);
      break;

    case UserAddr_dPo:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dPo, u8_length);
      break;

    case UserAddr_dAF:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dAF_L, u16_length);
      break;

    case UserAddr_FnC:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_FnC, u8_length);
      break;

    case UserAddr_Fnd:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Fnd, u8_length);
      break;

    case UserAddr_Fct:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Fct_L, u16_length);
      break;

    case UserAddr_FSt:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_FSt_L, u16_length);
      break;

    case UserAddr_Fon:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Fon_L, u16_length);
      break;

    case UserAddr_FoF:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_FoF_L, u16_length);
      break;

    case UserAddr_FAP:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_FAP, u8_length);
      break;

    case UserAddr_ALC:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_ALC, u8_length);
      break;

    case UserAddr_ALU:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_ALU_L, u16_length);
      break;

    case UserAddr_ALL:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_ALL_L, u16_length);
      break;

    case UserAddr_AFH:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_AFH_L, u16_length);
      break;

    case UserAddr_ALd:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_ALd_L, u16_length);
      break;

    case UserAddr_dAO:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dAO_L, u16_length);
      break;

    case UserAddr_AP2:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_AP2, u8_length);
      break;

    case UserAddr_AL2:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_AL2_L, u16_length);
      break;

    case UserAddr_Au2:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Au2_L, u16_length);
      break;

    case UserAddr_AH2:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_AH2_L, u16_length);
      break;

    case UserAddr_Ad2:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Ad2_L, u16_length);
      break;

    case UserAddr_dA2:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_dA2_L, u16_length);
      break;

    case UserAddr_bLL:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_bLL, u8_length);
      break;

    case UserAddr_AC2:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_AC2, u8_length);
      break;

    case UserAddr_i1P:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_i1P, u8_length);
      break;

    case UserAddr_i1F:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_i1F, u8_length);
      break;

    case UserAddr_did:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_did_L, u16_length);
      break;

    case UserAddr_nPS:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_nPS_L, u16_length);
      break;

    case UserAddr_odc:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_odc, u8_length);
      break;

    case UserAddr_rrd:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_rrd, u8_length);
      break;

    case UserAddr_HES:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_HES_L, u16_length);
      break;

    case UserAddr_Adr:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_Adr_L, u16_length);
      break;

    case UserAddr_PbC:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_PbC, u8_length);
      break;

    case UserAddr_onF:
      eep_write[0] = sys_value;
      I2C_EE_BufferWrite(eep_write, SPIAddr_onF, u8_length);
      break;

    case UserAddr_history_min:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_history_min_L, u16_length);
      break;

    case UserAddr_history_max:
      eep_write[0] = sys_value & 0xFF;
      eep_write[1] = sys_value >> 8;
      I2C_EE_BufferWrite(eep_write, SPIAddr_history_max_L, u16_length);
      break;
    
    default:
      break;
  }
}


void test_datastore(void)
{
  uint8_t i=UserAddr_Start;

  //產出reset的table, 建議每次上電都做
  original_to_reset();

  //將eeprom讀出後寫入system的table內直接使用
  eepread_to_systable();

  //測試將system的數值任意修改後寫入eeprom內
  // while(i<UserAddr_End)
  // {
  //   systable_to_eeprom(i);
  //   i++;
  //   WDT_Feed();
  // }

  while(1)
  {
    WDT_Feed();
  }
}