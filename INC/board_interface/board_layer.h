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
enum
{
  homeMode,
  menuMode,
  settingMode,
};

/* typedef definitions -----------------------------------------------------*/
typedef enum {
  DECIMAL_AT_2 = 2, // 2.34
  DECIMAL_AT_1 = 1, // 23.4
  DECIMAL_AT_0 = 0  // 234 (无小数点)
} decimalIndex;

typedef struct  //different timer flags
{
  bool Flag_1ms, Flag_5ms,Flag_100ms, Flag_200ms, Flag_500ms, Flag_1s, Flag_2s;
  uint8_t COM_Port, COM_Port_pre;
  int16_t Cnt_1s;
  uint32_t Cnt_1ms;
}r_tmr;

typedef struct
{
  uint8_t Max_Pv, Min_Pv;
}s_EEPROM;

typedef struct  //different timer flags
{
  uint8_t mode;                 //顯示目前的模式, 首頁/設定模式
  uint8_t decimalIndex;         //顯示的小數點位
  s_EEPROM eep;
  uint8_t history_max, history_min;
}s_Var;

/* task function protocol -----------------------------------------------*/
void System_Init(void);
void WDT_Feed(void);
fsp_err_t uart_send(uint8_t *p_data, uint32_t length);
fsp_err_t uart_receive(uint8_t *p_data, uint32_t length);
bool get_scanflag(void);
void set_scanflag(bool flag);
void SMG_OPEN(uint8_t smg);
void SMG_CLOSE(uint8_t smg);
void Key_ReadPin(void);
#endif /* INC_BOARD_LAYER_H_ */
