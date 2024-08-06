/**
 ******************************************************************************
 * @file    Display.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 * 調用應用層指令, 組合成使用者手冊的功能
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/Display.h"

/* extern variables -----------------------------------------------------------------*/
extern __IO r_tmr tmr;
extern __IO bool ALL_LED_FLAG;
extern __IO LED_SCAN1 Scan1Led;
extern __IO LED_SCAN2 Scan2Led;
extern __IO LED_SCAN3 Scan3Led;
extern __IO ICON_SCAN4 Scan4Icon;
extern __IO ICON_SCAN5 Scan5Icon;

extern __IO LED_SCAN1 Scan1Digi;
extern __IO LED_SCAN2 Scan2LedDigi;
extern __IO LED_SCAN3 Scan3LedDigi;

/* variables -----------------------------------------------------------------*/


/* Private function protocol -----------------------------------------------*/
static void COM_lightup(uint8_t com, bool switch_com);
static void Disalbe_COM(void);
static void SetValue_COM(uint8_t com);
static void PUSHPIN(uint8_t com);
static void Enable_COM(uint8_t com);
static void LED_WriteIOState(uint8_t StateBuff);

/* Function definitions ------------------------------------------------------*/
void LED_Display(void)
{
  bool switch_com = false;
  if(tmr.COM_Port != tmr.COM_Port_pre)
  {
    if(tmr.COM_Port>5)
      tmr.COM_Port=0;
    tmr.COM_Port_pre = tmr.COM_Port;
    switch_com = true;
  }
  COM_lightup(tmr.COM_Port, switch_com);
}

/* Static Function definitions ------------------------------------------------------*/
static void COM_lightup(uint8_t com, bool switch_com)
{
  if(switch_com)
    Disalbe_COM();
  SetValue_COM(com);
  PUSHPIN(com);
  if(switch_com)
    Enable_COM(com);
}

static void Disalbe_COM(void)
{
  SMG_1_CLOSE;
  SMG_2_CLOSE;
  SMG_3_CLOSE;
  SMG_4_CLOSE;
  SMG_5_CLOSE;
  SMG_6_CLOSE;
}

static void SetValue_COM(uint8_t com)
{
  switch (com)
  {
    case 0:
      Scan1Led.pincombine = Scan1Digi.pincombine;
    break;
    case 1:
      Scan2Led.pincombine = Scan1Digi.pincombine;
    break;
    case 2:
      Scan3Led.pincombine = Scan1Digi.pincombine;
    break;
    case 3:
      Scan4Icon.pincombine = Scan1Digi.pincombine;
    break;
    case 4:
      Scan5Icon.pincombine = Scan1Digi.pincombine;
    break;
  }
}

static void PUSHPIN(uint8_t com)
{
  uint8_t _8bitData;
  if(ALL_LED_FLAG)
  {
    _8bitData = 0xFF;
    LED_WriteIOState(_8bitData);
  }
  else
  {
    switch (com)
    {
      case 0:
        LED_WriteIOState(Scan1Led.pincombine);
      break;
      case 1:
        LED_WriteIOState(Scan2Led.pincombine);
      break;
      case 2:
        LED_WriteIOState(Scan3Led.pincombine);
      break;
      case 3:
        LED_WriteIOState(Scan4Icon.pincombine);
      break;
      case 4:
        LED_WriteIOState(Scan5Icon.pincombine);
      break;
    }
  }
}

static void Enable_COM(uint8_t com)
{
  switch (com)
  {
    case 0:
      SMG_1_OPEN;
      break;
    case 1:
      SMG_2_OPEN;
      break;
    case 2:
      SMG_3_OPEN;
      break;
    case 3:
      SMG_4_OPEN;
      break;
    case 4:
      SMG_5_OPEN;
      break;
    case 5:
      SMG_6_OPEN;
      break;
  }
}

static void LED_WriteIOState(uint8_t StateBuff)
{
  //將map a~dp轉成實際pin狀態
  if(StateBuff & 0b00000001) SMG_A_OPEN; else SMG_A_CLOSE;
  if(StateBuff & 0b00000010) SMG_B_OPEN; else SMG_B_CLOSE;
  if(StateBuff & 0b00000100) SMG_C_OPEN; else SMG_C_CLOSE;
  if(StateBuff & 0b00001000) SMG_D_OPEN; else SMG_D_CLOSE;
  if(StateBuff & 0b00010000) SMG_E_OPEN; else SMG_E_CLOSE;
  if(StateBuff & 0b00100000) SMG_F_OPEN; else SMG_F_CLOSE;
  if(StateBuff & 0b01000000) SMG_G_OPEN; else SMG_G_CLOSE;
  if(StateBuff & 0b10000000) SMG_DP_OPEN; else SMG_DP_CLOSE;
}

