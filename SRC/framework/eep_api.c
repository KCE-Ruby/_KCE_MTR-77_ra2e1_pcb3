// /**
//  ******************************************************************************
//  * @file    eep_api.c
//  * @author  KCE R&D Team
//  * @date    2024-09-09
//  * @author  Ruby
//  * 所有跟eeprom相關的應用程式碼
//  * 資料長度為2個byte, 所以要做再處理才能直接用API讀寫一個參數
//  * @copyright Copyright © 2024 KCE. All rights reserved.
//  ******************************************************************************
//  */

// /* Private includes ----------------------------------------------------------*/
// #include "INC/board_interface/board_layer.h"
// #include "INC/framework/eep_api.h"

// /* extern variables -----------------------------------------------------------------*/
// extern __IO s_Var System;

// /* variables -----------------------------------------------------------------*/


// /* Static Function definitions ------------------------------------------------------*/
// static uint32_t EEP_Read_API(uint8_t addr)
// {
//   uint32_t data;

//   switch (addr)
//   {
//     case UserAddr_EEPWrite_page:
//       data = EEP_Read_Data_u8(SPIAddr_EEPWrite_page);
//       break;
//     case UserAddr_EEPWrite_times:
//       data = EEP_Read_Data_u8Convertu32(SPIAddr_EEPWrite_times_LL);
//       break;
//   }
//     return data;
//   I2C_EE_BufferRead(I2c_Buf_Read, 0x00 , DATA_Size);
// }

// static void EEP_Write_API(uint8_t addr, uint32_t value)
// {
//   I2C_EE_BufferWrite( I2c_Buf_Write, 0x00 , DATA_Size);
// }

// /* Function definitions ------------------------------------------------------*/
// void EEP_Read_PWRON(void)
// {
  
// }

// void EEP_Write_PWRON(void)
// {

// }

// void EEP_ResetALL(void)
// {

// }

// void IsMCUneedRST(bool reset)
// {
//   //自動檢查是否為第一次使用mcu, 或是外部設定reset參數
//   // if(reset)
//   //   EEP_Write_API(UserAddr_EEPWrite_times, 0);

//   // //IsEEPROMWrited
//   // sysCfg.sysVar.EEPWrite_times = EEP_Read_API(UserAddr_EEPWrite_times);
//   // if(System.eep.IsEEPROMWrited != EEP_written)
//   //   EEP_ResetALL();
// }




