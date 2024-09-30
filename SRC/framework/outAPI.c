/*
  應用端策略邏輯(儀表總功能紀錄)
  * 輸出控制

  1. 負載輸出邏輯
  2. 面板操作邏輯
  3. 燈號顯示邏輯(配合面板)
  4. 儀表內運算邏輯(不含顯示)
  5. 主要功能說明
  6. 儀表內參數說明
  7. 輸入端邏輯
  8. TTL串口監控系統(RS485)
  9. 編程鑰匙(參數透過key輸入)
  10.警報類型說明
  11.參數出廠設定值(是否與預設值相同待確認)
*/

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/LED_Driver/app_icon_ctrl.h"
#include "INC/framework/LED_Driver/app_menu_ctrl.h"
#include "INC/framework/datapool.h"
#include "INC/framework/outAPI.h"

/* Private defines ----------------------------------------------------------*/
#define NC_control            (0)   //(normal close, default)
#define NO_control            (1)   //(normal open)

/* extern variables -----------------------------------------------------------------*/
extern __IO s_Var System;


bool manual_defrost(bool flag)
{
  if(System.pv < System.value[dtE])
    out3_Defrost_on();
  else
  {
    flag = false;
    out3_Defrost_off();
  }
  return flag;
}

/* Function definitions ------------------------------------------------------*/
void out1_FAN_on(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_HIGH);
}

void out1_FAN_off(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_LOW);
}

void out2_Compressor_on(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_HIGH);
}

void out2_Compressor_off(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_LOW);
}

void out3_Defrost_on(void)
{
  bool init_type = false;
  if(System.value[tdF] == type_EL) init_type = NC_control;
  else if(System.value[tdF] == type_in) init_type = NO_control;

  ICON_Defrost_ON();
  if(init_type == NC_control)
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_02, BSP_IO_LEVEL_HIGH);
  else if(init_type == NO_control)
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_02, BSP_IO_LEVEL_LOW);
}

void out3_Defrost_off(void)
{
  bool init_type = false;
  if(System.value[tdF] == type_EL) init_type = NC_control;
  else if(System.value[tdF] == type_in) init_type = NO_control;
  
  ICON_Defrost_OFF();
  if(init_type == NC_control)
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_02, BSP_IO_LEVEL_LOW);
  else if(init_type == NO_control)
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_02, BSP_IO_LEVEL_HIGH);
}

void out4_buzzer_on(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_HIGH);
}

void out4_buzzer_off(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_LOW);
}

/* Output Logical Function definitions ------------------------------------------------------*/
void Out_main(void)
{
  int8_t sv = (System.value[Set]+System.value[Set]);
  if(System.pv < sv)
  {
    out2_Compressor_on();
  }
  else if(System.pv <= System.value[Set])
  {
    out2_Compressor_off();
  }
}








