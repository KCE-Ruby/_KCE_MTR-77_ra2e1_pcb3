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
#include "INC/framework/LED_Driver/app_menu_ctrl.h"
#include "INC/framework/ADC.h"
#include "INC/framework/eep_api.h"
#include "INC/framework/datapool.h"
#include <math.h>


#define DEBUG_PRINT
/* Private defines ----------------------------------------------------------*/
#define GAIN_PARAMETER              (10)     //bytetable參數預設值的增益倍數
#define LS_OFFSET                   (-500)   //參數的offset值, -50.0
#define US_OFFSET                   (-500)   //參數的offset值, -50.0
#define Ot_OFFSET                   (-120)   //參數的offset值, -12.0

/* extern variables -----------------------------------------------------------------*/
extern __IO s_Var System;
extern ADC_TemperatureValue TempValue;

/* variables -----------------------------------------------------------------*/
static int8_t I2c_Buf_Write[eep_end] = {};    //TODO:這是暫時的, 應該要在eepapi才做設定
__IO uint8_t bytetable_pr1[Pr1_size];
__IO uint8_t bytetable_pr2[Pr2_size];
__IO ByteSettingTable bytetable[End] = 
{
  {xxx,             0,             0,            0,     NaN}, //對齊參數用的而已
  {Set,           -50,           110,         -5.0,     NaN},
  //參數字元,  下限值,        上限值,        預設值,   權限層
  { Hy,           0.1,          25.5,          2.0,     Pr1},
  { LS,         -50.0,           110,        -50.0,     Pr2},
  { US,         -50.0,           110,          110,     Pr2},
  { Ot,           -12,            12,            0,     Pr1},
  {P2P,       exist_n,       exist_y,      exist_n,     Pr2}, //n=不存在; y=存在
  { OE,           -12,            12,            0,     Pr2},
  {P3P,       exist_n,       exist_y,      exist_n,     Pr2}, //n=不存在; y=存在
  { O3,           -12,            12,            0,     Pr2},
  {P4P,       exist_n,       exist_y,      exist_n,     Pr2}, //n=不存在; y=存在
  { O4,           -12,            12,            0,     Pr2},
  {OdS,             0,           255,            0,     Pr1},
  { AC,             0,            50,            1,     Pr1},
  {rtr,             0,           100,          100,     Pr2}, //P1=100, P2=0
  {CCt,           0.0,          24.0,            0,     Pr2}, //精度為0.1hour = 6min
  {CCS,         -55.0,         150.0,           -5,     Pr2},
  {COn,             0,           255,           15,     Pr1},
  {COF,             0,           255,           30,     Pr1},
  { CF,      degree_C,      degree_F,     degree_C,     Pr2}, //攝氏=C, 華氏=F
  {rES,  DECIMAL_AT_1,  DECIMAL_AT_0, DECIMAL_AT_1,     Pr2}, //小數=dE=DECIMAL_AT_1, 整數=in=DECIMAL_AT_0
  {Lod,       disp_P1,      disp_dtr,      disp_P1,     Pr2},
  {rEd,       disp_P1,      disp_dtr,      disp_P1,     NaN},
  {dLY,             0,          20.0,            0,     Pr2}, //單位:0~20.0分鐘, 分辨率10秒
  {dtr,             0,           100,           50,     Pr2}, //P1=100, P2=0
  {tdF,       type_EL,       type_in,      type_EL,     Pr2},
  {dFP,    defrost_np,    defrost_p4,   defrost_p2,     Pr2},
  {dtE,           -50,            50,            8,     Pr1},
  {IdF,             1,           120,            6,     Pr1},
  {MdF,             0,           255,           30,     Pr1}, //0的時候不融霜
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
  {rSE,             0,             0,            0,     Pr2},
  {rEL,             0,             0,            0,     Pr2},
  {Ptb,             0,             0,            0,     Pr2},
};


/* Static Function protocol -----------------------------------------------*/
static int16_t offset_EEtoSYS_u8(uint8_t Table, uint8_t SPIaddr);
static int16_t offset_EEtoSYS_u16(uint8_t Table, uint8_t SPIaddr);

/* Static Function definitions ------------------------------------------------------*/
static int16_t offset_EEtoSYS_u8(uint8_t Table, uint8_t SPIaddr)
{
  uint8_t EE_Buf_Read[End] = {};
  int16_t ret;

  EE_Buf_Read[SPIaddr] = (uint8_t)((bytetable[Table].DefaultValue-bytetable[Table].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  ret = (int16_t)(EE_Buf_Read[SPIaddr]+(bytetable[Table].Range_Low*GAIN_PARAMETER));

  return ret;
}

static int16_t offset_EEtoSYS_u16(uint8_t Table, uint8_t SPIaddr)
{
  uint16_t base_u16=0;
  uint8_t EE_Buf_Read[End] = {};
  int16_t ret;

  base_u16 = (uint16_t)((bytetable[Table].DefaultValue-bytetable[Table].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  EE_Buf_Read[SPIaddr+1] = base_u16 >> 8;      //取高位元, TODO:EEPROM的API會取代這行
  EE_Buf_Read[SPIaddr] = base_u16 & 0xff;    //取低位元, TODO:EEPROM的API會取代這行
  base_u16 = (int16_t)(EE_Buf_Read[SPIaddr+1]<<8) + EE_Buf_Read[SPIaddr];  //組合成變數
  ret = (int16_t)(base_u16+(bytetable[Table].Range_Low*GAIN_PARAMETER));
  
  return ret;
}

/* Function definitions ------------------------------------------------------*/
void offset_EEtoSYS(void)
{
  //可能跟read_all_eeprom_data同功能
  //整個系統運算的值無小數點, 已放大10倍計算, ex. (12.5)->(125), (-55.9)->(-559)

  // uint16_t base_u16=0;
  // double print_double = 0;
  // uint8_t EE_Buf_Read[End] = {};
  int16_t EE_Buf_u16[End] = {};

  //Set
  EE_Buf_u16[UserAddr_Set] = offset_EEtoSYS_u16(Set, SPIAddr_Set_L);
  // base_u16 = (uint16_t)((bytetable[Set].DefaultValue-bytetable[Set].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_Set_H] = base_u16 >> 8;      //取高位元, TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_Set_L] = base_u16 & 0xff;    //取低位元, TODO:EEPROM的API會取代這行
  // base_u16 = (int16_t)(EE_Buf_Read[SPIAddr_Set_H]<<8) + EE_Buf_Read[SPIAddr_Set_L];  //組合成變數
  // EE_Buf_u16[UserAddr_Set] = (int16_t)(base_u16+(bytetable[Set].Range_Low*GAIN_PARAMETER));

  //Hy
  EE_Buf_u16[UserAddr_Hy] = offset_EEtoSYS_u8(Hy, SPIAddr_Hy);
  // EE_Buf_Read[SPIAddr_Hy] = (uint8_t)((bytetable[Hy].DefaultValue-bytetable[Hy].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  // EE_Buf_u16[UserAddr_Hy] = (int16_t)(EE_Buf_Read[SPIAddr_Hy]+(bytetable[Hy].Range_Low*GAIN_PARAMETER));

  //LS
  EE_Buf_u16[UserAddr_LS] = offset_EEtoSYS_u16(LS, SPIAddr_LS_L);
  // base_u16 = (uint16_t)((bytetable[LS].DefaultValue-bytetable[LS].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_LS_H] = base_u16 >> 8;      //取高位元, TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_LS_L] = base_u16 & 0xff;    //取低位元, TODO:EEPROM的API會取代這行
  // base_u16 = (int16_t)(EE_Buf_Read[SPIAddr_LS_H]<<8) + EE_Buf_Read[SPIAddr_LS_L];  //組合成變數
  // EE_Buf_u16[UserAddr_LS] = (int16_t)(base_u16+(bytetable[LS].Range_Low*GAIN_PARAMETER));

  //US
  EE_Buf_u16[UserAddr_US] = offset_EEtoSYS_u16(US, SPIAddr_US_L);
  // base_u16 = (uint16_t)((bytetable[US].DefaultValue-bytetable[US].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_US_H] = base_u16 >> 8;      //取高位元, TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_US_L] = base_u16 & 0xff;    //取低位元, TODO:EEPROM的API會取代這行
  // base_u16 = (int16_t)(EE_Buf_Read[SPIAddr_US_H]<<8) + EE_Buf_Read[SPIAddr_US_L];  //組合成變數
  // EE_Buf_u16[UserAddr_US] = (int16_t)(base_u16+(bytetable[US].Range_Low*GAIN_PARAMETER));

  //Ot
  EE_Buf_u16[UserAddr_Ot] = offset_EEtoSYS_u8(Ot, SPIAddr_Ot);
  // base_u16 = (uint8_t)((bytetable[Ot].DefaultValue-bytetable[Ot].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_Ot] = base_u16; //TODO:EEPROM的API會取代這行
  // EE_Buf_u16[UserAddr_Ot] = (int16_t)(EE_Buf_Read[SPIAddr_Ot]+(bytetable[Ot].Range_Low*GAIN_PARAMETER));

  //P2P
  EE_Buf_u16[UserAddr_P2P] = offset_EEtoSYS_u8(P2P, SPIAddr_P2P);
  // base_u16 = (uint8_t)((bytetable[P2P].DefaultValue-bytetable[P2P].Range_Low)*GAIN_PARAMETER); //TODO:EEPROM的API會取代這行
  // EE_Buf_Read[SPIAddr_P2P] = base_u16; //TODO:EEPROM的API會取代這行
  // EE_Buf_u16[UserAddr_P2P] = (int16_t)(EE_Buf_Read[SPIAddr_P2P]+(bytetable[P2P].Range_Low*GAIN_PARAMETER));


  #ifdef DEBUG_PRINT
  {
    printf("EE_Buf_u16[UserAddr_Set] = %d \r\n", EE_Buf_u16[UserAddr_Set]/GAIN_PARAMETER);
    printf("EE_Buf_u16[UserAddr_Hy]  = %d \r\n", EE_Buf_u16[UserAddr_Hy]/GAIN_PARAMETER);
    printf("EE_Buf_u16[UserAddr_LS]  = %d \r\n", EE_Buf_u16[UserAddr_LS]/GAIN_PARAMETER);
    printf("EE_Buf_u16[UserAddr_US]  = %d \r\n", EE_Buf_u16[UserAddr_US]/GAIN_PARAMETER);
    printf("EE_Buf_u16[UserAddr_Ot]  = %d \r\n", EE_Buf_u16[UserAddr_Ot]/GAIN_PARAMETER);
    printf("EE_Buf_u16[UserAddr_P2P] = %d \r\n", EE_Buf_u16[UserAddr_P2P]/GAIN_PARAMETER);
  }
  #endif
}

void get_Pv(void)
{
  //Pv值來自於sensor1 or 2, TODO:訊號源未新增
  System.pv = TempValue.sensor1;
}

void get_HistoryMax(void)
{
  uint8_t addr = eep_max_low;
  uint8_t length = eep_size_max;

  if(System.pv > System.history_max)
  {
    System.history_max = System.pv;
    I2c_Buf_Write[addr] = System.history_max & 0xFF;
    I2c_Buf_Write[addr+1] = System.history_max >> 8;
    // I2C_EE_BufferWrite( I2c_Buf_Write, addr, length);
  }
}

void get_HistoryMin(void)
{
  uint8_t addr = eep_min_low;
  uint8_t length = eep_size_min;

  if(System.pv < System.history_min)
  {
    System.history_min = System.pv;
    I2c_Buf_Write[addr] = System.history_min & 0xFF;
    I2c_Buf_Write[addr+1] = System.history_min >> 8;
    // I2C_EE_BufferWrite( I2c_Buf_Write, addr, length);
  }
}

void clear_History_value(void)
{
  System.history_max = 0;
  System.history_min = 0;
}

void get_bytetable_pr1(void)
{
  static uint8_t index=0, check_index=0;
  while(check_index <= End)
  {
    if(bytetable[check_index].Mode == Pr1)
    {
      bytetable_pr1[index] = bytetable[check_index].UserByte;
      index++;
    }
    check_index++;
  }
  index = 0;
  check_index = 0;
}





