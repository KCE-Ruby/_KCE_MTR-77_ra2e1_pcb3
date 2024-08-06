/**
 ******************************************************************************
 * @file    ADC.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 * 100ms取一次AD值, 取5次, 一秒更新兩次溫度值
 ******************************************************************************
 */

/*---------------------- Private includes ----------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/ADC.h"

#define FILTER_SIZE                 (9)
#define TABLE_SIZE                  (12)
#define ERROR_AD                    (-999)

/*---------------------- extern variables ----------------------*/
extern __IO bool scan_complete_flag;

/*---------------------- variables ----------------------*/
uint16_t AD[5];
ADC_TemperatureValue TempValue;
static TemperatureAdTable table[TABLE_SIZE] = 
{
  { -10.0, 3658 },
  { 0.0, 3452 },
  { 10.0, 3189 },
  { 20.0, 2880 },
  { 30.0, 2542 },
  { 40.0, 2186 },
  { 50.0, 1840 },
  { 60.0, 1520 },
  { 70.0, 1246 },
  { 80.0, 1005 },
  { 90.0, 794 },
  { 100.0, 655 }
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

uint16_t test;
/*---------------------- Function definitions ----------------------*/
void ADC_Main(void)
{
  test = getFiltedAD(ADC_CHANNEL_1);
  TempValue.sensor1 = converADtoNTC_multiply10(getFiltedAD(ADC_CHANNEL_1));
  R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);

  // TempValue.sensor2 = converADtoNTC_multiply10(getFiltedAD(ADC_CHANNEL_2));
  // R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);

  //AD convert Test API
    // AD[1] = Read_ADC_Voltage_Value(ADC_CHANNEL_1);
  // TempValue.sensor1 = (ADCtest(ADC_CHANNEL_1));
  // R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);
  // TempValue.sensor2 = (ADCtest(ADC_CHANNEL_2));
  // R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MICROSECONDS);
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
  bool sort_flag=false;

  // Store new value and update index
  ad_values[ad_index] = new_ad_value;
  ad_index = (ad_index + 1) % FILTER_SIZE;

  // Set sort flag if we've filled the buffer
  if (ad_index == 0) {
    sort_flag = true;
  }

  static int sorted_values[FILTER_SIZE];
  // Copy and sort the array if sort_flag is set
  if (sort_flag)
  {
    for (int i = 0; i < FILTER_SIZE; i++) {
      sorted_values[i] = ad_values[i];
    }
    qsort(sorted_values, FILTER_SIZE, sizeof(int), compare);
  }

  // Return the median value
  return sorted_values[FILTER_SIZE / 2];
}

static uint16_t getFiltedAD(adc_channel_t const channel)
{
  switch (channel)
  {
    case ADC_CHANNEL_1:
      AD[1] = Read_ADC_Voltage_Value(ADC_CHANNEL_1);
      return medianFilter(AD[1]);
      break;

    case ADC_CHANNEL_2:
      AD[2] = Read_ADC_Voltage_Value(ADC_CHANNEL_2);
      return medianFilter(AD[2]);
      break;
  
    default:
      return 0;
      break;
  }
}

//Convert AD to NTC temperature value
static int16_t converADtoNTC_multiply10(uint16_t adValue)
{
  int16_t ret = ERROR_AD; // 初始化为错误值

  for (int i = 0; i < TABLE_SIZE - 1; i++)
  {
    uint16_t adValueLow = table[i].adValue;
    uint16_t adValueHigh = table[i + 1].adValue;

    if (adValue >= adValueLow && adValue <= adValueHigh)
    {
      // 计算差值
      int16_t tempDiff = table[i + 1].temperature - table[i].temperature;
      uint16_t adDiff = adValueHigh - adValueLow;
      uint16_t adOffset = adValue - adValueLow;

      // 线性插值计算并乘以10
      ret = (int16_t)((table[i].temperature * 10) + (tempDiff * adOffset * 10) / adDiff);
      break;
    }
  }

  return ret;
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

