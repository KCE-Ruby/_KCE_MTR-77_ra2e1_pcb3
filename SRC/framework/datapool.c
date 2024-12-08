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
#include "INC/framework/ADC.h"
#include "INC/framework/eep_api.h"
#include "INC/framework/datapool.h"
#include <math.h>


#define DEBUG_PRINT                 (true)
/* Private defines ----------------------------------------------------------*/
#define LS_OFFSET                   (-500)   //參數的offset值, -50.0
#define US_OFFSET                   (-500)   //參數的offset值, -50.0
#define Ot_OFFSET                   (-120)   //參數的offset值, -12.0

/* extern variables -----------------------------------------------------------------*/
extern __IO s_Var Syscfg, Preload;
extern ADC_TemperatureValue TempValue;
extern uint8_t EE_Buf_Read[SPIAddr_End];

/* variables -----------------------------------------------------------------*/
static int8_t I2c_Buf_Write[eep_end] = {};    //TODO:這是暫時的, 應該要在eepapi才做設定
__IO uint8_t bytetable_pr1[End];
__IO uint8_t bytetable_pr2[End];

int16_t EE_Buf_u16[UserAddr_End];
__IO uint16_t rsttable[End];
__IO SYSTEM_TABLE systable[End];
/* variables -----------------------------------------------------------------*/
__IO ByteSettingTable bytetable[End] = 
{
  {Str,             0,             1,            1,     NaN,       in}, //對齊參數用的而已
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

/* variables tranfer */
void UserTabletoSytem(void)
{
  uint8_t i=0;
  while(i<End)
  {
    systable[i].Range_Low = bytetable[i].Range_Low*10;
    systable[i].Range_High = bytetable[i].Range_High*10;
    systable[i].DefaultValue = bytetable[i].DefaultValue*10;
    rsttable[i] = (uint16_t)(systable[i].DefaultValue - systable[i].Range_Low);
    i++;
  }
}

/* Parameters from EEPROM to SYSTEM API ------------------------------------------------------*/
void offset_EEtoSYS(void)
{
  uint8_t i=Str, addr=SPIAddr_Start;
  // static int16_t EE_Buf_u16[UserAddr_End] = {0}; //這個陣列不會被改變, 拿來存eeprom讀出來的值而已

  //將所有eeprom的值讀出來到int16_t的陣列內, 只有非數值型的參數用u8取值 
  while(i < UserAddr_End)
  {
    if((addr==SPIAddr_Start)||(addr==SPIAddr_P2P)||(addr==SPIAddr_P3P)||(addr==SPIAddr_P4P)||(addr==SPIAddr_CF)|| \
      (addr==SPIAddr_rES)||(addr==SPIAddr_Lod)||(addr==SPIAddr_rEd)||(addr==SPIAddr_tdF)|| \
      (addr==SPIAddr_dFP)||(addr==SPIAddr_dFd)||(addr==SPIAddr_dPo)||(addr==SPIAddr_FnC)|| \
      (addr==SPIAddr_Fnd)||(addr==SPIAddr_FAP)||(addr==SPIAddr_ALC)||(addr==SPIAddr_AP2)|| \
      (addr==SPIAddr_bLL)||(addr==SPIAddr_AC2)||(addr==SPIAddr_i1P)||(addr==SPIAddr_i1F)|| \
      (addr==SPIAddr_odc)||(addr==SPIAddr_rrd)||(addr==SPIAddr_PbC)||(addr==SPIAddr_onF)
      )
    {
      //把eeprom內uint8_t的值直接放入一格陣列, 並做offset
      EE_Buf_u16[i] = EE_Buf_Read[addr];
      EE_Buf_u16[i] += systable[i].Range_Low;
      // printf("EE_Buf_u16 = %d\r\n", EE_Buf_u16[i]);
      // printf("EE_Buf_Read = %d\r\n", EE_Buf_Read[addr]);
      // printf("Range_Low = %d\r\n", systable[i].Range_Low);
      addr++;
    }
    else
    {
      EE_Buf_u16[i] = EE_Buf_Read[addr+1] << 8; //取高位元
      EE_Buf_u16[i] |= EE_Buf_Read[addr];       //取低位元
      EE_Buf_u16[i] += (int16_t)systable[i].Range_Low;     //做offset
      // printf("EE_Buf_u16 = %d\r\n", EE_Buf_u16[i]);
      // printf("EE_Buf_Read = %d\r\n", EE_Buf_Read[addr]);
      // printf("Range_Low = %d\r\n", systable[i].Range_Low);
      addr+=2;
    }
    i++;
    WDT_Feed();
  }
  printf("讀出EE_Buf_u16結束, i=%d\r\n", i);

  //整個系統運算的值無小數點, 已放大10倍計算, ex. (12.5)->(125), (-55.9)->(-559)
  i=UserAddr_Set;
  while(i < UserAddr_End)
  {
    switch (i)
    {
      case UserAddr_OdS:
      case UserAddr_AC:
      case UserAddr_COn:
      case UserAddr_COF:
      case UserAddr_Lod:
      case UserAddr_dFP:
      case UserAddr_tdF:
        Preload.value[i] = EE_Buf_u16[i]/10;
        Syscfg.value[i] = Preload.value[i];
        break;
      
      default:
        Preload.value[i] = EE_Buf_u16[i];
        Syscfg.value[i] = Preload.value[i];
        break;
    }
    i++;
    WDT_Feed();
  }
  printf("讀出Syscfg結束, i=%d\r\n", i);

  Syscfg.value[rSE] = Syscfg.value[Set];
  Syscfg.value[rEL] = 10;           //v1.0
  printf("將讀出值做offset結束\r\n");
}

/* Function definitions ------------------------------------------------------*/
void get_Pv_old(void)
{
  //Syscfg.value參數基準點為小數後一位, 所以數值已經放大10倍, 取整數的話要除10回來
  const uint16_t rtr_const = Syscfg.value[rtr]/10;
  const uint16_t dtr_const = Syscfg.value[dtr]/10;
  const uint8_t source = 8;
  int16_t pv_1, pv_2, pv_3, pv_4;
  int16_t t1_f, t2_f, t3_f, t4_f;

  //目前判斷點是依據rtr參數將P1與P2的比例做調整, 整合後為pv值使用
  if(Syscfg.value[CF] == degree_F)
  {
    t1_f = celsius_to_fahrenheit(TempValue.sensor1);
    t2_f = celsius_to_fahrenheit(TempValue.sensor2);
    t3_f = celsius_to_fahrenheit(TempValue.sensor3);
    t4_f = celsius_to_fahrenheit(TempValue.sensor4);

    pv_1 = t1_f + Syscfg.value[Ot];
    pv_2 = t2_f + Syscfg.value[OE];
    pv_3 = t3_f + Syscfg.value[O3];
    pv_4 = t4_f + Syscfg.value[O4];
  }
  else
  {
    pv_1 = TempValue.sensor1 + Syscfg.value[Ot];
    pv_2 = TempValue.sensor2 + Syscfg.value[OE];
    pv_3 = TempValue.sensor3 + Syscfg.value[O3];
    pv_4 = TempValue.sensor4 + Syscfg.value[O4];
  }
  if(Syscfg.value[Lod] == disp_dtr)
    Syscfg.pv_disp = (dtr_const * (pv_1-pv_2) /100) + pv_2;
  else if (Syscfg.value[Lod] == disp_P1)
    Syscfg.pv_disp = pv_1;
  else if (Syscfg.value[Lod] == disp_P2)
    Syscfg.pv_disp = pv_2;

  Syscfg.pv = (rtr_const * (pv_1-pv_2) /100) + pv_2;

  // printf("rtr_const = %d\r\n", rtr_const);
  // printf("pv_1 = %d\r\n", pv_1);
  // printf("pv_2 = %d\r\n", pv_2);
  // printf("Syscfg.pv = %d\r\n", Syscfg.pv);
}

void get_HistoryMax_old(void)
{
  uint8_t length = 2;

  if(Syscfg.pv > Syscfg.RecordHigh)
  {
    Syscfg.RecordHigh = Syscfg.pv;
    I2c_Buf_Write[SPIAddr_RecordHigh_L] = Syscfg.RecordHigh & 0xFF;
    I2c_Buf_Write[SPIAddr_RecordHigh_H] = Syscfg.RecordHigh >> 8;
    // I2C_EE_BufferWrite( I2c_Buf_Write, addr, length);
  }
}

void get_HistoryMin_old(void)
{
  uint8_t length = 2;

  if(Syscfg.pv < Syscfg.RecordLow)
  {
    Syscfg.RecordLow = Syscfg.pv;
    I2c_Buf_Write[SPIAddr_RecordLow_L] = Syscfg.RecordLow & 0xFF;
    I2c_Buf_Write[SPIAddr_RecordLow_H] = Syscfg.RecordLow >> 8;
    // I2C_EE_BufferWrite( I2c_Buf_Write, addr, length);
  }
}

uint8_t get_bytetable_pr1(void)
{
  uint8_t index=0, check_index=0;
  while(check_index < End)
  {
    if(bytetable[check_index].Mode == Pr1)
    {
      bytetable_pr1[index] = bytetable[check_index].UserByte;
      index++;
    }
    check_index++;
  }
  // printf("get_bytetable_pr1測試結束3: %d\r\n", index);
  return index;
}

uint8_t get_bytetable_pr2(void)
{
  uint8_t index=0, check_index=0;
  while(check_index < End)
  {
    if((bytetable[check_index].Mode==Pr1) || (bytetable[check_index].Mode==Pr2))
    {
      bytetable_pr2[index] = bytetable[check_index].UserByte;             //取得Pr2層的字符table
      index++;
    }
    check_index++;
  }
  // printf("get_bytetable_pr2測試結束3: %d\r\n", index);
  return index;
}

int16_t check_Limit_Value(int16_t data, int8_t index)
{
  int16_t max_data, min_data;

  //除了需要特殊處理的部分其它以bytetable陣列的預設值做為標準
  switch (index)
  {
    case Set:
      //設定值的最大允許值 = US
      max_data = (int16_t)(Syscfg.value[US]);
      //設定值的最小允許值 = LS
      min_data = (int16_t)(Syscfg.value[LS]);
      if(data > max_data)
        data = max_data;
      else if(data < min_data)
        data = min_data;
      break;

    case OdS:
    case AC:
    case COn:
    case COF:
    case Lod:
    case dFP:
    case tdF:
      //for level1&2參數內的數值使用, 直接顯示數值不*10倍
      max_data = (int16_t)(bytetable[index].Range_High);
      min_data = (int16_t)(bytetable[index].Range_Low);
      if(data > max_data)
        data = max_data;
      else if(data < min_data)
        data = min_data;
      break;

    default:
    //for level1&2參數內的數值使用
      max_data = (int16_t)(bytetable[index].Range_High*10);
      min_data = (int16_t)(bytetable[index].Range_Low*10);
      if(data > max_data)
      {
        data = max_data;
        // printf("數值大於最大值, 取代後 = %d\r\n", data);
      }
      else if(data < min_data)
      {
        data = min_data;
        // printf("數值小於最小值, 取代後 = %d\r\n", data);
      }
      else
      {
        // printf("數值不須處理\r\n");
      }
      break;
  }
  // printf("max_data = %d\r\n", max_data);
  // printf("min_data = %d\r\n", min_data);

  return data;
}

int16_t celsius_to_fahrenheit(int16_t pv)
{
  int16_t ret;
    // ret = (pv * 9.0 / 5.0) + 320;
    ret = (pv * 9 + 1600) / 5;
  return ret;
}

/* upload data Function definitions ------------------------------------------------------*/
// void upload_syscfg_data(int8_t pr_index)
// {
//   sys_table[pr_index] = pre_table[pr_index];
// }





