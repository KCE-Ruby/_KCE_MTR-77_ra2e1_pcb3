/**
 ******************************************************************************
 * @file    eep_api.h
 * @author  KCE R&D Team
 * @date    2024-09-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_EEP_API_H_ 
#ifdef INC_EEP_API_H_

/* Enum type definitions -----------------------------------------------------*/
enum SPIAddr
{ 
  /*
  * 懷疑前面有保留位元, 所以從byte5開始使用, 最後一個byte(0xff)不可使用
  * 可用的位置共有 255-2-1 = 252個byte
  * 26 (0~7 setting, 8~2047 data, page->40address, 2040/40=51 page)
  * 若是數值類型的參數, 一律使用2byte做存取
  */
  SPIAddr_Start = 0,

  SPIAddr_Hy,
  SPIAddr_Ot,
  SPIAddr_P2P,
  SPIAddr_OE,
  SPIAddr_P3P,
  SPIAddr_O3,
  SPIAddr_P4P,
  SPIAddr_O4,
  SPIAddr_OdS,
  SPIAddr_AC,
  SPIAddr_rtr,
  SPIAddr_CCt,
  SPIAddr_COn,
  SPIAddr_COF,
  SPIAddr_CF,
  SPIAddr_rES,    //0=dE; 1=in
  SPIAddr_Lod,    //0=P1; 1=P2; 2=P3; 3=P4; 4=SET
  SPIAddr_rEd,    //0=P1; 1=P2; 2=P3; 3=P4; 4=SET(預留, 無作用)
  SPIAddr_dLY,
  SPIAddr_dtr,
  SPIAddr_tdF,    //0=EL; 1=in
  SPIAddr_dFP,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4
  SPIAddr_IdF,
  SPIAddr_MdF,
  SPIAddr_dSd,
  SPIAddr_dFd,      //0=rt; 1=it; 2=SEt; 3=dEF
  SPIAddr_dAd,
  SPIAddr_Fdt,
  SPIAddr_dPo,      //0=n; 1=y
  SPIAddr_dAF,
  SPIAddr_FnC,      //0=C_n; 1=0_n; 2=C_y; 3=o_y
  SPIAddr_Fnd,
  SPIAddr_Fct,      //0 ~ 50.0
  SPIAddr_FSt,      //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_Fon,
  SPIAddr_FoF,
  SPIAddr_FAP,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4
  SPIAddr_ALC,
  SPIAddr_AFH,
  SPIAddr_ALd,
  SPIAddr_dAO,    //0.0 ~ 23.5小時
  SPIAddr_AP2,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4
  SPIAddr_AH2,
  SPIAddr_Ad2,
  SPIAddr_dA2,    //0.0 ~ 23.5小時
  SPIAddr_bLL,    //0=n; 1=y
  SPIAddr_AC2,    //0=n; 1=y
  SPIAddr_i1P,    //0=CL; 1=OP
  SPIAddr_i1F,    //0=EAL; 1=bAL; 2=PAL; 3=dor; 4=dEF; 5=AUS; 6=Htr; 7=FAn; 8=ES
  SPIAddr_did,
  SPIAddr_nPS,
  SPIAddr_odc,    //0=no; 1=Fan; 2=CPr; 3=F_C
  SPIAddr_rrd,    //0=n; 1=y
  SPIAddr_HES,  //-30.0 ~ 30.0 讀出的值需要offset
  SPIAddr_Adr,
  SPIAddr_PbC,
  SPIAddr_onF,    //0=nu; 1=oFF; 2=ES

  SPIAddr_Set_L,
  SPIAddr_Set_H,
  SPIAddr_LS_L,
  SPIAddr_LS_H,
  SPIAddr_US_L,
  SPIAddr_US_H,
  SPIAddr_CCS_L,
  SPIAddr_CCS_H,
  SPIAddr_dtE_L,  //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_dtE_H,
  SPIAddr_ALU_L,  //0.0 ~ 110.0
  SPIAddr_ALU_H,  
  SPIAddr_ALL_L,  //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_ALL_H,  
  SPIAddr_AL2_L,  //-55.0 ~ 150.0 讀出的值需要offset
  SPIAddr_AL2_H,  
  SPIAddr_Au2_L,  //-55.0 ~ 150.0 讀出的值需要offset
  SPIAddr_Au2_H,

  //非User定義參數
  SPIAddr_RecordLow_L,
  SPIAddr_RecordLow_H,
  SPIAddr_RecordHigh_L,
  SPIAddr_RecordHigh_H,

  SPIAddr_End,   //一次讀出全部時的結束點
};

enum UserAddr
{
  UserAddr_Start,

  UserAddr_Hy,  
  UserAddr_Ot,
  UserAddr_P2P,
  UserAddr_OE,
  UserAddr_P3P,
  UserAddr_O3,
  UserAddr_P4P,
  UserAddr_O4,
  UserAddr_OdS,
  UserAddr_AC,
  UserAddr_rtr,
  UserAddr_CCt,
  UserAddr_COn,
  UserAddr_COF,  
  UserAddr_CF,
  UserAddr_rES,
  UserAddr_Lod,
  UserAddr_rEd,
  UserAddr_dLY,
  UserAddr_dtr,  
  UserAddr_tdF,
  UserAddr_dFP,
  UserAddr_dtE,
  UserAddr_IdF,
  UserAddr_MdF,
  UserAddr_dSd,
  UserAddr_dFd,
  UserAddr_dAd,
  UserAddr_Fdt,
  UserAddr_dPo,
  UserAddr_dAF,
  UserAddr_FnC,
  UserAddr_Fnd,
  UserAddr_Fct,
  UserAddr_FSt,
  UserAddr_Fon,
  UserAddr_FoF,
  UserAddr_FAP,  
  UserAddr_ALC,
  UserAddr_AFH,
  UserAddr_ALd,
  UserAddr_dAO,  
  UserAddr_AP2,
  UserAddr_AH2,
  UserAddr_Ad2,
  UserAddr_dA2,
  UserAddr_bLL,
  UserAddr_AC2,  
  UserAddr_i1P,
  UserAddr_i1F,
  UserAddr_did,
  UserAddr_nPS,
  UserAddr_odc,
  UserAddr_rrd,
  UserAddr_HES,  
  UserAddr_Adr,
  UserAddr_PbC,
  UserAddr_onF,

  UserAddr_Set,
  UserAddr_LS,
  UserAddr_US,
  UserAddr_CCS,
  UserAddr_ALU,
  UserAddr_ALL,
  UserAddr_AL2,
  UserAddr_Au2,
  UserAddr_RecordHigh,
  UserAddr_RecordLow,
  
  UserAddr_End,
};

/* typedef definitions -----------------------------------------------------*/

/* task function protocol -----------------------------------------------*/
void EEP_ResetALL(void);
uint32_t EEP_Read_API(uint8_t addr);
void EEP_Write_API(uint8_t addr, uint32_t value);

void EEPROM_TEST(void);
uint8_t I2C_Test_1(void);

#endif /* INC_EEP_API_H_ */
