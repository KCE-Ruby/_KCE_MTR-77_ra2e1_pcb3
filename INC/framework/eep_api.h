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
{ //26 (0~7 setting, 8~2047 data, page->40address, 2040/40=51 page)
  SPIAddr_EEPWrite_times,   //0 or 1

  SPIAddr_Set_L,
  SPIAddr_Set_H,

  SPIAddr_Hy_H,

  SPIAddr_LS_L,
  SPIAddr_LS_H,

  SPIAddr_US_L,
  SPIAddr_US_H,

  SPIAddr_Ot,     //-12.0~12.0 讀出的值需要offset

  SPIAddr_P2P,    //0=n; 1=y
  SPIAddr_OE,     //-12.0~12.0 讀出的值需要offset

  SPIAddr_P3P,    //0=n; 1=y
  SPIAddr_O3,     //-12.0~12.0 讀出的值需要offset

  SPIAddr_P4P,    //0=n; 1=y
  SPIAddr_O4,     //-12.0~12.0 讀出的值需要offset

  SPIAddr_OdS,
  SPIAddr_AC,
  SPIAddr_rtr,
  SPIAddr_CCt,    //0~24.0

  SPIAddr_CCS_L,  //-55.0~150.0 讀出的值需要offset
  SPIAddr_CCS_H,

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

  SPIAddr_dtE_L,    //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_dtE_H,
  
  SPIAddr_IdF,
  SPIAddr_MdF,
  SPIAddr_dSd,
  SPIAddr_dFd,      //0=rt; 1=it; 2=SEt; 3=dEF
  SPIAddr_dAd,
  SPIAddr_Fdt,
  SPIAddr_dPo,      //0=n; 1=y
  SPIAddr_dAF,      //0~23.5小時
  SPIAddr_FnC,      //0=C_n; 1=0_n; 2=C_y; 3=o_y
  SPIAddr_Fnd,
  
  SPIAddr_Fct_L,    //0 ~ 50.0
  SPIAddr_Fct_H,
  
  SPIAddr_FSt_L,    //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_FSt_H,

  SPIAddr_Fon,
  SPIAddr_FoF,
  SPIAddr_FAP,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4
  SPIAddr_ALC,
  
  SPIAddr_ALU_L,  //0.0 ~ 110.0
  SPIAddr_ALU_H,
  
  SPIAddr_ALL_L,  //-50.0 ~ 50.0 讀出的值需要offset
  SPIAddr_ALL_H,
  
  SPIAddr_AFH,
  SPIAddr_ALd,
  SPIAddr_dAO,    //0.0 ~ 23.5小時
  SPIAddr_AP2,    //0=nP; 1=P1; 2=P2; 3=P3; 4=P4
  
  SPIAddr_AL2_L,  //-55.0 ~ 150.0 讀出的值需要offset
  SPIAddr_AL2_H,
  
  SPIAddr_Au2_L,  //-55.0 ~ 150.0 讀出的值需要offset
  SPIAddr_Au2_H,

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
  
  SPIAddr_HES_L,  //-30.0 ~ 30.0 讀出的值需要offset
  SPIAddr_HES_H,

  SPIAddr_Adr,
  SPIAddr_PbC,
  SPIAddr_onF,    //0=nu; 1=oFF; 2=ES
};

enum UserAddr
{
  UserAddr_EEPWrite_times,

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
};

/* typedef definitions -----------------------------------------------------*/


/* task function protocol -----------------------------------------------*/
void EEP_ResetALL(void);
uint32_t EEP_Read_API(uint8_t addr);
void EEP_Write_API(uint8_t addr, uint32_t value);

#endif /* INC_EEP_API_H_ */
