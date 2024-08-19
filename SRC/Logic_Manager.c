/**
 ******************************************************************************
 * @file    Logic_Manager.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/Display.h"
#include "INC/framework/ADC.h"
#include "INC/framework/Key.h"
#include "INC/Logic_Manager.h"

//for test
#define UART_BUFFER_SIZE 256
// static void uart_test(void);

/* extern variables -----------------------------------------------------------------*/
extern r_tmr tmr;
extern volatile uint8_t data;
extern ADC_TemperatureValue TempValue;
extern __IO s_Var System;

/* variables -----------------------------------------------------------------*/
uint32_t Device_Version;
bool _loop_flag = false;

/* Private function protocol -----------------------------------------------*/


/* Function definitions ------------------------------------------------------*/
void Task_Main(void)
{
  // static bool _loop_flag = false;

  const uint8_t Release = 0x00;
  const uint8_t dev     = 0x00;
  const uint8_t test    = 0x09;
  Device_Version = Release*65536 + dev*256 + test;

  boot_init();
  printf("初始化完成\r\n");
  printf("軟體版本: %lu\r\n", Device_Version);
  I2C_EE_Writederase();
  I2C_Test();
  // NumToDisplay(798);
  CharToDisplay(LS);
  while(1)
  {
    // _loop_flag = tmr.Flag_200ms;
    // if(_loop_flag)
    // {
    //   if(System.mode != homeMode)
    //   {
    //     // CharToDisplay(LS);
    //     NumToDisplay(123);
    //   }
    //   else
    //   {
    //     NumToDisplay(000);
    //     // ADC_Main();
    //   }

    //   _loop_flag = false;
    // }
    LED_Display();
    tmr.Cnt_1s = (tmr.Cnt_1s>999)? -999:tmr.Cnt_1s;

    WDT_Feed();
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MICROSECONDS);
  }
}

/* Static Function definitions ------------------------------------------------------*/

