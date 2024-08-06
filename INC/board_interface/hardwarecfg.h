/**
 ******************************************************************************
 * @file    hardwarecfg.h
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_HARDWARECFG_H_ 
#ifdef INC_HARDWARECFG_H_

/* Macro definitions ---------------------------------------------------------*/
#ifdef HARDWARE_INTERFACE
// RESET 10K
// VCL 4.7UF
// VCC +5V
// GND
// There are two 74HC595 for display and key scan, the one for display(DATA), also for key, the other one for scan 7-seg group
// 74HC595-1, DATA & KEY  DATA(a=Q1, b=Q0, c=Q3, d=Q6, eQ5, f=Q4, g=Q3, dp=Q2) KEY(SET=Q1, SHIFT=Q3, UP=Q5, DOWN=Q4)
// 74HC595-2, SCAN GROUP(S1=Q5, S2=Q6, S3=Q7, S4=Q1, S5=Q4)
#endif

#ifdef DEBUG_JLINK_SEGGER_PIN
  #define SWDIO         BSP_IO_PORT_01_PIN_08
  #define SWCLK         BSP_IO_PORT_03_PIN_00
#endif

/* Enum type definitions -----------------------------------------------------*/


/* typedef definitions -----------------------------------------------------*/


/* task function protocol -----------------------------------------------*/


#endif /* INC_HARDWARECFG_H_ */
