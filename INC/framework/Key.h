/**
 ******************************************************************************
 * @file    Key.h
 * @author  KCE R&D Team
 * @date    2024-06-26
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_KEY_H_ 
#ifdef INC_KEY_H_

#define Key_Read R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_14, &KeyPin)
/* Enum type definitions -----------------------------------------------------*/

/* task function protocol -----------------------------------------------*/
void Key_main(void);

#endif /* INC_KEY_H_ */
