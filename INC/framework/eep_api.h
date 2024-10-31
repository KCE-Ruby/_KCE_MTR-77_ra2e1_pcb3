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

  SPIAddr_Set_L,
  SPIAddr_Set_H,

  SPIAddr_Hy_L,
  SPIAddr_Hy_H,

  SPIAddr_LS_L,
  SPIAddr_LS_H,

  SPIAddr_US_L,
  SPIAddr_US_H,

  SPIAddr_Ot_L,
  SPIAddr_Ot_H,

  SPIAddr_P2P,
  SPIAddr_OE_L,
  SPIAddr_OE_H,

  SPIAddr_P3P,
  SPIAddr_O3_L,
  SPIAddr_O3_H,

  SPIAddr_P4P,
  SPIAddr_O4_L,
  SPIAddr_O4_H,

  SPIAddr_OdS_L,
  SPIAddr_OdS_H,
  SPIAddr_AC_L,
  SPIAddr_AC_H,
  SPIAddr_rtr_L,
  SPIAddr_rtr_H,
  SPIAddr_CCt_L,
  SPIAddr_CCt_H,
  SPIAddr_CCS_L,
  SPIAddr_CCS_H,

  SPIAddr_COn_L,
  SPIAddr_COn_H,
  SPIAddr_COF_L,
  SPIAddr_COF_H,

  SPIAddr_CF,

  SPIAddr_rES,    //0=dE; 1=in
  SPIAddr_Lod,    //0=P1; 1=P2; 2=P3; 3=P4; 4=SET
  SPIAddr_rEd,    //0=P1; 1=P2; 2=P3; 3=P4; 4=SET(預留, 無作用)

  SPIAddr_dLY_L,
  SPIAddr_dLY_H,
  SPIAddr_dtr_L,
  SPIAddr_dtr_H,

  SPIAddr_tdF,    //0=EL; 1=in
  SPIAddr_dFP,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4

  SPIAddr_dtE_L,    //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_dtE_H,
  
  SPIAddr_IdF_L,
  SPIAddr_IdF_H,
  SPIAddr_MdF_L,
  SPIAddr_MdF_H,
  SPIAddr_dSd_L,
  SPIAddr_dSd_H,

  SPIAddr_dFd,      //0=rt; 1=it; 2=SEt; 3=dEF

  SPIAddr_dAd_L,
  SPIAddr_dAd_H,
  SPIAddr_Fdt_L,
  SPIAddr_Fdt_H,

  SPIAddr_dPo,      //0=n; 1=y

  SPIAddr_dAF_L,
  SPIAddr_dAF_H,

  SPIAddr_FnC,      //0=C_n; 1=0_n; 2=C_y; 3=o_y
  SPIAddr_Fnd,
  
  SPIAddr_Fct_L,    //0 ~ 50.0
  SPIAddr_Fct_H,
  
  SPIAddr_FSt_L,    //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_FSt_H,

  SPIAddr_Fon_L,
  SPIAddr_Fon_H,
  SPIAddr_FoF_L,
  SPIAddr_FoF_H,

  SPIAddr_FAP,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4
  SPIAddr_ALC,
  
  SPIAddr_ALU_L,  //0.0 ~ 110.0
  SPIAddr_ALU_H,
  
  SPIAddr_ALL_L,  //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_ALL_H,
  
  SPIAddr_AFH_L,
  SPIAddr_AFH_H,
  SPIAddr_ALd_L,
  SPIAddr_ALd_H,
  SPIAddr_dAO_L,    //0.0 ~ 23.5小時
  SPIAddr_dAO_H,

  SPIAddr_AP2,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4
  
  SPIAddr_AL2_L,  //-55.0 ~ 150.0 讀出的值需要offset
  SPIAddr_AL2_H,
  
  SPIAddr_Au2_L,  //-55.0 ~ 150.0 讀出的值需要offset
  SPIAddr_Au2_H,

  SPIAddr_AH2_L,
  SPIAddr_AH2_H,
  SPIAddr_Ad2_L,
  SPIAddr_Ad2_H,
  SPIAddr_dA2_L,    //0.0 ~ 23.5小時
  SPIAddr_dA2_H,

  SPIAddr_bLL,    //0=n; 1=y
  SPIAddr_AC2,    //0=n; 1=y
  SPIAddr_i1P,    //0=CL; 1=OP
  SPIAddr_i1F,    //0=EAL; 1=bAL; 2=PAL; 3=dor; 4=dEF; 5=AUS; 6=Htr; 7=FAn; 8=ES
  SPIAddr_did_L,
  SPIAddr_did_H,
  SPIAddr_nPS_L,
  SPIAddr_nPS_H,
  SPIAddr_odc,    //0=no; 1=Fan; 2=CPr; 3=F_C
  SPIAddr_rrd,    //0=n; 1=y
  
  SPIAddr_HES_L,  //-30.0 ~ 30.0 讀出的值需要offset
  SPIAddr_HES_H,
  SPIAddr_Adr_L,
  SPIAddr_Adr_H,

  SPIAddr_PbC,
  SPIAddr_onF,    //0=nu; 1=oFF; 2=ES

  SPIAddr_history_min_L,
  SPIAddr_history_min_H,
  SPIAddr_history_max_L,
  SPIAddr_history_max_H,

  SPIAddr_End,   //一次讀出全部時的結束點
};

enum UserAddr
{
  UserAddr_Start,

  UserAddr_Set,

  UserAddr_Hy,
  UserAddr_LS,
  UserAddr_US,  
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
  UserAddr_CCS,
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
  UserAddr_ALU,
  UserAddr_ALL,
  UserAddr_AFH,
  UserAddr_ALd,
  UserAddr_dAO,
  
  UserAddr_AP2,
  UserAddr_AL2,
  UserAddr_Au2,
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

  UserAddr_history_min,
  UserAddr_history_max,
  
  UserAddr_End,
};

/* typedef definitions -----------------------------------------------------*/

/* task function protocol -----------------------------------------------*/
void EEP_ResetALL(void);
uint32_t EEP_Read_API(uint8_t addr);
void EEP_Write_API(uint8_t addr, uint32_t value);

void EEPROM_TEST(void);

#endif /* INC_EEP_API_H_ */
