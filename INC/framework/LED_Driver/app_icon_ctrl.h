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

/* Public function protocol -----------------------------------------------*/
void ALL_LED_ON(void);
void ALL_LED_OFF(void);

void ICON_Refrigerate_ON(void);
void ICON_Refrigerate_OFF(void);
void ICON_Refrigerate_Flashing(void);

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

void ICON_Energy_Saving_ON(void);
void ICON_Energy_Saving_OFF(void);

void ICON_Bulb_ON(void);
void ICON_Bulb_OFF(void);

void ICON_Aux_ON(void);
void ICON_Aux_OFF(void);

void ICON_Clock_ON(void);
void ICON_Clock_OFF(void);

#endif /* INC_APP_LED_CTRL_H_ */
