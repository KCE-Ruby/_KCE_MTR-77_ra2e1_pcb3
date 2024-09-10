/**
 ******************************************************************************
 * @file    eep_api.c
 * @author  KCE R&D Team
 * @date    2024-09-09
 * @author  Ruby
 * 所有跟eeprom相關的應用程式碼
 * 資料長度為2個byte, 所以要做再處理才能直接用API讀寫一個參數
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/eep_api.h"
#include "eeprom/i2c_ee.h"

/* extern variables -----------------------------------------------------------------*/
extern __IO s_Var System;

/* variables -----------------------------------------------------------------*/
static uint8_t EEP_Read_Data_u8(uint8_t addr);
static uint16_t EEP_Read_Data_u8Convertu16(uint8_t addr);
static int32_t EEP_Read_API(uint8_t addr);
static void EEP_Write_Data_u8_write(uint8_t addr, uint8_t* value);
static void EEP_Write_Data_u16Convertu8(uint8_t addr, uint16_t value);
static void EEP_Write_API(uint8_t addr, int32_t value);

/* Static Function definitions ------------------------------------------------------*/
static uint8_t EEP_Read_Data_u8(uint8_t addr)
{
  uint8_t SPI_u8;

  I2C_EE_BufferRead(&SPI_u8, addr , 1);

  // if(dbgType.watchdog)
    // debug_print("[test]: u8read = 0x%X\r\n", SPI_u8);
  return SPI_u8;
}

static uint16_t EEP_Read_Data_u8Convertu16(uint8_t addr)
{
  uint8_t SPI_u16Table[2];
  uint16_t value;

  I2C_EE_BufferRead(&SPI_u16Table[0], addr+0 , 1);
  I2C_EE_BufferRead(&SPI_u16Table[1], addr+1 , 1);
  // SPI_EEP_ReadData(addr+0, &SPI_u16Table[0]);
  // SPI_EEP_ReadData(addr+1, &SPI_u16Table[1]);
  value = (SPI_u16Table[1]<<8) + SPI_u16Table[0];

  // if(dbgType.watchdog)
  //   debug_print("[test]: u16read = 0x%X\r\n", value);
  return value;
}

static int32_t EEP_Read_API(uint8_t addr)
{
  int32_t data=0;

  switch (addr)
  {
    case UserAddr_EEPWrite_times:
      data = EEP_Read_Data_u8(SPIAddr_EEPWrite_times);
      break;
    case UserAddr_Set:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_Set_L);
      break;
    case UserAddr_Hy:
      data = EEP_Read_Data_u8(SPIAddr_Hy);
      break;
    case UserAddr_LS:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_LS_L);
      break;
    case UserAddr_US:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_US_L);
      break;
    case UserAddr_Ot:
      data = EEP_Read_Data_u8(SPIAddr_Ot);
      data += Ot_offset;   //讀出來的值需要offset
      break;
    case UserAddr_P2P:
      data = EEP_Read_Data_u8(SPIAddr_P2P);
      break;
    case UserAddr_OE:
      data = EEP_Read_Data_u8(SPIAddr_OE);
      data += OE_offset;   //讀出來的值需要offset
      break;
    case UserAddr_P3P:
      data = EEP_Read_Data_u8(SPIAddr_P3P);
      break;
    case UserAddr_O3:
      data = EEP_Read_Data_u8(SPIAddr_O3);
      data += O3_offset;   //讀出來的值需要offset
      break;
    case UserAddr_P4P:
      data = EEP_Read_Data_u8(SPIAddr_P4P);
      break;
    case UserAddr_O4:
      data = EEP_Read_Data_u8(SPIAddr_O4);
      data += O4_offset;   //讀出來的值需要offset
      break;
    case UserAddr_OdS:
      data = EEP_Read_Data_u8(SPIAddr_OdS);
      break;
    case UserAddr_AC:
      data = EEP_Read_Data_u8(SPIAddr_AC);
      break;
    case UserAddr_rtr:
      data = EEP_Read_Data_u8(SPIAddr_rtr);
      break;
    case UserAddr_CCt:
      data = EEP_Read_Data_u8(SPIAddr_CCt);
      break;
    case UserAddr_CCS:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_CCS_L);
      data += CCS_offset;   //讀出來的值需要offset
      break;
    case UserAddr_COn:
      data = EEP_Read_Data_u8(SPIAddr_COn);
      break;
    case UserAddr_COF:
      data = EEP_Read_Data_u8(SPIAddr_COF);
      break;

    case UserAddr_CF:
      data = EEP_Read_Data_u8(SPIAddr_CF);
      break;
    case UserAddr_rES:
      data = EEP_Read_Data_u8(SPIAddr_rES);
      break;
    case UserAddr_Lod:
      data = EEP_Read_Data_u8(SPIAddr_Lod);
      break;
    case UserAddr_rEd:
      data = EEP_Read_Data_u8(SPIAddr_rEd);
      break;
    case UserAddr_dLY:
      data = EEP_Read_Data_u8(SPIAddr_dLY);
      break;
    case UserAddr_dtr:
      data = EEP_Read_Data_u8(SPIAddr_dtr);
      break;
      
    case UserAddr_tdF:
      data = EEP_Read_Data_u8(SPIAddr_tdF);
      break;
    case UserAddr_dFP:
      data = EEP_Read_Data_u8(SPIAddr_dFP);
      break;
    case UserAddr_dtE:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_dtE_L);
      data += dtE_offset;   //讀出來的值需要offset
      break;
    case UserAddr_IdF:
      data = EEP_Read_Data_u8(SPIAddr_IdF);
      break;
    case UserAddr_MdF:
      data = EEP_Read_Data_u8(SPIAddr_MdF);
      break;
    case UserAddr_dSd:
      data = EEP_Read_Data_u8(SPIAddr_dSd);
      break;
    case UserAddr_dFd:
      data = EEP_Read_Data_u8(SPIAddr_dFd);
      break;
    case UserAddr_dAd:
      data = EEP_Read_Data_u8(SPIAddr_dAd);
      break;
    case UserAddr_Fdt:
      data = EEP_Read_Data_u8(SPIAddr_Fdt);
      break;
    case UserAddr_dPo:
      data = EEP_Read_Data_u8(SPIAddr_dPo);
      break;
    case UserAddr_dAF:
      data = EEP_Read_Data_u8(SPIAddr_dAF);
      break;

    case UserAddr_FnC:
      data = EEP_Read_Data_u8(SPIAddr_FnC);
      break;
    case UserAddr_Fnd:
      data = EEP_Read_Data_u8(SPIAddr_Fnd);
      break;      
    case UserAddr_Fct:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_Fct_L);
      break;
    case UserAddr_FSt:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_FSt_L);
      data += FSt_offset;   //讀出來的值需要offset
      break;
    case UserAddr_Fon:
      data = EEP_Read_Data_u8(SPIAddr_Fon);
      break;
    case UserAddr_FoF:
      data = EEP_Read_Data_u8(SPIAddr_FoF);
      break;
    case UserAddr_FAP:
      data = EEP_Read_Data_u8(SPIAddr_FAP);
      break;

    case UserAddr_ALC:
      data = EEP_Read_Data_u8(SPIAddr_ALC);
      break;
    case UserAddr_ALU:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_ALU_L);
      break;
    case UserAddr_ALL:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_ALL_L);
      data += ALL_offset;   //讀出來的值需要offset
      break;
    case UserAddr_AFH:
      data = EEP_Read_Data_u8(SPIAddr_AFH);
      break;
    case UserAddr_ALd:
      data = EEP_Read_Data_u8(SPIAddr_ALd);
      break;
    case UserAddr_dAO:
      data = EEP_Read_Data_u8(SPIAddr_dAO);
      break;

    case UserAddr_AP2:
      data = EEP_Read_Data_u8(SPIAddr_AP2);
      break;
    case UserAddr_AL2:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_AL2_L);
      data += AL2_offset;   //讀出來的值需要offset
      break;
    case UserAddr_Au2:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_Au2_L);
      data += Au2_offset;   //讀出來的值需要offset
      break;
    case UserAddr_AH2:
      data = EEP_Read_Data_u8(SPIAddr_AH2);
      break;
    case UserAddr_Ad2:
      data = EEP_Read_Data_u8(SPIAddr_Ad2);
      break;
    case UserAddr_dA2:
      data = EEP_Read_Data_u8(SPIAddr_dA2);
      break;
    case UserAddr_bLL:
      data = EEP_Read_Data_u8(SPIAddr_bLL);
      break;
    case UserAddr_AC2:
      data = EEP_Read_Data_u8(SPIAddr_AC2);
      break;

    case UserAddr_i1P:
      data = EEP_Read_Data_u8(SPIAddr_i1P);
      break;
    case UserAddr_i1F:
      data = EEP_Read_Data_u8(SPIAddr_i1F);
      break;
    case UserAddr_did:
      data = EEP_Read_Data_u8(SPIAddr_did);
      break;
    case UserAddr_nPS:
      data = EEP_Read_Data_u8(SPIAddr_nPS);
      break;
    case UserAddr_odc:
      data = EEP_Read_Data_u8(SPIAddr_odc);
      break;
    case UserAddr_rrd:
      data = EEP_Read_Data_u8(SPIAddr_rrd);
      break;
    case UserAddr_HES:
      data = EEP_Read_Data_u8Convertu16(SPIAddr_HES_L);
      data += HES_offset;   //讀出來的值需要offset
      break;
      
    case UserAddr_Adr:
      data = EEP_Read_Data_u8(SPIAddr_Adr);
      break;
    case UserAddr_PbC:
      data = EEP_Read_Data_u8(SPIAddr_PbC);
      break;
    case UserAddr_onF:
      data = EEP_Read_Data_u8(SPIAddr_onF);
      break;

      default:
      break;
  }
    return data;
}

static void EEP_Write_Data_u8_write(uint8_t addr, uint8_t* value)
{
  I2C_EE_BufferWrite(value, addr , 1);
}

static void EEP_Write_Data_u16Convertu8(uint8_t addr, uint16_t value)
{
  uint8_t SPI_u8Table[4];

  SPI_u8Table[0]   = (value)    & 0xFF;
  SPI_u8Table[1]   = (value>>8) & 0xFF;

  // SPI_EEP_WriteData(addr, SPI_u8Table[0]);
  I2C_EE_BufferWrite(&SPI_u8Table[0], addr , 1);
  addr++;
  // SPI_EEP_WriteData(addr, SPI_u8Table[1]);
  I2C_EE_BufferWrite(&SPI_u8Table[1], addr , 1);
}

static void EEP_Write_API(uint8_t addr, int32_t value)
{
  switch (addr)
  {
    case UserAddr_EEPWrite_times:
      EEP_Write_Data_u8_write(SPIAddr_EEPWrite_times, (uint8_t*)value);
      break;
    case UserAddr_Set:
      EEP_Write_Data_u16Convertu8(SPIAddr_Set_L, value);
      break;
    case UserAddr_Hy:
      EEP_Write_Data_u16Convertu8(SPIAddr_Hy, value);
      break;
    case UserAddr_LS:
      EEP_Write_Data_u16Convertu8(SPIAddr_LS_L, value);
      break;
    case UserAddr_US:
      EEP_Write_Data_u16Convertu8(SPIAddr_US_L, value);
      break;
    case UserAddr_Ot:
      value -= Ot_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_Ot, value);
      break;
    case UserAddr_P2P:
      EEP_Write_Data_u16Convertu8(SPIAddr_P2P, value);
      break;
    case UserAddr_OE:
      value -= OE_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_OE, value);
      break;
    case UserAddr_P3P:
      EEP_Write_Data_u16Convertu8(SPIAddr_P3P, value);
      break;
    case UserAddr_O3:
      value -= O3_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_O3, value);
      break;
    case UserAddr_P4P:
      EEP_Write_Data_u16Convertu8(SPIAddr_P4P, value);
      break;
    case UserAddr_O4:
      value -= O4_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_O4, value);
      break;
    case UserAddr_OdS:
      EEP_Write_Data_u16Convertu8(SPIAddr_OdS, value);
      break;
    case UserAddr_AC:
      EEP_Write_Data_u16Convertu8(SPIAddr_AC, value);
      break;
    case UserAddr_rtr:
      EEP_Write_Data_u16Convertu8(SPIAddr_rtr, value);
      break;
    case UserAddr_CCt:
      EEP_Write_Data_u16Convertu8(SPIAddr_CCt, value);
      break;
    case UserAddr_CCS:
      value -= CCS_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_CCS_L, value);
      break;
    case UserAddr_COn:
      EEP_Write_Data_u16Convertu8(SPIAddr_COn, value);
      break;
    case UserAddr_COF:
      EEP_Write_Data_u16Convertu8(SPIAddr_COF, value);
      break;
    case UserAddr_CF:
      EEP_Write_Data_u16Convertu8(SPIAddr_CF, value);
      break;
    case UserAddr_rES:
      EEP_Write_Data_u16Convertu8(SPIAddr_rES, value);
      break;
    case UserAddr_Lod:
      EEP_Write_Data_u16Convertu8(SPIAddr_Lod, value);
      break;
    case UserAddr_rEd:
      EEP_Write_Data_u16Convertu8(SPIAddr_rEd, value);
      break;
    case UserAddr_dLY:
      EEP_Write_Data_u16Convertu8(SPIAddr_dLY, value);
      break;
    case UserAddr_dtr:
      EEP_Write_Data_u16Convertu8(SPIAddr_dtr, value);
      break;
    case UserAddr_tdF:
      EEP_Write_Data_u16Convertu8(SPIAddr_tdF, value);
      break;
    case UserAddr_dFP:
      EEP_Write_Data_u16Convertu8(SPIAddr_dFP, value);
      break;
    case UserAddr_dtE:
      value -= dtE_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_dtE_L, value);
      break;
    case UserAddr_IdF:
      EEP_Write_Data_u16Convertu8(SPIAddr_IdF, value);
      break;
    case UserAddr_MdF:
      EEP_Write_Data_u16Convertu8(SPIAddr_MdF, value);
      break;
    case UserAddr_dSd:
      EEP_Write_Data_u16Convertu8(SPIAddr_dSd, value);
      break;
    case UserAddr_dFd:
      EEP_Write_Data_u16Convertu8(SPIAddr_dFd, value);
      break;
    case UserAddr_dAd:
      EEP_Write_Data_u16Convertu8(SPIAddr_dAd, value);
      break;
    case UserAddr_Fdt:
      EEP_Write_Data_u16Convertu8(SPIAddr_Fdt, value);
      break;
    case UserAddr_dPo:
      EEP_Write_Data_u16Convertu8(SPIAddr_dPo, value);
      break;
    case UserAddr_dAF:
      EEP_Write_Data_u16Convertu8(SPIAddr_dAF, value);
      break;
    case UserAddr_FnC:
      EEP_Write_Data_u16Convertu8(SPIAddr_FnC, value);
      break;
    case UserAddr_Fnd:
      EEP_Write_Data_u16Convertu8(SPIAddr_Fnd, value);
      break;
    case UserAddr_Fct:
      EEP_Write_Data_u16Convertu8(SPIAddr_Fct_L, value);
      break;
    case UserAddr_FSt:
      value -= FSt_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_FSt_L, value);
      break;
    case UserAddr_Fon:
      EEP_Write_Data_u16Convertu8(SPIAddr_Fon, value);
      break;
    case UserAddr_FoF:
      EEP_Write_Data_u16Convertu8(SPIAddr_FoF, value);
      break;
    case UserAddr_FAP:
      EEP_Write_Data_u16Convertu8(SPIAddr_FAP, value);
      break;
    case UserAddr_ALC:
      EEP_Write_Data_u16Convertu8(SPIAddr_ALC, value);
      break;
    case UserAddr_ALU:
      EEP_Write_Data_u16Convertu8(SPIAddr_ALU_L, value);
      break;
    case UserAddr_ALL:
      value -= ALL_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_ALL_L, value);
      break;
    case UserAddr_AFH:
      EEP_Write_Data_u16Convertu8(SPIAddr_AFH, value);
      break;
    case UserAddr_ALd:
      EEP_Write_Data_u16Convertu8(SPIAddr_ALd, value);
      break;
    case UserAddr_dAO:
      EEP_Write_Data_u16Convertu8(SPIAddr_dAO, value);
      break;
    case UserAddr_AP2:
      EEP_Write_Data_u16Convertu8(SPIAddr_AP2, value);
      break;
    case UserAddr_AL2:
      value -= AL2_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_AL2_L, value);
      break;
    case UserAddr_Au2:
      value -= Au2_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_Au2_L, value);
      break;
    case UserAddr_AH2:
      EEP_Write_Data_u16Convertu8(SPIAddr_AH2, value);
      break;
    case UserAddr_Ad2:
      EEP_Write_Data_u16Convertu8(SPIAddr_Ad2, value);
      break;
    case UserAddr_dA2:
      EEP_Write_Data_u16Convertu8(SPIAddr_dA2, value);
      break;
    case UserAddr_bLL:
      EEP_Write_Data_u16Convertu8(SPIAddr_bLL, value);
      break;
    case UserAddr_AC2:
      EEP_Write_Data_u16Convertu8(SPIAddr_AC2, value);
      break;
    case UserAddr_i1P:
      EEP_Write_Data_u16Convertu8(SPIAddr_i1P, value);
      break;
    case UserAddr_i1F:
      EEP_Write_Data_u16Convertu8(SPIAddr_i1F, value);
      break;
    case UserAddr_did:
      EEP_Write_Data_u16Convertu8(SPIAddr_did, value);
      break;
    case UserAddr_nPS:
      EEP_Write_Data_u16Convertu8(SPIAddr_nPS, value);
      break;
    case UserAddr_odc:
      EEP_Write_Data_u16Convertu8(SPIAddr_odc, value);
      break;
    case UserAddr_rrd:
      EEP_Write_Data_u16Convertu8(SPIAddr_rrd, value);
      break;
    case UserAddr_HES:
      value -= HES_offset;   //寫進去前需要offset
      EEP_Write_Data_u16Convertu8(SPIAddr_HES_L, value);
      break;
    case UserAddr_Adr:
      EEP_Write_Data_u16Convertu8(SPIAddr_Adr, value);
      break;
    case UserAddr_PbC:
      EEP_Write_Data_u16Convertu8(SPIAddr_PbC, value);
      break;
    case UserAddr_onF:
      EEP_Write_Data_u16Convertu8(SPIAddr_onF, value);
      break;

      default:
      break;
  }
  // I2C_EE_BufferWrite( I2c_Buf_Write, 0x00 , DATA_Size);
}

/* Function definitions ------------------------------------------------------*/
void EEP_Read_PWRON(void)
{
  System.eep.IsEEPROMWrited = EEP_Read_API(UserAddr_EEPWrite_times);
  System.set = EEP_Read_API(UserAddr_Set);
  System.hy = EEP_Read_API(UserAddr_Hy);
  System.ls = EEP_Read_API(UserAddr_LS);
}

void EEP_Write_PWRON(void)
{

}

void EEP_ResetALL(void)
{
  // 後面的數字帶入出廠預設值, 不一定每個參數都要reset
  EEP_Write_API(UserAddr_Set, -50);
  EEP_Write_API(UserAddr_Hy, 20);
  EEP_Write_API(UserAddr_LS, -50);
  EEP_Write_API(UserAddr_US, 110);
  EEP_Write_API(UserAddr_Ot, 0);
  EEP_Write_API(UserAddr_P2P, 0);
  EEP_Write_API(UserAddr_OE, 0);
}

void IsMCUneedRST(bool reset)
{
  //自動檢查是否為第一次使用mcu, 或是外部設定reset參數
  if(reset)
    EEP_Write_API(UserAddr_EEPWrite_times, 0);

  //IsEEPROMWrited
  System.eep.IsEEPROMWrited = EEP_Read_API(UserAddr_EEPWrite_times);
  if(System.eep.IsEEPROMWrited != 1)
    EEP_ResetALL();
}




