/**
 ******************************************************************************
 * @file    board_layer.h
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_BOARD_LAYER_H_ 
#ifdef INC_BOARD_LAYER_H_

#include "hal_data.h"
#include "debug_uart/bsp_debug_uart.h"
#include "eeprom/i2c_ee.h"

/* Enum type definitions -----------------------------------------------------*/


/* typedef definitions -----------------------------------------------------*/
typedef struct  //different timer flags
{
  bool Flag_1ms, Flag_5ms, Flag_500ms, Flag_1s, Flag_2s;
  int16_t Cnt_1s;
  uint32_t Cnt_1ms;
}r_tmr;

/* task function protocol -----------------------------------------------*/
void WDT_init(void);
void WDT_Feed(void);
fsp_err_t uart_init(void);
fsp_err_t uart_send(uint8_t *p_data, uint32_t length);
fsp_err_t uart_receive(uint8_t *p_data, uint32_t length);

#endif /* INC_BOARD_LAYER_H_ */
