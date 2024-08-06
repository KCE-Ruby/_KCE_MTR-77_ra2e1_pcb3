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

/* variables -----------------------------------------------------------------*/
uint32_t Device_Version;

/* Private function protocol -----------------------------------------------*/
static void boot_init(void);
static void TMR_init(void);
static void ADC_init(void);

/* Function definitions ------------------------------------------------------*/
void Task_Main(void)
{
  const uint8_t Release = 0x00;
  const uint8_t dev     = 0x00;
  const uint8_t test    = 0x01;
  Device_Version = Release*65536 + dev*256 + test;

  boot_init();
  // uart_test();
  printf("初始化完成\r\n");
  printf("軟體版本: %lu\r\n", Device_Version);
  while(1)
  {
    I2C_EE_Writedrase();
    if (I2C_Test() ==1)
    {
      // LED3_OFF;
      // R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
    } 
    else 
    {
      // LED1_ON;
      // R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);
    }
    // if(tmr.Flag_500ms)
    // {
    //   ADC_Main();
    //   tmr.Flag_500ms = false;
    // }
    // else
    // {
    //    Key_main();
    //   // ShowNumber(tmr.Cnt_1s);
    //   // LED_Display_main();
    //   // tmr.Cnt_1s = (tmr.Cnt_1s>999)? -999:tmr.Cnt_1s;
    // }

    WDT_Feed();
    R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
  }
}

/* Static Function definitions ------------------------------------------------------*/
static void boot_init(void)
{
  WDT_init();   //16384 cycle watchdog
  TMR_init();   //1ms Timer
  Debug_UART1_Init();
  // ADC_init();
  I2C_EE_Init();
}

static void TMR_init(void)
{
  fsp_err_t err = FSP_SUCCESS;
  /* Initializes the module. */
  err = R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);
  /* Handle any errors. This function should be defined by the user. */
  assert(FSP_SUCCESS == err);
  /* Start the timer. */
  (void) R_GPT_Start(&g_timer0_ctrl);
}

static void ADC_init(void)
{
  fsp_err_t status;
  status = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
  status = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
  assert(FSP_SUCCESS == status);
}


// static void uart_test(void)
// {
//   // 初始化 UART
//   if (uart_init() != FSP_SUCCESS)
//   {
//       // 处理初始化失败
//       while (1);
//   }

//   // 示例：发送数据
//   uint8_t tx_data[] = "Hello, UART!";
//   uart_send(tx_data, sizeof(tx_data));

//   // 示例：接收数据
//   uint8_t rx_data[UART_BUFFER_SIZE];
//   uart_receive(rx_data, sizeof(rx_data));
// }