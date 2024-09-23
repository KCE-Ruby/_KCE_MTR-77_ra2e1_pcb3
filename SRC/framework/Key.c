/**
 ******************************************************************************
 * @file    Key.c
 * @author  KCE R&D Team
 * @date    2024-06-26
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/Key.h"
#include "INC/framework/datapool.h"
#include "INC/framework/Display.h"

/* Private defines ----------------------------------------------------------*/
#define KEY_DEBOUNCE_long              (150)      //(單位為:ms/次), 長按
#define KEY_DEBOUNCE_short              (15)      //(單位為:ms/次), 短按
#define CONTI_PRESS_THRESHOLD          (150)      //連加的閾值時間, 單位:ms
#define AUTO_INCREMENT_DELAY            (50)      //連續累加的間隔時間, 單位:ms

#define KEY_cnt_2s                     (400)
#define KEY_cnt_7s                    (1400)
#define KEY_cnt_max                   (5000)
#define KEY_cnt_min                      (1)

/* extern variables -----------------------------------------------------------------*/
extern __IO bsp_io_level_t KeyPin, pin_sta[6];
extern __IO s_Var System;
extern __IO s_Flag sFlag;
extern __IO r_tmr tmr;
extern __IO uint8_t bytetable_pr1[End];
extern __IO uint8_t bytetable_pr2[End];
extern __IO uint8_t Pr1_size, Pr2_size;

/* variables -----------------------------------------------------------------*/
__IO uint8_t disp_level;
__IO Key_Manager KeyUp, KeyDown, KeyStandby, KeyBulb, KeyDefrost, KeySet;

/* Private function protocol -----------------------------------------------*/
static Key_Manager key_detect(Key_Manager key);
static key_up_function(void);
static key_down_function(void);
static key_standby_function(void);
static key_bulb_function(void);
static key_defrost_function(void);
static key_set_function(void);

/* Function definitions ------------------------------------------------------*/
void Key_main(void)
{
  //[上鍵功能]
  key_up_function();

  //[下鍵功能]
  key_down_function();

  //[待機鍵功能]
  key_standby_function();

  //[燈開關鍵功能]
  key_bulb_function();

  //[融霜鍵功能]
  key_defrost_function();

  //[設定鍵功能]
  key_set_function();
}

void Key_debounce(void)
{
  static uint8_t flag;
  switch (tmr.COM_Port+1)
  {
    case keyup:
    if(flag != keyup)
    {
      KeyUp = key_detect(KeyUp);
      flag = keyup;
    }
    break;

    case keydown:
    if(flag != keydown)
    {
      KeyDown = key_detect(KeyDown);
      flag = keydown;
    }
    break;

    case keystandby:
    if(flag != keystandby)
    {
      KeyStandby = key_detect(KeyStandby);
      flag = keystandby;
    }
    break;

    case key_bulb:
    if(flag != key_bulb)
    {
      KeyBulb = key_detect(KeyBulb);
      flag = key_bulb;
    }
    break;

    case key_forst:
    if(flag != key_forst)
    {
      KeyDefrost = key_detect(KeyDefrost);
      flag = key_forst;
    }
    break;

    case keyset:
    if(flag != keyset)
    {
      KeySet = key_detect(KeySet);
      flag = keyset;
    }
    break;
  }
}

bool IsAnyKeyPressed(void)
{
  bool ku, kd, ks, kb, kf, kt;
  ku = (KeyUp.Cnt!=0)? true:false;
  kd = (KeyDown.Cnt!=0)? true:false;
  ks = (KeyStandby.Cnt!=0)? true:false;
  kb = (KeyBulb.Cnt!=0)? true:false;
  kf = (KeyDefrost.Cnt!=0)? true:false;
  kt = (KeySet.Cnt!=0)? true:false;

  if(ku || kd || ks || kb || kf || kt)
    return true;
  else
    return false;
}

static Key_Manager key_detect(Key_Manager key)
{
  // Key HW config, Release = High; Pressed = Low
  bool key_sta;
  key_sta = pin_sta[tmr.COM_Port+1];

  //按下時, 計數器++
  if(key_sta)
  {
    key.Cnt++;
    //長按超過一定時間時, 判斷給按鍵數字連按用
    if(key.Cnt>=CONTI_PRESS_THRESHOLD)
      key.conti_pressed = true;
  }
  else
  {
    //放開時, 清除連按旗標
    key.conti_pressed = false;

    //放開時, 判斷計數器被按下多久, 分成長按以及短按
    if(key.Cnt>=KEY_DEBOUNCE_long)    //長按暫時未被使用
      key.LongPressed++;

    if(key.Cnt>=KEY_DEBOUNCE_short)
    {
      key.shortPressed++;
      if(key.shortPressed > KEY_cnt_max)
        key.shortPressed = KEY_cnt_min;
    }
    key.Cnt=0;
  }

  return key;
}

static key_up_function(void)
{
  /*[上鍵功能]
  * Home狀態下, 單擊可查看最大值
  * Menu狀態下, 單擊向下(參數表)瀏覽參數
  * Setting狀態下, 單擊增加參數值
  * [組合鍵功能]
  * [上+下] = 鎖定or解鎖鍵盤
  */
  int16_t data_bytetable;
  int8_t pr1_index, pr2_index;
  static uint32_t lastIncrementTime_up = 0;

  if(KeyUp.shortPressed != 0)
  {
    System.keymode.Max_flag = false;
    switch (System.mode)
    {
      case homeMode:
        System.keymode.Max_flag = true;
      break;

      case level1Mode:
      //如果有SET鍵代表要離開level1層了, 所以要加減數值必須要沒有SET鍵
      if(KeySet.Cnt==0)
      {
        //TODO:要先帶入現有的數值再--, 還要檢查最大最小值
        if(sFlag.Level1_value == Vindex)
        {
          //要被修改的index應該是pr1的table, 而不是System.value的table
          System.level1_index++;
          if(System.level1_index >= Pr1_size) System.level1_index= 0;
        }
        else if(sFlag.Level1_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是System.value的table
          pr1_index = bytetable_pr1[System.level1_index];
          System.value[pr1_index]++;

          //檢查最大最小值, index要放pr1的不是總表的
          data_bytetable = System.value[pr1_index];
          // printf("-------------key up測試開始-------------\r\n");
          // printf("data_bytetable = %d\r\n", data_bytetable);
          System.value[pr1_index] = check_Limit_Value(data_bytetable, pr1_index);
          // printf("keyup數值 = %d\r\n", System.value[pr1_index]);
        }
      }
      break;

      case level2Mode:
      //如果有SET鍵代表要離開level1層了, 所以要加減數值必須要沒有SET鍵
      if(KeySet.Cnt==0)
      {
        //TODO:要先帶入現有的數值再--, 還要檢查最大最小值
        if(sFlag.Level2_value == Vindex)
        {
          //要被修改的index應該是pr1的table, 而不是System.value的table
          System.level2_index++;
          if(System.level2_index >= Pr2_size) System.level2_index= 0;
        }
        else if(sFlag.Level2_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是System.value的table
          pr2_index = bytetable_pr2[System.level2_index];
          System.value[pr2_index]++;

          //檢查最大最小值, index要放pr1的不是總表的
          data_bytetable = System.value[pr2_index];
          printf("-------------key up測試開始-------------\r\n");
          printf("data_bytetable = %d\r\n", data_bytetable);
          System.value[pr2_index] = check_Limit_Value(data_bytetable, pr2_index);
          printf("keyup數值 = %d\r\n", System.value[pr2_index]);
        }
      }
      break;

      case settingMode:
        //TODO: get設定值的最大值API, 不要extern 整個table
        System.value[Set]++;
        if(System.value[Set] > 1100) System.value[Set] = 1100;
      break;

      default:
        System.keymode.Max_flag = false;
        System.keymode.Min_flag = false;
      break;
    }
    KeyUp.shortPressed = 0;
  }
  else if(KeyUp.conti_pressed==true)
  {
    //檢測是否為連加
    {
      //檢測是否到了自動累加的時間間隔
      if (tmr.Cnt_1ms - lastIncrementTime_up >= AUTO_INCREMENT_DELAY)
      {
        lastIncrementTime_up = tmr.Cnt_1ms;  //更新累加時間
        if(System.mode == level1Mode)
        {
          //處理長時間連加數值的動作
          if(sFlag.Level1_value == Vvalue)
          {
            //要被修改的index應該是pr1的table, 而不是System.value的table
            pr1_index = bytetable_pr1[System.level1_index];
            System.value[pr1_index]++;

            //檢查最大最小值, index要放pr1的不是總表的
            data_bytetable = System.value[pr1_index];
            // printf("-------------連加測試開始-------------\r\n");
            // printf("data_bytetable = %d\r\n", data_bytetable);
            System.value[pr1_index] = check_Limit_Value(data_bytetable, pr1_index);
            // printf("keyup數值 = %d\r\n", System.value[pr1_index]);
          }
        }
        else if(System.mode == level2Mode)
        {
          //處理長時間連加數值的動作
          if(sFlag.Level2_value == Vvalue)
          {
            //要被修改的index應該是pr1的table, 而不是System.value的table
            pr2_index = bytetable_pr2[System.level2_index];
            System.value[pr2_index]++;

            //檢查最大最小值, index要放pr1的不是總表的
            data_bytetable = System.value[pr2_index];
            // printf("-------------連加測試開始-------------\r\n");
            // printf("data_bytetable = %d\r\n", data_bytetable);
            System.value[pr2_index] = check_Limit_Value(data_bytetable, pr2_index);
            // printf("keyup數值 = %d\r\n", System.value[pr1_index]);
          }
        }
      }
    }
  }
}

static key_down_function(void)
{
  /*[下鍵功能]
  * Home狀態下, 單擊可查看最小值
  * Menu狀態下, 單擊向上(參數表)瀏覽參數
  * Setting狀態下, 單擊減少參數值
  */
  int16_t data_bytetable;
  int8_t pr1_index, pr2_index;
  static uint32_t lastIncrementTime_down = 0;

  if(KeyDown.shortPressed)
  {
    System.keymode.Min_flag = false;
    switch (System.mode)
    {
      case homeMode:
        System.keymode.Min_flag = true;
      break;

      case level1Mode:
      //如果有SET鍵代表要離開level1層了, 所以要加減數值必須要沒有SET鍵
      if(KeySet.Cnt==0)
      {
        //TODO:要先帶入現有的數值再--, 還要檢查最大最小值
        if(sFlag.Level1_value == Vindex)
        {
          System.level1_index--;
          if(System.level1_index < 0) System.level1_index = (Pr1_size-1);
        }
        else if(sFlag.Level1_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是System.value的table
          pr1_index = bytetable_pr1[System.level1_index];
          System.value[pr1_index]--;
          
          //檢查最大最小值, index要放pr1的不是總表的
          // printf("-------------key down測試開始-------------\r\n");
          data_bytetable = System.value[pr1_index];
          // printf("data_bytetable = %d\r\n", data_bytetable);
          System.value[pr1_index] = check_Limit_Value(data_bytetable, pr1_index);
          // printf("keydown數值 = %d\r\n", System.value[pr1_index]);
        }
      }
      break;

      case level2Mode:
      //如果有SET鍵代表要離開level1層了, 所以要加減數值必須要沒有SET鍵
      if(KeySet.Cnt==0)
      {
        //TODO:要先帶入現有的數值再--, 還要檢查最大最小值
        if(sFlag.Level2_value == Vindex)
        {
          System.level2_index--;
          if(System.level2_index < 0) System.level2_index = (Pr2_size-1);
        }
        else if(sFlag.Level2_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是System.value的table
          pr2_index = bytetable_pr2[System.level2_index];
          System.value[pr2_index]--;
          
          //檢查最大最小值, index要放pr1的不是總表的
          printf("-------------key down測試開始-------------\r\n");
          data_bytetable = System.value[pr2_index];
          printf("data_bytetable = %d\r\n", data_bytetable);
          System.value[pr2_index] = check_Limit_Value(data_bytetable, pr2_index);
          printf("keydown數值 = %d\r\n", System.value[pr2_index]);
        }
      }
      break;

      case settingMode:
        //TODO: get設定值的最小值API, 不要extern 整個table
        System.value[Set]--;
        if(System.value[Set] < -500) System.value[Set] = -500;
      break;

      default:
        System.keymode.Max_flag = false;
        System.keymode.Min_flag = false;
      break;
    }
    KeyDown.shortPressed = 0;
  }
  else if(KeyDown.conti_pressed==true)
  {
    //檢測是否為連減
    {
      //檢測是否到了自動累減的時間間隔
      if (tmr.Cnt_1ms - lastIncrementTime_down >= AUTO_INCREMENT_DELAY)
      {
        lastIncrementTime_down = tmr.Cnt_1ms;  //更新累加時間

        //處理長時間連減數值的動作
        if(sFlag.Level1_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是System.value的table
          pr1_index = bytetable_pr1[System.level1_index];
          System.value[pr1_index]--;

          //檢查最大最小值, index要放pr1的不是總表的
          data_bytetable = System.value[pr1_index];
          // printf("-------------連加測試開始-------------\r\n");
          // printf("data_bytetable = %d\r\n", data_bytetable);
          System.value[pr1_index] = check_Limit_Value(data_bytetable, pr1_index);
          // printf("keyup數值 = %d\r\n", System.value[pr1_index]);
        }
      }
    }
  }
}

static key_standby_function(void)
{
  /*[待機鍵功能]
  * 在參數 onF=oFF時, 單擊進入待機功能;
  * 在參數 onF=ES時, 單擊進入節能運行功能;
  */

 if(KeyStandby.shortPressed)
 {
    //TODO: 按下按鍵後的功能
    KeyStandby.shortPressed = 0;
 }
}

static key_bulb_function(void)
{
  /*[燈開關鍵功能]
  * 此鍵不可用(無作用)
  */

 if(KeyBulb.shortPressed)
 {
    //TODO: 按下按鍵後的功能
    KeyBulb.shortPressed = 0;
 }
}

static key_defrost_function(void)
{
  /*[融霜鍵功能]
  * Home狀態下, 單擊啟動一次手動融霜
  * Menu狀態下, 無作用
  */

 if(KeyDefrost.Cnt > KEY_cnt_2s)
 {
    switch (System.mode)
    {
      case homeMode:
        sFlag.Defrost = true;
      break;

      case level1Mode:
      
      break;

      case level2Mode:
      
      break;

      case settingMode:

      break;

      default:
      
      break;
    }
 }
 else if(KeyDefrost.LongPressed != 0)
 {
    KeyDefrost.LongPressed = 0;
    KeyDefrost.shortPressed = 0;
 }
 else if(KeyDefrost.shortPressed)
 {
    //TODO: 按下按鍵後的功能
    KeyDefrost.shortPressed = 0;
 }
}

static key_set_function(void)
{
  int16_t pre_value[End];
  /*<設定鍵功能>
  * homeMode狀態下, 單擊顯示目標設定點(Pv)切換成checkgMode
  * level1&2Mode狀態下, 單擊選擇參數或確認操作
  * checkgMode, 5s後或者再次單擊跳回homeMode
  * settingMode狀態下, 修改參數值
  * 
  * <組合鍵功能>
  * [設定+下] = 進入Menu模式
  * [設定+上] = 退出Menu回Home
  */

 //按鍵當次判斷只允許一種事件發生, 長按優先權大於短按
 if(KeySet.Cnt > KEY_cnt_2s)
 {
    switch (System.mode)
    {
      case homeMode:
      if(KeyDown.Cnt==0)
      {
        //進入修改設定值功能
        System.mode = settingMode;
        System.keymode.SET_value_flag = true;
      }
      else
      {
        //進入用戶層第一層
        System.mode = level1Mode;
        System.level1_index = 0;
      }
      break;

      case level1Mode:
      if((KeySet.Cnt>KEY_cnt_7s)&&(KeyDown.Cnt!=0))
      {
        //進入隱藏層第二層, 顯示Pr2
        sFlag.Level1_value = Pr2_symbol;
      }
      break;

      case level2Mode:
      
      break;

      case settingMode:

      break;

      default:
      
      break;
    }
 }
 else if(KeySet.LongPressed != 0)
 {
  if(sFlag.Level1_value == Pr2_symbol)
  {
    //長按後7秒進入Pr2顯示後, 放開兩鍵則進入隱藏層
    System.mode = level2Mode;
    System.level2_index = 0;
  }
  KeySet.LongPressed = 0;
  KeySet.shortPressed = 0;
 }
 else if(KeySet.shortPressed != 0)
 {
    //TODO: 按下按鍵後的功能
    switch (System.mode)
    {
      case homeMode:
        System.mode = checkgMode;
        System.keymode.SET_value_flag = true;
      break;

      case level1Mode:
        if(KeyUp.Cnt!=0)  //離開level1的路徑, 短按SET鍵+下鍵 -> 離開
        {
          KeyUp.Cnt = 0;
          System.mode = homeMode;
          // System.level1_index = 0;
        }
        else  //切換參數名稱or參數數值的地方
        {
          if(sFlag.Level1_value == Vindex)
            sFlag.Level1_value = Vvalue;
          else
          {
            sFlag.Level1_value = Vindex;
            if(pre_value[System.level1_index] != System.value[System.level1_index])
            {
              //TODO: 若參數有變動則寫入eeprom內
            }
          }
        }
      break;

      case level2Mode:
      break;

      case settingMode:
        sFlag.leaveSet = true; //短按一次後離開set模式
      break;

      case checkgMode:
        System.keymode.SET_value_flag = false;
      break;

      default:
      break;
    }
    KeySet.shortPressed = 0;
 }
}

/*--------------------------- for test key funcitons ---------------------------*/
// uint8_t keyPressed[7];
// __IO uint16_t key_Cnt[7];
// void Key_test(void)
// {
//   //輪巡按鍵測試
//   static uint8_t t_key=1;
//   bool key_sta;
  
//   if(t_key > 6) t_key = 1;
//   SMG_CLOSE(1);
//   SMG_CLOSE(2);
//   SMG_CLOSE(3);
//   SMG_CLOSE(4);
//   SMG_CLOSE(5);
//   SMG_CLOSE(6);
//   SMG_OPEN(t_key);

//   R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_14, &KeyPin);
//   pin_sta[t_key] = KeyPin ? false : true;

//   key_sta = pin_sta[t_key];
//   if(key_sta)
//     key_Cnt[t_key]++;
//   else
//   {
//     //放開時, 判斷計數器是否有被按下
//     if(key_Cnt[t_key] > 0)
//     {
//       keyPressed[t_key]++;
//       key_Cnt[t_key]=0;
//     }
//   }
//   t_key++;
// }

// void Key_oneshot(void)
// {
//   //單擊測試, KeyPin->HIGH為放開, LOW為按下
//   // 1->右下(keystandby); 2->右上(keyup); 3->左上(key_limp)
//   // 4->左中(key_forst); 5->右中(keydown); 6->左下(keyset)
//   uint8_t t_key=6;
//   fsp_err_t err;
//   SMG_OPEN(t_key);
//   SMG_CLOSE(1);
//   SMG_CLOSE(2);
//   SMG_CLOSE(3);
//   SMG_CLOSE(4);
//   SMG_CLOSE(5);
//   err = R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_14, &KeyPin);
//   pin_sta[t_key] = KeyPin ? false : true;
// }
