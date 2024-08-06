/**
 ******************************************************************************
 * @file    board_layer.c
 * @author  KCE R&D Team
 * @date    2024-07-19
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 * 
 * callback code by RA Liabary or the funciton that everywhere can use
 * Watchdog
 * Timer
 * Uart
 * I2c(eeprom)
 * RS485
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"

#define Timer_Limit_Counter_Max  (600000) //10min; 600s

/* variables -----------------------------------------------------------------*/
static bool scan_complete_flag = false;
__IO r_tmr tmr;

/* Global Function definitions ------------------------------------------------------*/
//watchdog
void WDT_init(void)
{
  fsp_err_t status;

  //如果使用J-Link调试器进行调试的话需要加上这一句话
  R_DEBUG->DBGSTOPCR_b.DBGSTOP_WDT = 0;

  status = R_WDT_Open(&g_wdt0_ctrl, &g_wdt0_cfg);
  while(status!=FSP_SUCCESS);

  /*刷新看门狗计数器，在这里的作用是初次启动寄存模式下的看门狗计数器
   *要注意，除非是在刷新允许的范围内，否则自启动模式下不应该使用该函数*/
  status = R_WDT_Refresh(&g_wdt0_ctrl);
  while(status!=FSP_SUCCESS);
}

void WDT_Feed(void)
{
   R_WDT_Refresh(&g_wdt0_ctrl);
}

//uart
fsp_err_t uart_init(void)
{
  fsp_err_t err;
  err = R_SCI_UART_Open(&g_uart9_ctrl, &g_uart9_cfg);
  if (FSP_SUCCESS != err)
  {
      // 处理错误
      return err;
  }
  return FSP_SUCCESS;
}

fsp_err_t uart_send(uint8_t *p_data, uint32_t length)
{
  fsp_err_t err;

  err = R_SCI_UART_Write(&g_uart9_ctrl, p_data, length);
  if (FSP_SUCCESS != err)
  {
      // 处理错误
      return err;
  }
  return FSP_SUCCESS;
}

fsp_err_t uart_receive(uint8_t *p_data, uint32_t length)
{
  fsp_err_t err;

  err = R_SCI_UART_Read(&g_uart9_ctrl, p_data, length);
  if (FSP_SUCCESS != err)
  {
      // 处理错误
      return err;
  }
  return FSP_SUCCESS;
}

//ADC
bool get_scanflag(void)
{
  return scan_complete_flag;
}

void set_scanflag(bool flag)
{
  scan_complete_flag = flag;
}

/* callback Function definitions by code generated ------------------------------------------------------*/
void r_wdt_callback(wdt_callback_args_t * p_args)
{
   /*防止编译器产生关于函数中没有使用形参的警告*/
   (void) p_args;

  /* TODO: 这里应该放最重要的函数，比如保存重要数据等 */
   R_BSP_SoftwareDelay(3, BSP_DELAY_UNITS_SECONDS);

   /* 通过软件复位MCU*/
   __NVIC_SystemReset();
}

void timer0_callback(timer_callback_args_t *p_args)
{
  if (TIMER_EVENT_CYCLE_END == p_args->event)
  {
    tmr.Cnt_1ms++;
    
    if((tmr.Cnt_1ms%1)==0) tmr.Flag_5ms = true;
    if((tmr.Cnt_1ms%100)==0) tmr.Flag_100ms = true;
    if((tmr.Cnt_1ms%500)==0) tmr.Flag_500ms = true;
    if((tmr.Cnt_1ms%2000)==0) tmr.Flag_2s = true;
    if((tmr.Cnt_1ms%100)==0) tmr.Cnt_1s++;

    tmr.Cnt_1ms = (tmr.Cnt_1ms>Timer_Limit_Counter_Max)? 1:tmr.Cnt_1ms;
  }
}

void user_adc_callback(adc_callback_args_t * p_args)
{
  scan_complete_flag = false;
  FSP_PARAMETER_NOT_USED(p_args);
  scan_complete_flag = true;
}

