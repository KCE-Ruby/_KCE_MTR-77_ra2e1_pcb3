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

/*---------------------- Private includes ----------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/ADC.h"

#define FILTER_SIZE                 (5)
#define TABLE_SIZE                  (10)
#define ERROR_AD                    (-999)

/*---------------------- extern variables ----------------------*/
extern __IO bool scan_complete_flag;

/*---------------------- variables ----------------------*/
static uint16_t AD[5];
static ADC_TemperatureValue TempValue;
static TemperatureAdTable table[TABLE_SIZE] = 
{
  { -40.0, 100 },
  { -20.0, 200 },
  { 0.0, 300 },
  { 20.0, 400 },
  { 40.0, 500 },
  { 60.0, 600 },
  { 80.0, 700 },
  { 100.0, 800 },
  { 120.0, 900 },
  { 140.0, 1000 }
};

/*---------------------- Private function protocol ----------------------*/
static uint16_t Read_ADC_Voltage_Value(adc_channel_t const reg_id);

//raw data process and filter
static int compare(const void *a, const void *b);
static int medianFilter(int new_ad_value);
static uint16_t getFiltedAD(adc_channel_t const channel);

//Convert AD to NTC temperature value
static int16_t converADtoNTC_multiply10(uint16_t adValue);

//Convert AD to NTC temperature value
static int16_t ADCtest(adc_channel_t const channel);

/*---------------------- Function definitions ----------------------*/
void ADC_Main(void)
{
  // TempValue.sensor1 = converADtoNTC_multiply10(getFiltedAD(ADC_CHANNEL_1));
  // R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);

  // TempValue.sensor2 = converADtoNTC_multiply10(getFiltedAD(ADC_CHANNEL_2));
  // R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);

  //AD convert Test API
  TempValue.sensor1 = (ADCtest(ADC_CHANNEL_1));
  R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);
  TempValue.sensor1 = (ADCtest(ADC_CHANNEL_2));
  R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);
}

/*---------------------- Static Function definitions ----------------------*/
//raw data process and filter
static uint16_t Read_ADC_Voltage_Value(adc_channel_t const reg_id)
{
  uint16_t adc_data;
  bool flag = false;

  (void)R_ADC_ScanStart(&g_adc0_ctrl);
  while (!get_scanflag()) //等待转换完成标志
  {
    ;
  }
  set_scanflag(flag); //重新清除标志位

  /* 读取通道0数据 */
  R_ADC_Read(&g_adc0_ctrl, reg_id, &adc_data);

  return adc_data;
}

static int compare(const void *a, const void *b)
{
  return (*(int*)a - *(int*)b);
}

static int medianFilter(int new_ad_value)
{
  static int ad_values[FILTER_SIZE];
  static int ad_index = 0;

  ad_values[ad_index] = new_ad_value;
  ad_index = (ad_index + 1) % FILTER_SIZE;

  int sorted_values[FILTER_SIZE];
  for (int i = 0; i < FILTER_SIZE; i++) {
    sorted_values[i] = ad_values[i];
  }

  qsort(sorted_values, FILTER_SIZE, sizeof(int), compare);

  return sorted_values[FILTER_SIZE / 2];
}

static uint16_t getFiltedAD(adc_channel_t const channel)
{
  switch (channel)
  {
    case ADC_CHANNEL_1:
      AD[1] = Read_ADC_Voltage_Value(ADC_CHANNEL_1);
      medianFilter(AD[1]);
      return AD[1];
      break;

    case ADC_CHANNEL_2:
      AD[2] = Read_ADC_Voltage_Value(ADC_CHANNEL_2);
      medianFilter(AD[2]);
      return AD[2];
      break;
  
    default:
      return 0;
      break;
  }
}

//Convert AD to NTC temperature value
static int16_t converADtoNTC_multiply10(uint16_t adValue)
{
  int16_t ret;

  for (int i = 0; i < TABLE_SIZE - 1; i++) {
    if (adValue >= table[i].adValue && adValue <= table[i + 1].adValue) {
      // 线性插值计算
      float ratio = (float)((adValue - table[i].adValue) / (table[i + 1].adValue - table[i].adValue));
      ret = (int16_t)((table[i].temperature + ratio * (table[i + 1].temperature - table[i].temperature))*10);
      return ret;
    }
  }
  return ERROR_AD; // 返回-999表示未找到匹配的AD值
}




/*---------------------- Test Function definitions ----------------------*/
static int16_t ADCtest(adc_channel_t const channel)
{
  uint16_t ADfortest = 0;
  ADfortest = (channel == 1 || channel == 2) ? getFiltedAD(channel) : 0;

  int16_t temperature = converADtoNTC_multiply10(ADfortest);
  if (temperature != ERROR_AD) {
      printf("AD Value: %d, Temperature: %.2f\n", ADfortest, (float)temperature/10);
  } else {
      printf("AD Value: %d not found in table.\n", ADfortest);
  }
  return temperature;
}

