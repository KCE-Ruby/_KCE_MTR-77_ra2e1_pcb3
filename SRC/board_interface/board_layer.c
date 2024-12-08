/**
 ******************************************************************************
 * @file    board_layer.c
 * @author  KCE R&D Team
 * @date    2024-07-19
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 * 
 * callback code by RA Liabary or the funciton that everywhere can use
 * Watchdog
 * Timer
 * Uart
 * I2c(eeprom)
 * RS485
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"

#define Timer_Limit_Counter_Max  (600000) //10min; 600s

/* variables -----------------------------------------------------------------*/
bool Recordmode_High, Recordmode_Low, Recordmode_clear;


static bool scan_complete_flag = false;
__IO r_tmr tmr;
__IO bsp_io_level_t KeyPin, pin_sta[7];
__IO s_Var Syscfg, Preload;
__IO s_Flag sFlag;
__IO uint8_t Pr1_size, Pr2_size;
__IO uint32_t catch_min[dly_end_min] = {};
__IO uint16_t catch_s[dly_end_sec] = {};
__IO uint32_t catch_ms[dly_end_ms] = {};

/* task function protocol -----------------------------------------------*/
static void TMR_init(void);
static void ADC_init(void);
static void WDT_init(void);
static fsp_err_t Uart_init(void);

/* Global Function definitions ------------------------------------------------------*/
void System_Init(void)
{
  WDT_init();   //16384 cycle watchdog
  TMR_init();   //1ms Timer
  Uart_init();
  ADC_init();
  I2C_EE_Init();
  Pr1_size = get_bytetable_pr1();  //開機時取得當次的用戶層相關table
  Pr2_size = get_bytetable_pr2();  //開機時取得當次的隱藏層相關table
  //預設開機為小數點一位數, TODO:讀取eeprom後這行要拿掉
  // Syscfg.decimalIndex = DECIMAL_AT_1;
}

static void TMR_init(void)
{
  fsp_err_t err = FSP_SUCCESS;
  /* Initializes the module. */
  err = R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);
  /* Handle any errors. This function should be defined by the user. */
  assert(FSP_SUCCESS == err);
  /* Start the timer. */
  (void) R_GPT_Start(&g_timer0_ctrl);

  // /* Initializes the module. */
  // err = R_AGT_Open(&g_timer1_ctrl, &g_timer1_cfg);
  // /* Handle any errors. This function should be defined by the user. */
  // assert(FSP_SUCCESS == err);
  // /* Start the timer. */
  // (void) R_AGT_Start(&g_timer1_ctrl);
}

static void ADC_init(void)
{
  fsp_err_t status;
  status = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
  status = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
  assert(FSP_SUCCESS == status);
}

//watchdog
static void WDT_init(void)
{
  fsp_err_t status;

  //如果使用J-Link调试器进行调试的话需要加上这一句话
  R_DEBUG->DBGSTOPCR_b.DBGSTOP_WDT = 0;

  status = R_WDT_Open(&g_wdt0_ctrl, &g_wdt0_cfg);
  while(status!=FSP_SUCCESS);

  /*刷新看门狗计数器，在这里的作用是初次启动寄存模式下的看门狗计数器
   *要注意，除非是在刷新允许的范围内，否则自启动模式下不应该使用该函数*/
  status = R_WDT_Refresh(&g_wdt0_ctrl);
  while(status!=FSP_SUCCESS);
}

void WDT_Feed(void)
{
   R_WDT_Refresh(&g_wdt0_ctrl);
}

//uart
static fsp_err_t Uart_init(void)
{
  fsp_err_t err;
  err = R_SCI_UART_Open(&g_uart9_ctrl, &g_uart9_cfg);
  if (FSP_SUCCESS != err)
  {
      // 处理错误
      return err;
  }
  return FSP_SUCCESS;
}

fsp_err_t uart_send(uint8_t *p_data, uint32_t length)
{
  fsp_err_t err;

  err = R_SCI_UART_Write(&g_uart9_ctrl, p_data, length);
  if (FSP_SUCCESS != err)
  {
      // 处理错误
      return err;
  }
  return FSP_SUCCESS;
}

fsp_err_t uart_receive(uint8_t *p_data, uint32_t length)
{
  fsp_err_t err;

  err = R_SCI_UART_Read(&g_uart9_ctrl, p_data, length);
  if (FSP_SUCCESS != err)
  {
      // 处理错误
      return err;
  }
  return FSP_SUCCESS;
}

//ADC
bool get_scanflag(void)
{
  return scan_complete_flag;
}

void set_scanflag(bool flag)
{
  scan_complete_flag = flag;
}

//IO
void SMG_OPEN(uint8_t smg)
{
  switch (smg)
  {
    case 1:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_00, BSP_IO_LEVEL_LOW);
      break;
    case 2:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_01, BSP_IO_LEVEL_LOW);
      break;
    case 3:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_LOW);
      break;
    case 4:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_03, BSP_IO_LEVEL_LOW);
      break;
    case 5:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_13, BSP_IO_LEVEL_LOW);
      break;
    case 6:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_07, BSP_IO_LEVEL_LOW);
      break;

    default:
      break;
  }
}

void SMG_CLOSE(uint8_t smg)
{
  switch (smg)
  {
    case 1:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_00, BSP_IO_LEVEL_HIGH);
      break;
    case 2:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_01, BSP_IO_LEVEL_HIGH);
      break;
    case 3:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_HIGH);
      break;
    case 4:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_03, BSP_IO_LEVEL_HIGH);
      break;
    case 5:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_13, BSP_IO_LEVEL_HIGH);
      break;
    case 6:
      R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_07, BSP_IO_LEVEL_HIGH);
      break;

    default:
      break;
  }
}

void Key_ReadPin(void)
{
  fsp_err_t err;
  err = R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_14, &KeyPin);
  pin_sta[tmr.COM_Port+1] = KeyPin ? false : true;
}

//timer
bool Mydelay_min(uint8_t flag, uint32_t min)
{
  /*
  * 這是以分為單位的計時器, 如果用60s的計時單位會有誤差值最多1min, 所以要用s
  * 用系統值delay min, 第一次進來的時候catch住當時的時間
  * 結束之後free掉min, 給下次用
  * 因為是loop執行, 所以有可能別人的delay還沒跑完就又要跑下一個delay
  * 可以同時執行多個delay, 所以要catch_min分旗標用, 每個人的起始值都獨立運算
  */
  const uint32_t catch_end = (tmr.Cnt_1s-catch_min[flag]);
  bool ret=false;

  // printf("delay的sec = %d\r\n", min);
  // printf("tmr.Cnt_60s = %d\r\n", tmr.Cnt_60s);
  if(catch_min[flag] == 0)
  {
    catch_min[flag] = tmr.Cnt_1s; //記住當下值
    // printf("記住當下值\r\n");
  }
  else if(catch_end >= (min*60))
  {
    //時間到後, 清除catch值以及回傳true代表delay計時結束
    catch_min[flag] = 0;
    ret = true;
    printf("catch_min[%d] = %d;   ", flag, catch_min[flag]);
    printf("min = %d\r\n", min);
  }
  return ret;
}

bool Mydelay_sec(uint8_t flag, uint16_t sec)
{
  /*
  * 這是以秒為單位的計時器
  * 用系統值delay s, 第一次進來的時候catch住當時的時間
  * 結束之後free掉s, 給下次用
  * 因為是loop執行, 所以有可能別人的delay還沒跑完就又要跑下一個delay
  * 可以同時執行多個delay, 所以要catch_s分旗標用, 每個人的起始值都獨立運算
  */
  const uint16_t catch_end = (tmr.Cnt_1s-catch_s[flag]);
  bool ret=false;

  // printf("delay的sec = %d\r\n", sec);
  // printf("tmr.Cnt_1s = %d\r\n", tmr.Cnt_1s);
  if(catch_s[flag] == 0)
  {
    catch_s[flag] = tmr.Cnt_1s; //記住當下值
    // printf("記住當下值\r\n");
  }
  else if(catch_end >= sec)
  {
    //時間到後, 清除catch值以及回傳true代表delay計時結束
    catch_s[flag] = 0;
    ret = true;
    printf("catch_s[%d] = %d;   ", flag, catch_s[flag]);
    printf("sec = %d\r\n", sec);
  }
  return ret;
}

bool Mydelay_ms(uint8_t flag, uint16_t ms)
{
  /*
  * 這是以毫秒為單位的計時器
  * 用系統值delay ms, 第一次進來的時候catch住當時的時間
  * 結束之後free掉ms, 給下次用
  * 因為是loop執行, 所以有可能別人的delay還沒跑完就又要跑下一個delay
  * 可以同時執行多個delay, 所以要catch_ms分旗標用, 每個人的起始值都獨立運算
  */
  const uint32_t catch_end = (tmr.Cnt_1ms-catch_ms[flag]);
  bool ret=false;

  if(catch_ms[flag] == 0)
  {
    catch_ms[flag] = tmr.Cnt_1ms; //記住當下值
  }
  else if(catch_end >= ms)
  {
    //時間到後, 清除catch值以及回傳true代表delay計時結束
    catch_ms[flag] = 0;
    ret = true;
    // printf("catch_ms[%d] = %d\r\n", flag, catch_ms[flag]);
  }
  return ret;
}

/* callback Function definitions by code generated ------------------------------------------------------*/
void r_wdt_callback(wdt_callback_args_t * p_args)
{
   /*防止编译器产生关于函数中没有使用形参的警告*/
   (void) p_args;

  /* TODO: 这里应该放最重要的函数，比如保存重要数据等 */
   R_BSP_SoftwareDelay(3, BSP_DELAY_UNITS_SECONDS);

   /* 通过软件复位MCU*/
   __NVIC_SystemReset();
}

void timer0_callback(timer_callback_args_t *p_args)
{
  if (TIMER_EVENT_CYCLE_END == p_args->event)
  {
    tmr.Cnt_1ms++;
    tmr.COM_Port++;

    if ((tmr.Cnt_1ms % 500) == 0)  tmr.FlashFlag_1Hz = !tmr.FlashFlag_1Hz;
    if ((tmr.Cnt_1ms % 1000) == 0)  tmr.FlashFlag_2Hz = !tmr.FlashFlag_2Hz;

    if((tmr.Cnt_1ms%1)==0) tmr.Flag_1ms = true;
    // if((tmr.Cnt_1ms%100)==0) tmr.Flag_100ms = true;
    if((tmr.Cnt_1ms%100)==0) tmr.Flag_100ms = true;
    // if((tmr.Cnt_1ms%500)==0) tmr.Flag_500ms = true;
    // if((tmr.Cnt_1ms%1000)==0) tmr.Flag_1s = true;
    if((tmr.Cnt_1ms%60000)==0) tmr.Cnt_60s++;
    if((tmr.Cnt_1ms%1000)==0) tmr.Cnt_1s++;

    tmr.Cnt_1ms = (tmr.Cnt_1ms>Timer_Limit_Counter_Max)? 1:tmr.Cnt_1ms;
  }
}

// void timer1_callback(timer_callback_args_t *p_args)
// {
  // if (TIMER_EVENT_CYCLE_END == p_args->event)
  // {
  //   tmr.Cnt_1ms++;
    
  //   if((tmr.Cnt_1ms%1)==0) tmr.Flag_5ms = true;
  //   if((tmr.Cnt_1ms%100)==0) tmr.Flag_100ms = true;
  //   if((tmr.Cnt_1ms%200)==0) tmr.Flag_200ms = true;
  //   if((tmr.Cnt_1ms%500)==0) tmr.Flag_500ms = true;
  //   if((tmr.Cnt_1ms%1000)==0) tmr.Flag_1s = true;
  //   if((tmr.Cnt_1ms%2000)==0) tmr.Flag_2s = true;
  //   if((tmr.Cnt_1ms%1000)==0) tmr.Cnt_1s++;

  //   tmr.Cnt_1ms = (tmr.Cnt_1ms>Timer_Limit_Counter_Max)? 1:tmr.Cnt_1ms;
  // }
// }

void user_adc_callback(adc_callback_args_t * p_args)
{
  scan_complete_flag = false;
  FSP_PARAMETER_NOT_USED(p_args);
  scan_complete_flag = true;
}

