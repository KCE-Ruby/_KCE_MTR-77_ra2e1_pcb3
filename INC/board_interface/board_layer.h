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
enum API_STATE
{
  //一次只能執行一件事情
  API_FREE = 0,
  API_BUSY1 = 1,
  API_BUSY2 = 2,
  API_BUSY3 = 3,
  API_BUSY4 = 4,
  API_BUSY5 = 5,
};

enum DISPLAY_SPECIAL
{
  notshow = 10,
  CLEARALL=19999, //清除所有數字區顯示
};

enum
{
  homeMode,
  historyMode,
  level1Mode,
  level2Mode,
  checkgMode,
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
  bool FlashFlag_1Hz, FlashFlag_2Hz;
  bool Flag_1ms, Flag_5ms,Flag_100ms, Flag_200ms, Flag_500ms, Flag_1s, Flag_2s;
  uint8_t COM_Port, COM_Port_pre;
  int16_t Cnt_1s;
  uint32_t Cnt_1ms;
}r_tmr;

typedef struct
{
  bool IsEEPROMWrited;
  uint8_t Max_Pv, Min_Pv;
  int16_t set;
}s_EEPROM;

typedef struct
{
  bool SET_value_flag;
  bool Max_flag, Min_flag;
  uint16_t index;
}f_menu;

typedef struct  //different timer flags
{
  int16_t value[74];            //大小為[End], 各參數內的數值, 有負數沒有小數, 以整數儲存
  int16_t pv, history_max, history_min; //3位數值, 含負數
  uint8_t mode;                 //顯示目前的模式, 首頁/設定模式
  uint8_t decimalIndex;         //顯示的小數點位
  s_EEPROM eep;                 //有被存入EEPROM的參數們
  int8_t level1_index, level2_index; //用戶層的參數名稱指標
  f_menu keymode;               //按鍵切換旗標用
}s_Var;

typedef struct  //different timer flags
{
  bool Defrost;
  uint8_t Level1_value;
  bool Level2_value;
  bool leaveSet;
}s_Flag;

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
