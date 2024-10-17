/**
 ******************************************************************************
 * @file    app_icon_ctrl.h
 * @author  KCE R&D Team
 * @date    2024-06-26
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_APP_LED_CTRL_H_ 
#ifdef INC_APP_LED_CTRL_H_


/* Enum type definitions -----------------------------------------------------*/
enum ICON_API_STATE
{
  ICON_OFF,
  ICON_ON,
  ICON_FLASHING,
};

enum Enhanced_Cooling_TYPE
{
  //先開一個uint8_t用, 所有可能會讓壓縮機開關的條件都在這, 兩兩一組
  /*縮寫解釋
  * enhanced_cooling -> EC, 加強製冷
  */
  icon_none = 0,
  icon_EC_enable     = 0x01,     //第1個bit 啟動要用 0b0000 0001(0x01)
  icon_EC_disable    = 0xFE,     //第1個bit 清除要用 &1111 1110(0xFE)
};


/* typedef definitions -----------------------------------------------------*/
typedef struct
{
  uint8_t Refrigerate_sta;
  uint8_t Enhanced_Cooling_sta;
} icon_api_flag;


/* Public function protocol -----------------------------------------------*/
void ALL_LED_ON(void);
void ALL_LED_OFF(void);

uint8_t ICON_LeaveSet_Flashing(uint8_t flash_cnt);

void ICON_Refrigerate_ON(void);
void ICON_Refrigerate_OFF(void);
void ICON_Refrigerate_Flashing(void);
void ICON_Refrigerate_API(void);

void ICON_Defrost_ON(void);
void ICON_Defrost_OFF(void);
void ICON_Defrost_Flashing(void);

void ICON_Fan_ON(void);
void ICON_Fan_OFF(void);
void ICON_Fan_Flashing(void);

void ICON_degrees_Celsius_ON(void);
void ICON_degrees_Celsius_OFF(void);
void ICON_degrees_Fahrenheit_ON(void);
void ICON_degrees_Fahrenheit_OFF(void);
void ICON_degrees_Flashing(void);
void ICON_degrees_API(void);

void ICON_Alarm_ON(void);
void ICON_Alarm_OFF(void);

void ICON_Enhanced_Cooling_ON(void);
void ICON_Enhanced_Cooling_OFF(void);
void ICON_Enhanced_Cooling_API(void);

void ICON_Energy_Saving_ON(void);
void ICON_Energy_Saving_OFF(void);

void ICON_Bulb_ON(void);
void ICON_Bulb_OFF(void);

void ICON_Aux_ON(void);
void ICON_Aux_OFF(void);

void ICON_Clock_ON(void);
void ICON_Clock_OFF(void);

bool rStToDisplay_Flashing(void);
void P1ToDisplay_Flashing(void);
#endif /* INC_APP_LED_CTRL_H_ */
