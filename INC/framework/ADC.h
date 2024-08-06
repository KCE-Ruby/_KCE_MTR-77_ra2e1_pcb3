/**
 ******************************************************************************
 * @file    ADC.h
 * @author  KCE R&D Team
 * @date    2024-06-10
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_ADC_H_ 
#ifdef INC_ADC_H_

/* typedef definitions -----------------------------------------------------*/
typedef struct  //different timer flags
{
  int16_t sensor1;
  int16_t sensor2;
  int16_t sensor3;
  int16_t sensor4;
  float temp;
}ADC_TemperatureValue;

typedef struct
{
  float temperature;
  int adValue;
} TemperatureAdTable;

/* task function protocol -----------------------------------------------*/
void ADC_Main(void);
void r_adc_callback(adc_callback_args_t * p_args);

#endif /* INC_ADC_H_ */
