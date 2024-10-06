/**
 ******************************************************************************
 * @file    outAPI.h
 * @author  KCE R&D Team
 * @date    2024-09-05
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_OUTAPI_H_ 
#ifdef INC_OUTAPI_H_

/* typedef definitions -----------------------------------------------------*/
typedef struct  //different timer flags
{
  int16_t sensor1;  //已放大十倍成整數型態(可負數))
  int16_t sensor2;
  int16_t sensor3;
  int16_t sensor4;
  float temp;
}ADC_TemperatureValue;

typedef struct
{
  int16_t temperature;
  uint16_t adValue;
} TemperatureAdTable;

/* task function protocol -----------------------------------------------*/
bool manual_defrost(bool flag);
void Out_main(void);

#endif /* INC_OUTAPI_H_ */
