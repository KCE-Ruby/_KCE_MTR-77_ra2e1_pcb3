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
extern __IO icon_api_flag icon;
extern __IO ADC_TemperatureValue TempValue;
extern __IO uint16_t catch_min[dly_end_min];
extern __IO uint16_t catch_s[dly_end_sec];
extern __IO bool bootled_En;

/* static variables -----------------------------------------------------------------*/
static out_api_flag out_sta;

/* Private function protocol -----------------------------------------------*/
static void out1_FAN_on(void);
static void out1_FAN_off(void);
static void out1_FAN_api(void);
static void out2_Compressor_on(void);
static void out2_Compressor_off(void);
static void out2_Compressor_api(void);
static void out3_Defrost_on(void);
static void out3_Defrost_off(void);
static void out3_Defrost_api(void);
static void out4_buzzer_on(void);
static void out4_buzzer_off(void);
static void out4_buzzer_api(void);
static void out_alloff(void);

/* static output logic function api protocol---------------------------------*/
static bool ac_logic(bool act);
static bool ods_logic(void);
static bool defrost_logic(void);
static void cct_logic(void);
static void concof_logic(void);
static void normal_refrigerate_logic(void);

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

static void out1_FAN_api(void)
{
  switch (out_sta.fan)
  {
    case true:
      out1_FAN_on();
      break;
    case false:
      out1_FAN_off();
      break;
    
    default:
      break;
  }
}

static void out2_Compressor_on(void)
{
  /* 
  * 控制壓縮機輸出, 若輸入ON代表溫度值到點了
  * 但實際輸出需依據AC參數做防頻繁啟動的限制
  * 每次壓縮機被關閉後, 旗標需要被reset
  */
  if(ac_logic(act_on) == true)
  {
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_HIGH);
  }
}

static void out2_Compressor_off(void)
{
  //壓縮機off時, 不須delay且重置旗標為false
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_LOW);
  ac_logic(act_off);
}

static void out2_Compressor_api(void)
{
  switch (out_sta.compressor)
  {
    case out_none:
      out2_Compressor_off();
      break;
    
    default:
      out2_Compressor_on();
      break;
  }
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

static void out3_Defrost_api(void)
{
  switch (out_sta.defrost)
  {
    case true:
      out3_Defrost_on();
      break;
    case false:
      out3_Defrost_off();
      break;
    
    default:
      break;
  }
}

static void out4_buzzer_on(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_HIGH);
}

static void out4_buzzer_off(void)
{
  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_15, BSP_IO_LEVEL_LOW);
}

static void out4_buzzer_api(void)
{
  switch (out_sta.buzzer)
  {
    case true:
      out4_buzzer_on();
      break;
    case false:
      out4_buzzer_off();
      break;
    
    default:
      break;
  }
}

static void out_alloff(void)
{
  out_sta.fan = false;
  out_sta.compressor = false;
  out_sta.defrost = false;
  out_sta.buzzer = false;
}

/* static output logic function api ------------------------------------------------------*/
static bool ac_logic(bool act)
{
  /* 防頻繁啟動延時
  * 保證壓縮機從關機到開機的最短間隔時間
  * 每次壓縮機被關閉後, 旗標需要被reset
  * 由於System.value是小數後一位被放大10倍的值, 所以要除回去才是正確的整數值
  * AC單位為分鐘, 需要做個小轉換變成ms做delay
  * 如果AC有值才需要做delay的判斷
  */
  static bool ret=false;
  const uint8_t min2sec = 60;
  const uint16_t sec = (System.value[AC]/10)*min2sec;

  //壓縮機off時, 不須delay且reset ret旗標為false
  if(act == act_off)
    return ret=false;

  //當壓縮機on時, 開始計時
  if((ret==false) && (sec>0))
  {
    icon.Refrigerate_sta = ICON_FLASHING;
    ret = Mydelay_sec(dly_AC, sec);
    // printf("System.value[AC] = %d\r\n", System.value[AC]);
    // printf("sec = %d\r\n", sec);
    // printf("ret = %d\r\n", ret);
  }
  else
    ret = true;

  return ret;
}

static bool ods_logic(void)
{
  /* 上電延遲
  * 在這段時間內任何輸出都維持在未通電狀態
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

static bool defrost_logic(void)
{
  //TODO: 融霜邏輯
  return 0;
}

static void cct_logic(void)
{
  /* 強冷凍循環
  * 確認不在融霜狀態, 且使用者是否啟動強冷循環模式
  * 若大於等於CCS加速冷凍設定點, 則依據CCt時間持續啟動壓縮機
  * 若CCt的時間到了但是系統還是沒降到CCS的設定點, 離開模式且關閉壓縮機
  * 若低於設定值, 則不動作(依照普通製冷邏輯控制壓縮機輸出)
  * 若低於設定值, 重置計時器的狀態(可有可無)
  * 回傳true代表CCt強冷功能啟動中計時未完成,  反之false代表未啟動強冷功能
  */
  const bool CCS_sta = (System.pv >= System.value[CCS])? true:false;
  const uint8_t CCt_hour = (System.value[CCt]/10);
  uint8_t CCt_min = (System.value[CCt]%10) * 10;
  uint8_t CCt_sum = CCt_min + (CCt_hour*60);  //總合成分鐘數
  static bool dly = true;

  // printf("CCt_hour = %d\r\n", CCt_hour);
  // printf("CCt_min = %d\r\n", CCt_min);
  // printf("CCt_sum = %d\r\n", CCt_sum);
  if((defrost_logic()==false) && (icon.Enhanced_Cooling_sta!=icon_none))
  {
    // printf("CCt_處於強冷模式\r\n");
    // printf("CCS_sta = %d\r\n, CCS_sta");
    if(CCS_sta)
    {
      dly = Mydelay_min(dly_CCt, CCt_sum);
      if(dly==false)
      {
        out_sta.compressor |= out_EC_enable;
        // printf("CCS_開啟壓縮機輸出 = %d\r\n", out_sta.compressor);
      }
      else  //時間到後, 系統還是沒降到CCS的設定點, 關閉壓縮機且離開模式
      {
        out_sta.compressor &= out_EC_disable;
        icon.Enhanced_Cooling_sta &= icon_EC_disable;
        // printf("CCt_關閉壓縮機輸出\r\n");
      }
    }
    else
    {
      out_sta.compressor &= out_EC_disable;
    }
  }
  else
  {
    //若在融霜模式下, 則不可啟動強冷功能
    out_sta.compressor &= out_EC_disable;
    icon.Enhanced_Cooling_sta = icon_none;
  }
}

static void concof_logic(void)
{
  /* 探頭失靈時, 壓縮機動作
  * 先啟動壓縮機, 依照COn時間保持開啟, 時間到後依據COF保持關閉, 以此循環
  */

  static bool dly_1 = false, dly_2 = false;
  static bool working_mode=0;   //0:COn模式; 1:COF模式

  if(TempValue.sensor1 == ERROR_AD)
  {
    if(working_mode==0)
    {
      dly_1 = Mydelay_min(dly_COn, System.value[COn]);
      if(dly_1 == false)
      {
        out_sta.compressor|= out_COn_enable;
        icon.Refrigerate_sta = ICON_ON;
        // printf("COn_開啟壓縮機輸出 = %d\r\n", out_sta.compressor);
      }
      else
      {
        out_sta.compressor &= out_COn_disable;
        icon.Refrigerate_sta = ICON_OFF;
        // printf("COn_關閉壓縮機輸出 = %d\r\n", out_sta.compressor);
        working_mode = 1;
      }
    }
    else
    {
      dly_2 = Mydelay_min(dly_COF, System.value[COF]);
      if(dly_2 == false)
      {
        out_sta.compressor &= out_COF_disable;
        icon.Refrigerate_sta = ICON_OFF;
        // printf("COF_關閉壓縮機輸出 = %d\r\n", out_sta.compressor);
      }
      else
      {
        out_sta.compressor|= out_COF_enable;
        icon.Refrigerate_sta = ICON_ON;
        working_mode = 0;
        // printf("COF_開啟壓縮機輸出 = %d\r\n", out_sta.compressor);
      }
    }
  }
  else
  {
    icon.Refrigerate_sta = ICON_OFF;
    out_sta.compressor &= out_COF_disable;
    out_sta.compressor &= out_COn_disable;
    dly_1 = false;
    dly_2 = false;
    working_mode = 0;
  }
}


static void normal_refrigerate_logic(void)
{
  int16_t sv = (System.value[Set]+System.value[Hy]);
  if(System.pv > sv)
  {
    icon.Refrigerate_sta = ICON_ON;
    out_sta.compressor |= out_NR_enable;
    // printf("System.pv = %d\r\n", System.pv);
    // printf("System.Set = %d\r\n", System.value[Set]);
    // printf("System.Hy = %d\r\n", System.value[Hy]);
    // printf("sv = %d\r\n", sv);
    // printf("normal開啟壓縮機輸出\r\n\n\n");
  }
  else if(System.pv <= System.value[Set])
  {
    icon.Refrigerate_sta = ICON_OFF;
    out_sta.compressor &= out_NR_disable;
    // printf("System.pv = %d\r\n", System.pv);
    // printf("sv = %d\r\n", sv);
    // printf("normal關閉壓縮機輸出\r\n\n\n");
  }
}

static void out_logic(void)
{
  /* 輸出間的邏輯控制
  * 首先確認是否有上電延遲需求 (ods_logic)
  * 是否為強冷凍循環 (cct_logic)
  * 進入普通製冷流程 (normal_refrigerate_logic)
  */
  if(ods_logic() == true)
  {
    cct_logic();
    concof_logic();
    normal_refrigerate_logic();
  }
  else
  {
    out_alloff();
    printf("關閉所有輸出邏輯\r\n");
  }
}

/* Output Logical Function definitions ------------------------------------------------------*/
void Out_main(void)
{
  // bootled_En -> 代表開機動畫已結束
  // 確認旗標狀態後, 執行out_api依照各旗標運作

  if(bootled_En==false)
  {
    out_logic();
    out1_FAN_api();
    out2_Compressor_api();
    out3_Defrost_api();
    out4_buzzer_api();
  }
  
}








