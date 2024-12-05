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
#include "INC/framework/datapool.h"
#include "INC/framework/eep_api.h"

/* Private defines ----------------------------------------------------------*/

/* extern variables -----------------------------------------------------------------*/
extern __IO s_Var Syscfg, Preload;
extern __IO uint16_t rsttable[End];
extern __IO uint8_t bytetable_pr1[End];
extern __IO SYSTEM_TABLE systable[End];

/* variables -----------------------------------------------------------------*/
/*eeprom內各參數用途解釋
* u8_eep_write: 拆成byte單位用來reset的陣列,
* 參數來源: datapool.c內的原廠設定陣列bytetable[End].DefaultValue
* 
* u8_eep_Read: 讀出eeprom內的值, 原型為byte類型
* 組裝後成為u16_eep_Read, 才可轉換成看得懂的參數使用 (讀出為放大10倍後的數值)
* 開機後讀出的數值, 會再轉換成系統值, 不會被改變, 隨時可確保讀出時數值的正確性
* 
* u16_eep_write原始的參數值, 錶可直接顯示出來的數值, (放大10倍後的數值)
* 需要經過轉換成為u8_eep_write才可寫入使用
*/
// uint8_t u8_eep_Read[], u8_eep_write[];
int16_t u16_eep_Read[End], u16_eep_write[End];
__IO uint8_t eeplength;
__IO uint8_t Preload_value[2];


//EEPROM_TEST用
uint16_t start_addr = 0x00, end_addr = 0xFF;
uint8_t length = 8, tdata[10];
extern uint8_t EE_Buf_Read[SPIAddr_End];
extern uint8_t I2c_Buf_Reset[SPIAddr_End];

/* Static Function definitions ------------------------------------------------------*/
static void eep_read_all(void);

/* Function definitions ------------------------------------------------------*/
void EEP_Read_PWRON(void)
{
  uint8_t addr[SPIAddr_RecordHigh_H], data[SPIAddr_End], length = SPIAddr_End;
  uint8_t u8_eep_Read[SPIAddr_End]={};

  printf("[eep_api] 開始讀出原始值\r\n");
  I2C_EE_BufferRead(u8_eep_Read, 0x00, length);
  R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);
  printf("[eep_api] 組裝系統值中\r\n");
  // for(int i=0; i<10; i++)
  // {
  //   printf("讀出[%d] addr=0x%x, data=0x%x\r\n",i, i, I2c_Buf_Read[i]);
  // }
  printf("[eep_api] 開機讀eeprom參數值程序完成\r\n");
}

void EEP_Write_PWROFF(void)
{
  /*關機前要寫入的eeprom值
  * addr & data都代表是24c02內的對應值,
  * 特別是data是組裝前的值, 需要再經過處理,
  * uint8_t 為1個byte, 直接讀出,
  * uint16_t 為2的byte, 由L/H 拼裝而成,
  * uint32_t 為4個byte, 由LL/LH/HL/HH 拼裝而成, (這個case沒有)
  * 參數1, 陣列直接帶入若參數則須取地址(&),
  * 參數2, 為要寫入的起始addr,
  * 參數3, 為要寫入的長度
  */
  uint8_t addr[SPIAddr_RecordHigh_H], data[SPIAddr_End];

  printf("開始寫入\r\n");
  I2C_EE_BufferWrite(data, SPIAddr_Start, 0x04);
  R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);
  // for(int i=5; i<9; i++)
  // {
  //   printf("寫入[%d] addr=0x%x, data=0x%x\r\n",i, i, data[i]);
  // }
}

void EEP_ResetALL(void)
{
  /*重置錶頭參數(恢復原廠設定)
  * 參數來源: datapool.c內的原廠設定陣列bytetable[End].DefaultValue
  * 存入的參數因為會有小數點, 所以全部放大10倍
  * 存入前需要先拆成uint8_t, uint16_t, uint32_t三種長度對應addr
  * 拆除後陣列: I2c_Buf_Reset[SPIAddr_End]
  */

  uint8_t i=Str, addr=SPIAddr_Start;
  // uint8_t I2c_Buf_Reset[SPIAddr_End];
  uint8_t length = SPIAddr_onF;

  while(i < UserAddr_onF)
  {
    if((addr==SPIAddr_Start)||(addr==SPIAddr_P2P)||(addr==SPIAddr_P3P)||(addr==SPIAddr_P4P)||(addr==SPIAddr_CF)|| \
      (addr==SPIAddr_rES)||(addr==SPIAddr_Lod)||(addr==SPIAddr_rEd)||(addr==SPIAddr_tdF)|| \
      (addr==SPIAddr_dFP)||(addr==SPIAddr_dFd)||(addr==SPIAddr_dPo)||(addr==SPIAddr_FnC)|| \
      (addr==SPIAddr_Fnd)||(addr==SPIAddr_FAP)||(addr==SPIAddr_ALC)||(addr==SPIAddr_AP2)|| \
      (addr==SPIAddr_bLL)||(addr==SPIAddr_AC2)||(addr==SPIAddr_i1P)||(addr==SPIAddr_i1F)|| \
      (addr==SPIAddr_odc)||(addr==SPIAddr_rrd)||(addr==SPIAddr_PbC)||(addr==SPIAddr_onF)
      )
    {
      //把RST內uint8_t的值直接放入一格陣列
      I2c_Buf_Reset[addr] = rsttable[i];
      addr++;
    }
    else
    {
      //把RST內uint16_t的值拆成2個byte放入陣列
      I2c_Buf_Reset[addr+1] = rsttable[i] >> 8;    //取高位元
      I2c_Buf_Reset[addr] = rsttable[i] & 0xff;    //取低位元
      addr+=2;
    }
    i++;
    WDT_Feed();
  }
  printf("恢復原廠參數值\r\n");

  //寫入原廠設定
  I2C_EE_BufferWrite(I2c_Buf_Reset, 0x00, length);

}

void IsMCUneedRST(bool reset)
{
  //自動檢查是否為第一次使用mcu, 或是外部設定reset參數
  // if(reset)
  //   EEP_Write_API(UserAddr_EEPWrite_times, 0);

  // //IsEEPROMWrited
  // sysCfg.sysVar.EEPWrite_times = EEP_Read_API(UserAddr_EEPWrite_times);
  // if(Syscfg.eep.IsEEPROMWrited != EEP_written)
  //   EEP_ResetALL();
}

uint8_t check_index_for_eep(uint8_t ind)
{
  uint8_t i=0, addr=0, next_addr=0;
  uint8_t sub_index = bytetable_pr1[Syscfg.level1_index];
  uint16_t remake;

  remake = (uint16_t)(Preload.value[sub_index] - systable[sub_index].Range_Low);
  while(i < UserAddr_onF)
  {
    addr = next_addr;   //紀錄當前真實的位置
    if((addr==SPIAddr_Start)||(addr==SPIAddr_P2P)||(addr==SPIAddr_P3P)||(addr==SPIAddr_P4P)||(addr==SPIAddr_CF)|| \
      (addr==SPIAddr_rES)||(addr==SPIAddr_Lod)||(addr==SPIAddr_rEd)||(addr==SPIAddr_tdF)|| \
      (addr==SPIAddr_dFP)||(addr==SPIAddr_dFd)||(addr==SPIAddr_dPo)||(addr==SPIAddr_FnC)|| \
      (addr==SPIAddr_Fnd)||(addr==SPIAddr_FAP)||(addr==SPIAddr_ALC)||(addr==SPIAddr_AP2)|| \
      (addr==SPIAddr_bLL)||(addr==SPIAddr_AC2)||(addr==SPIAddr_i1P)||(addr==SPIAddr_i1F)|| \
      (addr==SPIAddr_odc)||(addr==SPIAddr_rrd)||(addr==SPIAddr_PbC)||(addr==SPIAddr_onF)
      )
    {
      //把RST內uint8_t的值直接放入一格陣列
      Preload_value[0] = remake;
      next_addr++;
      eeplength = 1;
    }
    else
    {
      //把RST內uint16_t的值拆成2個byte放入陣列
      Preload_value[1] = remake >> 8;    //取高位元
      Preload_value[0] = remake & 0xff;    //取低位元
      next_addr+=2;
      eeplength = 2;
    }
    if(ind==i)
    {
      i = UserAddr_onF;
      return addr;
    }
    i++;
    WDT_Feed();
  }
}

uint8_t EE_Buf_test[255];
void EEPROM_TEST(void)
{
  System_Init();

  tdata[0] = 0x18;
  tdata[1] = 0x27;
  tdata[2] = 0x36;
  tdata[3] = 0x45;
  tdata[4] = 0x54;
  tdata[5] = 0x63;
  tdata[6] = 0x72;
  tdata[7] = 0x81;

  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  UserTabletoSytem();
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  EEP_ResetALL();
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  // printf("EEP_ResetALL\r\n");
  // R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  I2C_EE_BufferRead(EE_Buf_Read, 0x00, 255);
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  // printf("I2C_EE_BufferRead\r\n");
  // R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  offset_EEtoSYS();
  WDT_Feed();

  while (1)
  {
    WDT_Feed();
    printf("while(1)\r\n");
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
  }
}

unsigned char DATA_Size_test = 250;
unsigned char I2c_Buf_Write_test[255] = {};
unsigned char I2c_Buf_Read_test[255] = {};
uint8_t I2C_Test_1(void)
{
  uint16_t i;
  // unsigned char DATA_Size_test = 30;
  // unsigned char I2c_Buf_Write_test[33] = {};
  // unsigned char I2c_Buf_Read_test[33] = {};

  System_Init();
  printf("測試開始\r\n");

  //将I2c_Buf_Write中顺序递增的数据写入EERPOM中
  printf("寫入的數據_繁體\r\n");
  for ( i=0; i<DATA_Size_test; i++ ) { //填充缓冲
      I2c_Buf_Write_test[i] =i+5;
      printf("0x%02X ", I2c_Buf_Write_test[i]);
      if (i%16 == 15)
          printf("\n");
  }
  I2C_EE_BufferWrite( I2c_Buf_Write_test, 0x00 , DATA_Size_test);



  //将EEPROM读出数据顺序保持到I2c_Buf_Read中
  printf("\r\n讀出的數據_繁體\n");
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  I2C_EE_BufferRead(I2c_Buf_Read_test, 0x00 , DATA_Size_test);


  //将I2c_Buf_Read中的数据通过串口打印
  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
  for (i=0; i<DATA_Size_test; i++) {
      if (I2c_Buf_Read_test[i] != I2c_Buf_Write_test[i]) {
          printf("0x%02X \n", I2c_Buf_Read_test[i]);
          printf("錯誤:I2C EEPROM寫入與讀出的數據不一致_繁體\n");
          printf("%d\n",i);
          return 0;
      }
      printf("0x%02X ", I2c_Buf_Read_test[i]);
      if (i%16 == 15)
          printf("\n");

  }
  printf("\r\nI2C(AT24C02)讀寫測試成功_繁體\n");
  return 1;
}