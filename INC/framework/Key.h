/**
 ******************************************************************************
 * @file    Key.h
 * @author  KCE R&D Team
 * @date    2024-06-26
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_KEY_H_ 
#ifdef INC_KEY_H_

#define Key_Read R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_14, &KeyPin)
/* Enum type definitions -----------------------------------------------------*/
typedef struct
{
  uint8_t shortPressed;
  uint8_t LongPressed;
  uint16_t Cnt;
} Key_Manager;

enum KEY_name
{
  keystandby=1,
  keyup,
  key_limp,
  key_forst,
  keydown,
  keyset,
};

enum DISP_LEVEL
{
  DSIP_Set,
  DSIP_Hy,
  DSIP_LS,
  DSIP_US,
  DSIP_Ot,
  DSIP_P2P,
  DSIP_OE,
  DSIP_P3P,
  DSIP_O3,
  DSIP_P4P,
  DSIP_O4,
  DSIP_Ods,
  DSIP_AC,
  DSIP_rtr,
  DSIP_CCt,
  DSIP_CCS,
  DSIP_COn,
  DSIP_COF,
  DSIP_CF,
  DSIP_rES,
  DSIP_Lod,
  DSIP_rEd,
  DSIP_dLy,
  DSIP_dtr,
  DSIP_dFP,
  DSIP_tdF,
  DSIP_dtE,
  DSIP_IdF,
  DSIP_MdF,
  DSIP_dSd,
  DSIP_dFd,
  DSIP_dAd,
  DSIP_Fdt,
  DSIP_dPo,
  DSIP_dAF,
  DSIP_Fnc,
  DSIP_Fnd,
  DSIP_Fct,
  DSIP_FSt,
  DSIP_Fon,
  DSIP_FoF,
  DSIP_FAP,
  DSIP_ALc,
  DSIP_ALU,
  DSIP_ALL,
  DSIP_AFH,
  DSIP_ALd,
  DSIP_dAO,
  DSIP_AP2,
  DSIP_AL2,
  DSIP_AU2,
  DSIP_AH2,
  DSIP_Ad2,
  DSIP_dA2,
  DSIP_bLL,
  DSIP_AC2,
  DSIP_i1P,
  DSIP_i1F,
  DSIP_did,
  DSIP_nPS,
  DSIP_odc,
  DSIP_rrd,
  DSIP_HES,
  DSIP_Adr,
  DSIP_Pbc,
  DSIP_onF,
  DSIP_dp1,
  DSIP_dp2,
  DSIP_dp3,
  DSIP_dp4,
  DSIP_rSE,
  DSIP_rEL,
  DSIP_Ptb,
};


/* task function protocol -----------------------------------------------*/
void Key_main(void);
void Key_debounce(void);

void Key_test(void);
void Key_oneshot(void);
#endif /* INC_KEY_H_ */
