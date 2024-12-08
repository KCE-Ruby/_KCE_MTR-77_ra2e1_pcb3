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
#include "INC/framework/outAPI.h"
#include "INC/framework/data_store.h"


/* Private defines ----------------------------------------------------------*/
static void user_to_eepreset(uint8_t index);
static void original_to_reset(void);
static void eepread_to_user(uint8_t index);
static void eepread_to_systable(void);
static void systable_to_eeprom(uint8_t addr);

/* extern variables -----------------------------------------------------------------*/
extern __IO s_Var Syscfg;
extern __IO ADC_TemperatureValue TempValue;

/* variables -----------------------------------------------------------------*/
//請記得如果修改了User_original的陣列, 需要讓系統重置請將eep_read[Str]旗標寫成NeedtoReset
__IO ByteSettingTable User_original[End] = 
{
  {Str,  New_eeprom,  NeedtoReset,   No_problem,     NaN,       in}, //對齊參數用的而已
  {Set,           -50,         110.0,         -5.0,     NaN,       dE},
  //參數字元,  下限值,        上限值,        預設值,   權限層      整數
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

static uint8_t max_index = var_end;
uint16_t User_int[var_end] = {};
static uint8_t User_reset[SPIAddr_End] = {};
uint8_t eep_read[255] = {};
int16_t sys_table[End] = {};
int16_t pre_table[End] = {};

static void user_to_eepreset(uint8_t index)
{
  bool user_to_eep_err=false;

  switch (index)
  {
    //長度為1個byte的值
    case Str:
      User_reset[SPIAddr_Start] = User_int[index];
      // printf("Str裡面User_int[%d]: %d\r\n",index , User_int[index]);
      // printf("Str裡面User_reset[%d]: %d\r\n",SPIAddr_Start , User_reset[SPIAddr_Start]);
      break;
    case Hy:
      User_reset[SPIAddr_Hy] = User_int[index];
      // printf("Hy裡面User_int[%d]: %d\r\n",index , User_int[index]);
      // printf("Hy裡面User_reset[%d]: %d\r\n",SPIAddr_Hy , User_reset[SPIAddr_Hy]);
      break;
    case Ot:
      User_reset[SPIAddr_Ot] = User_int[index];
      // printf("Ot裡面User_int[%d]: %d\r\n",index , User_int[index]);
      // printf("Ot裡面User_reset[%d]: %d\r\n",SPIAddr_Ot , User_reset[SPIAddr_Ot]);
      break;
    case P2P:
      User_reset[SPIAddr_P2P] = User_int[index];
      break;
    case OE:
      User_reset[SPIAddr_OE] = User_int[index];
      break;
    case P3P:
      User_reset[SPIAddr_P3P] = User_int[index];
      break;
    case O3:
      User_reset[SPIAddr_O3] = User_int[index];
      break;
    case P4P:
      User_reset[SPIAddr_P4P] = User_int[index];
      break;
    case O4:
      User_reset[SPIAddr_O4] = User_int[index];
      break;
    case OdS:
      User_reset[SPIAddr_OdS] = User_int[index];
      break;
    case AC:
      User_reset[SPIAddr_AC] = User_int[index];
      break;
    case rtr:
      User_reset[SPIAddr_rtr] = User_int[index];
      break;
    case CCt:
      User_reset[SPIAddr_CCt] = User_int[index];
      break;
    case COn:
      User_reset[SPIAddr_COn] = User_int[index];
      break;
    case COF:
      User_reset[SPIAddr_COF] = User_int[index];
      break;
    case CF:
      User_reset[SPIAddr_CF] = User_int[index];
      break;
    case rES:
      User_reset[SPIAddr_rES] = User_int[index];
      break;
    case Lod:
      User_reset[SPIAddr_Lod] = User_int[index];
      break;
    case rEd:
      User_reset[SPIAddr_rEd] = User_int[index];
      break;
    case dLY:
      User_reset[SPIAddr_dLY] = User_int[index];
      break;
    case dtr:
      User_reset[SPIAddr_dtr] = User_int[index];
      break;
    case tdF:
      User_reset[SPIAddr_tdF] = User_int[index];
      break;
    case dFP:
      User_reset[SPIAddr_dFP] = User_int[index];
      break;
    case IdF:
      User_reset[SPIAddr_IdF] = User_int[index];
      break;
    case MdF:
      User_reset[SPIAddr_MdF] = User_int[index];
      break;
    case dSd:
      User_reset[SPIAddr_dSd] = User_int[index];
      break;
    case dFd:
      User_reset[SPIAddr_dFd] = User_int[index];
      break;
    case dAd:
      User_reset[SPIAddr_dAd] = User_int[index];
      break;
    case Fdt:
      User_reset[SPIAddr_Fdt] = User_int[index];
      break;
    case dPo:
      User_reset[SPIAddr_dPo] = User_int[index];
      break;
    case dAF:
      User_reset[SPIAddr_dAF] = User_int[index];
      break;
    case FnC:
      User_reset[SPIAddr_FnC] = User_int[index];
      break;
    case Fnd:
      User_reset[SPIAddr_Fnd] = User_int[index];
      break;
    case Fct:
      User_reset[SPIAddr_Fct] = User_int[index];
      break;
    case FSt:
      User_reset[SPIAddr_FSt] = User_int[index];
      break;
    case Fon:
      User_reset[SPIAddr_Fon] = User_int[index];
      break;
    case FoF:
      User_reset[SPIAddr_FoF] = User_int[index];
      break;
    case FAP:
      User_reset[SPIAddr_FAP] = User_int[index];
      break;
    case ALC:
      User_reset[SPIAddr_ALC] = User_int[index];
      break;
    case AFH:
      User_reset[SPIAddr_AFH] = User_int[index];
      break;
    case ALd:
      User_reset[SPIAddr_ALd] = User_int[index];
      break;
    case dAO:
      User_reset[SPIAddr_dAO] = User_int[index];
      break;
    case AP2:
      User_reset[SPIAddr_AP2] = User_int[index];
      break;
    case AH2:
      User_reset[SPIAddr_AH2] = User_int[index];
      break;
    case Ad2:
      User_reset[SPIAddr_Ad2] = User_int[index];
      break;
    case dA2:
      User_reset[SPIAddr_dA2] = User_int[index];
      break;
    case bLL:
      User_reset[SPIAddr_bLL] = User_int[index];
      break;
    case AC2:
      User_reset[SPIAddr_AC2] = User_int[index];
      break;
    case i1P:
      User_reset[SPIAddr_i1P] = User_int[index];
      break;
    case i1F:
      User_reset[SPIAddr_i1F] = User_int[index];
      break;
    case did:
      User_reset[SPIAddr_did] = User_int[index];
      break;
    case nPS:
      User_reset[SPIAddr_nPS] = User_int[index];
      break;
    case odc:
      User_reset[SPIAddr_odc] = User_int[index];
      break;
    case rrd:
      User_reset[SPIAddr_rrd] = User_int[index];
      break;
    case HES:
      User_reset[SPIAddr_HES] = User_int[index];
      break;
    case Adr:
      User_reset[SPIAddr_Adr] = User_int[index];
      break;
    case PbC:
      User_reset[SPIAddr_PbC] = User_int[index];
      break;
    case onF:
      User_reset[SPIAddr_onF] = User_int[index];
      break;

    //長度為2個byte的值
    case Set:
      User_reset[SPIAddr_Set_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_Set_L] = User_int[index] & 0xff;  //取低位元
      break;
    case LS:
      User_reset[SPIAddr_LS_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_LS_L] = User_int[index] & 0xff;  //取低位元
      break;
    case US:
      User_reset[SPIAddr_US_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_US_L] = User_int[index] & 0xff;  //取低位元
      break;
    case CCS:
      User_reset[SPIAddr_CCS_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_CCS_L] = User_int[index] & 0xff;  //取低位元
      break;
    case dtE:
      User_reset[SPIAddr_dtE_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_dtE_L] = User_int[index] & 0xff;  //取低位元
      break;
    case ALU:
      User_reset[SPIAddr_ALU_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_ALU_L] = User_int[index] & 0xff;  //取低位元
      break;
    case ALL:
      User_reset[SPIAddr_ALL_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_ALL_L] = User_int[index] & 0xff;  //取低位元
      break;
    case AL2:
      User_reset[SPIAddr_AL2_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_AL2_L] = User_int[index] & 0xff;  //取低位元
      break;
    case Au2:
      User_reset[SPIAddr_Au2_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_Au2_L] = User_int[index] & 0xff;  //取低位元
      break;

    case RecordLow:
      User_reset[SPIAddr_RecordLow_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_RecordLow_L] = User_int[index] & 0xff;  //取低位元
      break;
    case RecordHigh:
      User_reset[SPIAddr_RecordHigh_H] = User_int[index] >> 8;    //取高位元
      User_reset[SPIAddr_RecordHigh_L] = User_int[index] & 0xff;  //取低位元
      break;

    default:
      printf("user_to_eepreset輸入錯誤\r\n");
      user_to_eep_err = true;
      break;
  }
}

static void original_to_reset(void)
{
  uint8_t i = Str;
  float dE_value;

  while(i<max_index)
  {
  //把原始的使用者數值(小數點)shift成正數
    dE_value = User_original[i].DefaultValue - User_original[i].Range_Low;

  //判斷是否需轉換成10倍做eeprom的存取(亦為系統使用值)
    if(User_original[i].DataDigit == in)
      User_int[i] = (uint16_t)dE_value; //整數值直接讀取
    else if(User_original[i].DataDigit == dE)
      User_int[i] = (uint16_t)(dE_value*10); //小數值需放大10倍
    // printf("外面User_int[%d]: %d\r\n",i , User_int[i]);

  //把原始的使用者數值轉換後, 準備寫入eeprom的陣列內, addr的順序與Usertable排列不同
    user_to_eepreset(i);
    // printf("外面User_reset[%d]: %d\r\n",i , User_reset[i]);
    i++;
  }

}

static void eepread_to_user(uint8_t index)
{
  int16_t i16_value=0;
  uint8_t user_addr=0;
  bool eeptouser_err=false;

//設定對應的參數值
  switch (index)
  {
  //長度為1個byte的值
    case Str:
      i16_value = eep_read[SPIAddr_Start];
      break;
    case Hy:
      i16_value = eep_read[SPIAddr_Hy];
      break;
    case Ot:
      i16_value = eep_read[SPIAddr_Ot];
      break;
    case P2P:
      i16_value = eep_read[SPIAddr_P2P];
      break;
    case OE:
      i16_value = eep_read[SPIAddr_OE];
      break;
    case P3P:
      i16_value = eep_read[SPIAddr_P3P];
      break;
    case O3:
      i16_value = eep_read[SPIAddr_O3];
      break;
    case P4P:
      i16_value = eep_read[SPIAddr_P4P];
      break;
    case O4:
      i16_value = eep_read[SPIAddr_O4];
      break;
    case OdS:
      i16_value = eep_read[SPIAddr_OdS];
      break;
    case AC:
      i16_value = eep_read[SPIAddr_AC];
      break;
    case rtr:
      i16_value = eep_read[SPIAddr_rtr];
      break;
    case CCt:
      i16_value = eep_read[SPIAddr_CCt];
      break;
    case COn:
      i16_value = eep_read[SPIAddr_COn];
      break;
    case COF:
      i16_value = eep_read[SPIAddr_COF];
      break;
    case CF:
      i16_value = eep_read[SPIAddr_CF];
      break;
    case rES:
      i16_value = eep_read[SPIAddr_rES];
      break;
    case Lod:
      i16_value = eep_read[SPIAddr_Lod];
      break;
    case rEd:
      i16_value = eep_read[SPIAddr_rEd];
      break;
    case dLY:
      i16_value = eep_read[SPIAddr_dLY];
      break;
    case dtr:
      i16_value = eep_read[SPIAddr_dtr];
      break;
    case tdF:
      i16_value = eep_read[SPIAddr_tdF];
      break;
    case dFP:
      i16_value = eep_read[SPIAddr_dFP];
      break;
    case IdF:
      i16_value = eep_read[SPIAddr_IdF];
      break;
    case MdF:
      i16_value = eep_read[SPIAddr_MdF];
      break;
    case dSd:
      i16_value = eep_read[SPIAddr_dSd];
      break;
    case dFd:
      i16_value = eep_read[SPIAddr_dFd];
      break;
    case dAd:
      i16_value = eep_read[SPIAddr_dAd];
      break;
    case Fdt:
      i16_value = eep_read[SPIAddr_Fdt];
      break;
    case dPo:
      i16_value = eep_read[SPIAddr_dPo];
      break;
    case dAF:
      i16_value = eep_read[SPIAddr_dAF];
      break;
    case FnC:
      i16_value = eep_read[SPIAddr_FnC];
      break;
    case Fnd:
      i16_value = eep_read[SPIAddr_Fnd];
      break;
    case Fct:
      i16_value = eep_read[SPIAddr_Fct];
      break;
    case FSt:
      i16_value = eep_read[SPIAddr_FSt];
      break;
    case Fon:
      i16_value = eep_read[SPIAddr_Fon];
      break;
    case FoF:
      i16_value = eep_read[SPIAddr_FoF];
      break;
    case FAP:
      i16_value = eep_read[SPIAddr_FAP];
      break;
    case ALC:
      i16_value = eep_read[SPIAddr_ALC];
      break;
    case AFH:
      i16_value = eep_read[SPIAddr_AFH];
      break;
    case ALd:
      i16_value = eep_read[SPIAddr_ALd];
      break;
    case dAO:
      i16_value = eep_read[SPIAddr_dAO];
      break;
    case AP2:
      i16_value = eep_read[SPIAddr_AP2];
      break;
    case AH2:
      i16_value = eep_read[SPIAddr_AH2];
      break;
    case Ad2:
      i16_value = eep_read[SPIAddr_Ad2];
      break;
    case dA2:
      i16_value = eep_read[SPIAddr_dA2];
      break;
    case bLL:
      i16_value = eep_read[SPIAddr_bLL];
      break;
    case AC2:
      i16_value = eep_read[SPIAddr_AC2];
      break;
    case i1P:
      i16_value = eep_read[SPIAddr_i1P];
      break;
    case i1F:
      i16_value = eep_read[SPIAddr_i1F];
      break;
    case did:
      i16_value = eep_read[SPIAddr_did];
      break;
    case nPS:
      i16_value = eep_read[SPIAddr_nPS];
      break;
    case odc:
      i16_value = eep_read[SPIAddr_odc];
      break;
    case rrd:
      i16_value = eep_read[SPIAddr_rrd];
      break;
    case HES:
      i16_value = eep_read[SPIAddr_HES];
      break;
    case Adr:
      i16_value = eep_read[SPIAddr_Adr];
      break;
    case PbC:
      i16_value = eep_read[SPIAddr_PbC];
      break;
    case onF:
      i16_value = eep_read[SPIAddr_onF];
      break;

  //長度為2個byte的值
    case Set:
      i16_value = (eep_read[SPIAddr_Set_H]<<8) | eep_read[SPIAddr_Set_L];
      // i16_value = (User_reset[SPIAddr_Set_H]<<8) | User_reset[SPIAddr_Set_L];
     break;
    case LS:
      i16_value = (eep_read[SPIAddr_LS_H]<<8) | eep_read[SPIAddr_LS_L];
      break;
    case US:
      i16_value = (eep_read[SPIAddr_US_H]<<8) | eep_read[SPIAddr_US_L];
      break;
    case CCS:
      i16_value = (eep_read[SPIAddr_CCS_H]<<8) | eep_read[SPIAddr_CCS_L];
      break;
    case dtE:
      i16_value = (eep_read[SPIAddr_dtE_H]<<8) | eep_read[SPIAddr_dtE_L];
      break;
    case ALU:
      i16_value = (eep_read[SPIAddr_ALU_H]<<8) | eep_read[SPIAddr_ALU_L];
      break;
    case ALL:
      i16_value = (eep_read[SPIAddr_ALL_H]<<8) | eep_read[SPIAddr_ALL_L];
      break;
    case AL2:
      i16_value = (eep_read[SPIAddr_AL2_H]<<8) | eep_read[SPIAddr_AL2_L];
      break;
    case Au2:
      i16_value = (eep_read[SPIAddr_Au2_H]<<8) | eep_read[SPIAddr_Au2_L];
      break;
    case RecordHigh:
      i16_value = (eep_read[SPIAddr_RecordHigh_H]<<8) | eep_read[SPIAddr_RecordHigh_L];
      break;
    case RecordLow:
      i16_value = (eep_read[SPIAddr_RecordLow_H]<<8) | eep_read[SPIAddr_RecordLow_L];
      break;
    
    default:
      printf("eepread_to_user輸入錯誤\r\n");
      eeptouser_err = true;
      break;
  }

//若輸入的address正確, 判斷是否需將offset做10倍的shift(亦為系統使用值)
  if(eeptouser_err==false)
  {
    if(User_original[index].DataDigit == in)
      sys_table[index] = i16_value + User_original[index].Range_Low; //整數值直接讀取
    else if(User_original[index].DataDigit == dE)
      sys_table[index] = i16_value + (User_original[index].Range_Low*10); //小數值需放大10倍
  }
}

static void eepread_to_systable(void)
{
  uint8_t i = Str;

//組合成int16_t的值並shift成User值
  while(i<max_index)
  {
    eepread_to_user(i);
    i++;
  }

//整個系統運算的值無小數點, 已放大10倍計算, ex. (12.5)->(125), (-55.9)->(-559)
  sys_table[rSE] = sys_table[Set];
  sys_table[rEL] = 10;           //v1.0
  // printf("offset結束\r\n");
}

static void systable_to_eeprom(uint8_t addr)
{
  uint8_t eep_write[2]={};
  uint8_t length=0;
  uint8_t u8_length = 0x01;
  uint8_t u16_length = 0x02;
  uint16_t sys_value = sys_table[addr] - (User_original[addr].Range_Low*10);
  uint8_t spi_addr=0;

  switch (addr)
  {
    case Str:
      spi_addr = SPIAddr_Start;
      break;
    case Hy:
      spi_addr = SPIAddr_Start;
      break;
    case Ot:
      spi_addr = SPIAddr_Start;
      break;
    case P2P:
      spi_addr = SPIAddr_P2P;
      break;
    case OE:
      spi_addr = SPIAddr_OE;
      break;
    case P3P:
      spi_addr = SPIAddr_P3P;
      break;
    case O3:
      spi_addr = SPIAddr_O3;
      break;
    case P4P:
      spi_addr = SPIAddr_P4P;
      break;
    case O4:
      spi_addr = SPIAddr_O4;
      break;
    case OdS:
      spi_addr = SPIAddr_OdS;
      break;
    case AC:
      spi_addr = SPIAddr_AC;
      break;
    case rtr:
      spi_addr = SPIAddr_rtr;
      break;
    case CCt:
      spi_addr = SPIAddr_CCt;
      break;
    case COn:
      spi_addr = SPIAddr_COn;
      break;
    case COF:
      spi_addr = SPIAddr_COF;
      break;
    case CF:
      spi_addr = SPIAddr_CF;
      break;
    case rES:
      spi_addr = SPIAddr_rES;
      break;
    case Lod:
      spi_addr = SPIAddr_Lod;
      break;
    case rEd:
      spi_addr = SPIAddr_rEd;
      break;
    case dLY:
      spi_addr = SPIAddr_dLY;
      break;
    case dtr:
      spi_addr = SPIAddr_dtr;
      break;
    case tdF:
      spi_addr = SPIAddr_tdF;
      break;
    case dFP:
      spi_addr = SPIAddr_dFP;
      break;
    case IdF:
      spi_addr = SPIAddr_IdF;
      break;
    case MdF:
      spi_addr = SPIAddr_MdF;
      break;
    case dSd:
      spi_addr = SPIAddr_dSd;
      break;
    case dFd:
      spi_addr = SPIAddr_dFd;
      break;
    case dAd:
      spi_addr = SPIAddr_dAd;
      break;
    case Fdt:
      spi_addr = SPIAddr_Fdt;
      break;
    case dPo:
      spi_addr = SPIAddr_dPo;
      break;
    case dAF:
      spi_addr = SPIAddr_dAF;
      break;
    case FnC:
      spi_addr = SPIAddr_FnC;
      break;
    case Fnd:
      spi_addr = SPIAddr_Fnd;
      break;
    case Fct:
      spi_addr = SPIAddr_Fct;
      break;
    case FSt:
      spi_addr = SPIAddr_FSt;
      break;
    case Fon:
      spi_addr = SPIAddr_Fon;
      break;
    case FoF:
      spi_addr = SPIAddr_FoF;
      break;
    case FAP:
      spi_addr = SPIAddr_FAP;
      break;
    case ALC:
      spi_addr = SPIAddr_ALC;
      break;
    case AFH:
      spi_addr = SPIAddr_AFH;
      break;
    case ALd:
      spi_addr = SPIAddr_ALd;
      break;
    case dAO:
      spi_addr = SPIAddr_dAO;
      break;
    case AP2:
      spi_addr = SPIAddr_AP2;
      break;
    case AH2:
      spi_addr = SPIAddr_AH2;
      break;
    case Ad2:
      spi_addr = SPIAddr_Ad2;
      break;
    case dA2:
      spi_addr = SPIAddr_dA2;
      break;
    case bLL:
      spi_addr = SPIAddr_bLL;
      break;
    case AC2:
      spi_addr = SPIAddr_AC2;
      break;
    case i1P:
      spi_addr = SPIAddr_i1P;
      break;
    case i1F:
      spi_addr = SPIAddr_i1F;
      break;
    case did:
      spi_addr = SPIAddr_did;
      break;
    case nPS:
      spi_addr = SPIAddr_nPS;
      break;
    case odc:
      spi_addr = SPIAddr_odc;
      break;
    case rrd:
      spi_addr = SPIAddr_rrd;
      break;
    case HES:
      spi_addr = SPIAddr_HES;
      break;
    case Adr:
      spi_addr = SPIAddr_Adr;
      break;
    case PbC:
      spi_addr = SPIAddr_PbC;
      break;
    case onF:
      spi_addr = SPIAddr_onF;
      break;


    case Set:
      spi_addr = SPIAddr_Set_L;
      break;
    case LS:
      spi_addr = SPIAddr_LS_L;
      break;
    case US:
      spi_addr = SPIAddr_US_L;
      break;
    case CCS:
      spi_addr = SPIAddr_CCS_L;
      break;
    case dtE:
      spi_addr = SPIAddr_dtE_L;
      break;
    case ALU:
      spi_addr = SPIAddr_ALU_L;
      break;
    case ALL:
      spi_addr = SPIAddr_ALL_L;
      break;
    case AL2:
      spi_addr = SPIAddr_AL2_L;
      break;
    case Au2:
      spi_addr = SPIAddr_Au2_L;
      break;
    case RecordLow:
      spi_addr = SPIAddr_RecordLow_L;
      break;
    case RecordHigh:
      spi_addr = SPIAddr_RecordHigh_L;
      break;

    default:
      break;
  }


  if(length == u8_length)
  {
    eep_write[0] = sys_value;
    I2C_EE_BufferWrite(eep_write, spi_addr, length);
  }
  else if(length == u16_length)
  {
    eep_write[0] = sys_value & 0xFF;
    eep_write[1] = sys_value >> 8;
    I2C_EE_BufferWrite(eep_write, spi_addr, length);
  }
}

void datastore_boot(void)


//系統共用數值轉換
void get_Pv(void)
{
  //Syscfg.value參數基準點為小數後一位, 所以數值已經放大10倍, 取整數的話要除10回來
  const uint16_t rtr_const = sys_table[rtr]/10;
  const uint16_t dtr_const = sys_table[dtr]/10;
  const uint8_t source = 8;
  int16_t pv_1, pv_2, pv_3, pv_4;
  int16_t t1_f, t2_f, t3_f, t4_f;

  //目前判斷點是依據rtr參數將P1與P2的比例做調整, 整合後為pv值使用
  if(sys_table[CF] == degree_F)
  {
    t1_f = celsius_to_fahrenheit(TempValue.sensor1);
    t2_f = celsius_to_fahrenheit(TempValue.sensor2);
    t3_f = celsius_to_fahrenheit(TempValue.sensor3);
    t4_f = celsius_to_fahrenheit(TempValue.sensor4);

    pv_1 = t1_f + sys_table[Ot];
    pv_2 = t2_f + sys_table[OE];
    pv_3 = t3_f + sys_table[O3];
    pv_4 = t4_f + sys_table[O4];
  }
  else
  {
    pv_1 = TempValue.sensor1 + sys_table[Ot];
    pv_2 = TempValue.sensor2 + sys_table[OE];
    pv_3 = TempValue.sensor3 + sys_table[O3];
    pv_4 = TempValue.sensor4 + sys_table[O4];
  }
  if(sys_table[Lod] == disp_dtr)
    Syscfg.pv_disp = (dtr_const * (pv_1-pv_2) /100) + pv_2;
  else if (sys_table[Lod] == disp_P1)
    Syscfg.pv_disp = pv_1;
  else if (sys_table[Lod] == disp_P2)
    Syscfg.pv_disp = pv_2;

  Syscfg.pv = (rtr_const * (pv_1-pv_2) /100) + pv_2;
}

void get_RecordLow(void)
{
  uint8_t length = 2;

  if(Syscfg.pv < Syscfg.RecordLow)
  {
    Syscfg.RecordLow = Syscfg.pv;
    systable_to_eeprom(SPIAddr_RecordLow_L);
  }
}

void get_RecordHigh(void)
{
  uint8_t length = 2;

  if(Syscfg.pv > Syscfg.RecordHigh)
  {
    Syscfg.RecordHigh = Syscfg.pv;
    systable_to_eeprom(SPIAddr_RecordHigh_L);
  }
}

void upload_syscfg_data(int8_t pr_index)
{
  sys_table[pr_index] = pre_table[pr_index];
}

{
  uint8_t i=Str;
  uint8_t length = SPIAddr_End-1;

//產出reset要用的table (User_reset[])
  original_to_reset();

//步驟1:讀出eeprom的數值 (eep_read[])
  printf("步驟1:讀出eeprom的數值\r\n");
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  I2C_EE_BufferRead(eep_read, 0x00, length);
  R_BSP_SoftwareDelay(100U, BSP_DELAY_UNITS_MILLISECONDS);

//請記得如果修改了User_original的陣列, 需要讓系統重置請將eep_read[Str]旗標寫成NeedtoReset
//也就是把這段打開就好
  // #define resetsystem
  #ifdef resetsystem
    eep_read[Str] = NeedtoReset;
    printf("成功修改User_original陣列_繁體\r\n");
  #endif

//步驟2:判斷是否需要reset, 若要reset則產出reset的table
  printf("步驟2:判斷是否需要reset\r\n");
  if(eep_read[Str] != No_problem)
  {
    //寫入原廠設定
    printf("寫入的數據_繁體\r\n");
    for ( i=0; i<length; i++ ) { //填充缓冲
        printf("0x%02X ", User_reset[i]);
        if (i%16 == 15)
            printf("\n");
    }
    printf("\n");
    printf("開始恢復原廠值\r\n");
    R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
      I2C_EE_BufferWrite(User_reset, 0x00, length);
    R_BSP_SoftwareDelay(100U, BSP_DELAY_UNITS_MILLISECONDS);
      printf("完成恢復原廠值\r\n");
  }
  else
    printf("系統值不須重置\r\n");

//步驟3:將eepread值轉換成system值直接使用 (sys_table[])
  printf("步驟3:將eepread值轉換成system值\r\n");
  eepread_to_systable();
}