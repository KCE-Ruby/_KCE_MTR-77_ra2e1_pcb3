/**
 ******************************************************************************
 * @file    ADC.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/ADC.h"

/* extern variables -----------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/
volatile bool scan_complete_flag;
__IO double AD[5];

/* Private function protocol -----------------------------------------------*/
static double Read_ADC_Voltage_Value(adc_channel_t const reg_id);

/* Function definitions ------------------------------------------------------*/
void ADC_Main(void)
{
//  AD[0] = Read_ADC_Voltage_Value(ADC_CHANNEL_0);
//  R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
  AD[1] = Read_ADC_Voltage_Value(ADC_CHANNEL_1);
  R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
  AD[2] = Read_ADC_Voltage_Value(ADC_CHANNEL_2);
  R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
//  AD[2] = Read_ADC_Voltage_Value(ADC_CHANNEL_5);
//  R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
//  AD[3] = Read_ADC_Voltage_Value(ADC_CHANNEL_6);
//  R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
//  AD[4] = Read_ADC_Voltage_Value(ADC_CHANNEL_8);
}

void user_adc_callback(adc_callback_args_t * p_args)
{
  scan_complete_flag = false;
  FSP_PARAMETER_NOT_USED(p_args);
  scan_complete_flag = true;
}

/* Static Function definitions ------------------------------------------------------*/
/* 进行ADC采集，读取ADC数据并转换结果 */
static double Read_ADC_Voltage_Value(adc_channel_t const reg_id)
{
  uint16_t adc_data;
  double a0;

  (void)R_ADC_ScanStart(&g_adc0_ctrl);
  while (!scan_complete_flag) //等待转换完成标志
  {
    ;
  }
  scan_complete_flag = false; //重新清除标志位

  /* 读取通道0数据 */
  R_ADC_Read(&g_adc0_ctrl, reg_id, &adc_data);
  /* ADC原始数据转换为电压值（ADC参考电压为3.3V） */
  // a0 = (double)(adc_data*3.3/4095);

  return adc_data;
}
