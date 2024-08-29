/**
 ******************************************************************************
 * @file    datapool.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 * 程式碼的數據轉換區
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/datapool.h"
#include "INC/framework/LED_Driver/app_menu_ctrl.h"

static ByteSettingTable bytetable[End] = 
{
  //參數字元, 上限值, 下限值, 預設值, 權限層
  {Set,     System.ls,     System.us,     -5.0,     NaN},
  { Hy,           0.1,          25.5,      2.0,     Pr1},
  { LS,         -50.0,    System.set,     -5.0,     Pr2},
  { US,    System.set,           110,      110,     Pr2},

};

