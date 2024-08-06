/**
 ******************************************************************************
 * @file    Logic_Manager.h
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_LOGIC_MANGER_H_ 
#ifdef INC_LOGIC_MANGER_H_

#define OUT_1_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_HIGH)
#define OUT_2_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_HIGH)
#define OUT_3_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_02, BSP_IO_LEVEL_HIGH)
#define OUT_4_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_HIGH)

#define OUT_1_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_LOW)
#define OUT_2_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_LOW)
#define OUT_3_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_02, BSP_IO_LEVEL_LOW)
#define OUT_4_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_LOW)

/* task function protocol -----------------------------------------------*/
void Task_Main(void);

#endif /* INC_LOGIC_MANGER_H_ */
