/**
 ******************************************************************************
 * @file    Display.h
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_DISPLAY_H_ 
#ifdef INC_DISPLAY_H_

/* Private includes ----------------------------------------------------------*/
#include "INC/framework/LED_Driver/app_icon_ctrl.h"
#include "INC/framework/LED_Driver/Indicator_encode.h"

/* Private define ----------------------------------------------------------*/
#define SMG_1_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_00, BSP_IO_LEVEL_LOW)
#define SMG_2_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_01, BSP_IO_LEVEL_LOW)
#define SMG_3_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_LOW)
#define SMG_4_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_03, BSP_IO_LEVEL_LOW)
#define SMG_5_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_13, BSP_IO_LEVEL_LOW)
#define SMG_6_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_07, BSP_IO_LEVEL_LOW)

#define SMG_1_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_00, BSP_IO_LEVEL_HIGH)
#define SMG_2_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_01, BSP_IO_LEVEL_HIGH)
#define SMG_3_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_HIGH)
#define SMG_4_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_03, BSP_IO_LEVEL_HIGH)
#define SMG_5_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_13, BSP_IO_LEVEL_HIGH)
#define SMG_6_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_07, BSP_IO_LEVEL_HIGH)

#define SMG_A_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_15, BSP_IO_LEVEL_HIGH)
#define SMG_B_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_09, BSP_IO_LEVEL_HIGH)
#define SMG_C_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_00, BSP_IO_LEVEL_HIGH)
#define SMG_D_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_12, BSP_IO_LEVEL_HIGH)
#define SMG_E_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, BSP_IO_LEVEL_HIGH)
#define SMG_F_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_08, BSP_IO_LEVEL_HIGH)
#define SMG_G_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_14, BSP_IO_LEVEL_HIGH)
#define SMG_DP_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, BSP_IO_LEVEL_HIGH)

#define SMG_A_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_15, BSP_IO_LEVEL_LOW)
#define SMG_B_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_09, BSP_IO_LEVEL_LOW)
#define SMG_C_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_00, BSP_IO_LEVEL_LOW)
#define SMG_D_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_12, BSP_IO_LEVEL_LOW)
#define SMG_E_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, BSP_IO_LEVEL_LOW)
#define SMG_F_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_08, BSP_IO_LEVEL_LOW)
#define SMG_G_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_14, BSP_IO_LEVEL_LOW)
#define SMG_DP_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, BSP_IO_LEVEL_LOW)

/* function protocol -----------------------------------------------*/
void LED_Display(void);

#endif /* INC_DISPLAY_H_ */
