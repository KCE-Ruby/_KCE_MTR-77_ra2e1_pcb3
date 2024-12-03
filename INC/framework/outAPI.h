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

/* Enum type definitions -----------------------------------------------------*/
enum OUT_ACTION
{
  act_off = 0,
  act_on = 1,
};

enum COMPRESSOR_TYPE
{
  //先開一個uint8_t用, 所有可能會讓壓縮機開關的條件都在這, 兩兩一組
  /*縮寫解釋
  * enhanced_cooling -> EC, 加強製冷
  * normal_refrigerate -> NR, 普通製冷
  */
  out_none = 0,
  out_EC_enable     = 0x01,     //第1個bit 啟動要用 0b0000 0001(0x01)
  out_EC_disable    = 0xFE,     //第1個bit 清除要用 &1111 1110(0xFE)
  out_NR_enable     = 0x02,     //第2個bit 啟動要用 0b0000 0010(0x02)
  out_NR_disable    = 0xFD,     //第2個bit 清除要用 &1111 1101(0xFD)
  out_COn_enable    = 0x04,     //第3個bit
  out_COn_disable   = 0xFB,     //第3個bit
  out_COF_enable    = 0x08,     //第4個bit
  out_COF_disable   = 0xF7,     //第4個bit
};

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

typedef struct
{
  uint8_t fan;
  uint8_t compressor;
  uint8_t defrost;
  uint8_t buzzer;
} out_api_flag;
/* task function protocol -----------------------------------------------*/

void Out_main(void);

#endif /* INC_OUTAPI_H_ */
