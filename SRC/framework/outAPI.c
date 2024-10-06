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

/* Private function protocol -----------------------------------------------*/
static void out1_FAN_on(void);
static void out1_FAN_off(void);
static void out2_Compressor_on(void);
static void out2_Compressor_off(void);
static void out3_Defrost_on(void);
static void out3_Defrost_off(void);
static void out4_buzzer_on(void);
static void out4_buzzer_off(void);
static void out_alloff(void);

/* static output logic function api protocol---------------------------------*/
static void out_alloff(void);


/* Function definitions ------------------------------------------------------*/
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

/* static Logic API funcitons ------------------------------------------------------*/
static void out1_FAN_on(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_HIGH);
}

static void out1_FAN_off(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_LOW);
}

static void out2_Compressor_on(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_HIGH);
}

static void out2_Compressor_off(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_LOW);
}

static void out3_Defrost_on(void)
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

static void out3_Defrost_off(void)
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

static void out4_buzzer_on(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_HIGH);
}

static void out4_buzzer_off(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_LOW);
}

static void out_alloff(void)
{
  out1_FAN_off();
  out2_Compressor_off();
  out3_Defrost_off();
  out4_buzzer_off();
}

/* static output logic function api ------------------------------------------------------*/
static bool ods_logic(void)
{
  /* 
  * 上電延遲, 在這段時間內任何輸出都維持在未通電狀態
  * 上電後只會執行一次, 當ret轉成true後不會再執行此含式
  * 由於System.value是小數後一位被放大10倍的值, 所以要除回去才是正確的整數值
  * OdS單位為分鐘, 需要做個小轉換變成ms做delay
  * 如果Ods有值才需要做delay的判斷
  */
  static bool ret=false;
  const uint8_t min2sec = 60;
  const uint16_t sec = (System.value[OdS]/10)*min2sec;

  if((ret==false) && (sec>0))
  {
    ret = Mydelay_sec(dly_OdS, sec);
    // printf("System.value[OdS] = %d\r\n", System.value[OdS]);
    // printf("sec = %d\r\n", sec);
    // printf("ret = %d\r\n", ret);
  }
  else
    ret = true;

  return ret;
}

/* Output Logical Function definitions ------------------------------------------------------*/
void Out_main(void)
{
  /*
  * 首先確認是否有上電延遲需求, 否則
  */
  int8_t sv = (System.value[Set]+System.value[Hy]);
  if(ods_logic() == true)
  {
    if(System.pv > sv)
    {
      out2_Compressor_on();
      // printf("System.pv = %d\r\n", System.pv);
      // printf("sv = %d\r\n", sv);
      // printf("開啟壓縮機輸出\r\n\n\n");
    }
    else if(System.pv <= System.value[Set])
    {
      out2_Compressor_off();
      // printf("System.pv = %d\r\n", System.pv);
      // printf("sv = %d\r\n", sv);
      // printf("關閉壓縮機輸出\r\n\n\n");
    }
  }
  else
  {
    out_alloff();
    // printf("關閉所有輸出邏輯\r\n");
  }
}








