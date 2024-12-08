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

#define ERROR_AD                           (-999)

/* Enum type definitions -----------------------------------------------------*/
enum API_STATE
{
  //所有按鍵共用一次只能執行一件事情
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

enum SYSTEM_MODE
{
  homeMode,
  recordMode,
  level1Mode,
  level2Mode,
  checkgMode,
  settingMode,
};

enum DELAY_TYPE_min
{
  dly_CCt,
  dly_COn,
  dly_COF,

  dly_end_min
};

enum DELAY_TYPE_sec
{
  dly_OdS,
  dly_AC,

  dly_end_sec
};

enum DELAY_TYPE_ms
{
  dly_fHigh,
  dly_fLow,
  dly_fLeave,

  dly_end_ms
};

/* typedef definitions -----------------------------------------------------*/
typedef enum {
  DECIMAL_AT_2 = 2, // 2.34 (不可能會有, 除非修改rES參數範圍)
  DECIMAL_AT_1 = 1, // 23.4
  DECIMAL_AT_0 = 0  // 234 (无小数点)
} decimalIndex;

typedef struct  //different timer flags
{
  bool FlashFlag_1Hz, FlashFlag_2Hz;
  bool Flag_1ms, Flag_100ms;
  bool Flag_1s;
  uint8_t COM_Port, COM_Port_pre;
  int16_t Cnt_1s, Cnt_60s;
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
  bool Max_flag, Min_flag, clear_flag;
  uint16_t index;
}f_menu;

typedef struct  //different timer flags
{
  //非初始化參數值
  int16_t RecordHigh, RecordLow;
  uint8_t mode;                       //顯示目前的模式, 首頁/設定模式
  int8_t level1_index, level2_index;  //用戶層的參數名稱指標
  int16_t pv_disp, pv;

  //其他
  int16_t value[78];            //大小為[End], 各參數內的數值, 有負數沒有小數, 以整數儲存
  // int16_t pv_disp, pv, history_max, history_min; //3位數值, 含負數
  uint8_t decimalIndex;         //顯示的小數點位
  s_EEPROM eep;                 //有被存入EEPROM的參數們
  f_menu keymode;               //按鍵切換旗標用
}s_Var;

typedef struct  //different timer flags
{
  bool Defrost;
  uint8_t Level1_value;
  bool Level2_value;
  bool leaveSet;
  bool Vvalue_int;
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
bool Mydelay_min(uint8_t flag, uint32_t min);
bool Mydelay_sec(uint8_t flag, uint16_t sec);
bool Mydelay_ms(uint8_t flag, uint16_t ms);
#endif /* INC_BOARD_LAYER_H_ */
