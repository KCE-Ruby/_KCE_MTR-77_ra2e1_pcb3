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
#include "INC/framework/eep_api.h"
#include "INC/framework/Display.h"

/* Private defines ----------------------------------------------------------*/
#define KEY_DEBOUNCE_long              (150)      //(單位為:ms/次), 長按
#define KEY_DEBOUNCE_short               (3)      //(單位為:ms/次), 短按
#define CONTI_PRESS_THRESHOLD          (150)      //連加的閾值時間, 單位:ms
#define AUTO_INCREMENT_DELAY_L          (60)      //連續累加的間隔時間, 單位:ms
#define AUTO_INCREMENT_DELAY_M          (50)      //連續累加的間隔時間, 單位:ms
#define AUTO_INCREMENT_DELAY_H          (15)      //連續累加的間隔時間, 單位:ms
#define CONTI_PRESS_times_M            (400)      //連續累加的承認次數, 單位:次
#define CONTI_PRESS_times_H           (1000)      //連續累加的承認次數, 單位:次

#define KEY_cnt_2s                     (350)
#define KEY_cnt_3s                     (525)
#define KEY_cnt_7s                    (1225)
#define KEY_cnt_max                   (5000)
#define KEY_cnt_min                      (1)

/* extern variables -----------------------------------------------------------------*/
extern int16_t sys_table[End];
extern int16_t pre_table[End];
extern bool Recordmode_High, Recordmode_Low, Recordmode_clear;
extern __IO ByteSettingTable User_original[End];

extern __IO bsp_io_level_t KeyPin, pin_sta[6];
extern __IO s_Var Syscfg, Preload;
extern __IO s_Flag sFlag;
extern __IO r_tmr tmr;
extern __IO uint8_t bytetable_pr1[End];
extern __IO uint8_t bytetable_pr2[End];
extern __IO uint8_t Pr1_size, Pr2_size;
extern __IO ByteSettingTable bytetable[End];
extern __IO bool clear_Max_flag, clear_Min_flag;
extern __IO icon_api_flag icon;
extern __IO uint8_t eeplength;
extern __IO uint8_t Preload_value[2];

/* variables -----------------------------------------------------------------*/
__IO uint8_t disp_level;
__IO Key_Manager KeyUp, KeyDown, KeyStandby, KeyBulb, KeyDefrost, KeySet;
static uint8_t api_sta=API_FREE;

/* Private function protocol -----------------------------------------------*/
//共用的按鍵API
static void levelmode_handle(uint8_t *index, bool iskeyup);
static void Vindex_process_DataDigit(int8_t pr_index);
static void clear_Recordmode_sta(void);

//設定鍵相關的API
static void key_set_function(void);

//上鍵相關的API
static void activateEnhancedCooling(void);
static void keyupshortpressed(void);
static void keyupconti_pressed(void);
static void keyupclearcnt(void);
static void key_up_function(void);

//下鍵相關的API
static void keydownshortpressed(void);
static void keydownconti_pressed(void);
static void keydownclearcnt(void);
static void key_down_function(void);

//待機鍵相關的API
static void keystandbyshortpressed(void);
static void keystandbyclearcnt(void);
static void key_standby_function(void);

//燈泡鍵相關的API
static void keybulbshortpressed(void);
static void keybulbclearcnt(void);
static void key_bulb_function(void);

//融霜鍵相關的API
static void keydefrostshortpressed(void);
static void keydefrostclearcnt(void);
static void key_defrost_function(void);

static void levelmode_keydown_handle(uint8_t *index);
static void Vindex_process_keyup(int8_t pr_index);
static void Vindex_process_keydown(int8_t pr_index);
static void Vvalue_process_keyup(int8_t pr_index);
static void Vvalue_process_keydown(int8_t pr_index);
static void intoPr2(void);
static Key_Manager key_detect(Key_Manager key);

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

/* Static Function definitions ------------------------------------------------------*/
//共用的按鍵API
static void levelmode_handle(uint8_t *index, bool iskeyup)
{
  /*[處理單擊上鍵後進入level1 or level2的功能]
  * 預先處理level1與level2的差異
  * 如果有SET鍵代表要離開level1層了, 所以要加減數值必須要沒有SET鍵
  * 
  * 如果是顯示參數字元, 則++對應table的index(pr1或是pr2), 而非總表的
  * 因為總表有一些保留字元, 在系統上是不會顯示的(也就是未開放功能)
  * 有一些比較特殊的參數數值顯示的方式不同,需要在選中該字元時就先做標記
  * 
  * 如果是顯示參數數值, 將數值++完後, 比較最大最小值即結束
  */

  uint8_t pr_size;
  int8_t *level_index;
  int8_t pr_index;

  //預先處理level1與level2的差異
  if(*index == sFlag.Level1_value)
  {
    level_index = &Syscfg.level1_index;
    pr_size = Pr1_size;
    pr_index = bytetable_pr1[(*level_index)];
  }
  else
  {
    level_index = &Syscfg.level2_index;
    pr_size = Pr2_size;
    pr_index = bytetable_pr2[(*level_index)];
  }

  //如果有SET鍵代表要離開level1層了, 所以要加減數值必須要沒有SET鍵
  if(KeySet.Cnt==0)
  {
    if(*index == Vindex)
    {
      //顯示參數字元index++/--
      if(iskeyup)
      {
        (*level_index)++;
        if((*level_index) >= pr_size) (*level_index)= 0;

        // 更新 pr_index 到最新值(一定要這樣寫)
        pr_index = ( *index == sFlag.Level1_value ) ? 
                    bytetable_pr1[(*level_index)] : 
                    bytetable_pr2[(*level_index)];

        //要先處理某些需要先特殊處理的參數
        Vindex_process_DataDigit(pr_index);
        // Vindex_process_keyup(pr_index);
      }
      else
      {
        (*level_index)--;
        if(*level_index < 0) *level_index = (pr_size-1);
        
        // 更新 pr_index 到最新值(一定要這樣寫)
        pr_index = ( *index == sFlag.Level1_value ) ? 
                    bytetable_pr1[(*level_index)] : 
                    bytetable_pr2[(*level_index)];

        //要先處理某些需要先特殊處理的參數
        Vindex_process_DataDigit(pr_index);
        // Vindex_process_keydown(pr_index);
      }

    }
    else if(*index == Vvalue)
    {
      //pr_index就是要被放到總表當index的, 所以在總表的onF以下都是可以改的數值
      if(pr_index <= onF)
      {
        //數值需要先被加/減過後再檢查最大最小值
        if(iskeyup)
        {
          //若本來是小數的則10倍數增加
          if(User_original[pr_index].DataDigit == dE)
          {
            if(sys_table[rES] == DECIMAL_AT_0)
              pre_table[pr_index] += 10;
            else if(sys_table[rES] == DECIMAL_AT_1)
              pre_table[pr_index]++;
          }
          else
            pre_table[pr_index]++;
        }
        else
        {
          //若本來是小數的則10倍數減少
          if(User_original[pr_index].DataDigit == dE)
          {
            if(sys_table[rES] == DECIMAL_AT_0)
              pre_table[pr_index] -= 10;
            else if(sys_table[rES] == DECIMAL_AT_1)
              pre_table[pr_index]--;
          }
          else
            pre_table[pr_index]--;
        }

        // 更新 pr_index 到最新值(一定要這樣寫)
        pr_index = ( *index == sFlag.Level1_value ) ? 
                    bytetable_pr1[(*level_index)] : 
                    bytetable_pr2[(*level_index)];

        pre_table[pr_index] = check_Limit_Value(pre_table[pr_index], pr_index);
        // printf("數值 = %d\r\n", pre_table[pr_index]);
      }
    }
  }
  else if(KeyUp.shortPressed != 0)
  {
    upload_syscfg_data(pr_index);
    KeyUp.Cnt = 0;
    Syscfg.mode = homeMode;
  }
}

static void Vindex_process_DataDigit(int8_t pr_index)
{
  //數值為整數單位
  if(User_original[pr_index].DataDigit == in)
    sFlag.Vvalue_int = true;
  else
    sFlag.Vvalue_int = false;
}

static void clear_Recordmode_sta(void)
{
  Recordmode_High = false;
  Recordmode_Low = false;
}


static void Vindex_process_keyup(int8_t pr_index)
{
  sFlag.Vvalue_int = false;
  // printf("keyup pr_index數值 = %d\r\n", pr_index);
  switch (pr_index)
  {
    case OdS:
    case AC:
    case COn:
    case COF:
      //數值為整數單位
      sFlag.Vvalue_int = true;
      break;
    
    default:
      break;
  }
}

static void Vindex_process_keydown(int8_t pr_index)
{
  sFlag.Vvalue_int = false;
  // printf("keydown pr_index數值 = %d\r\n", pr_index);
  switch (pr_index)
  {
    case OdS:
    case AC:
    case COn:
    case COF:
      //數值為整數單位
      sFlag.Vvalue_int = true;
      break;
    
    default:
      break;
  }
}

static void Vvalue_process_keyup(int8_t pr_index)
{
  switch (pr_index)
  {
    //要先處理某些需要先特殊處理的參數
    case CCt:
      if(Preload.value[CCt]%10 >= 6)
        Preload.value[CCt] += (10 - (Preload.value[CCt]%10));
      break;

    default:
      break;
  }
}

static void Vvalue_process_keydown(int8_t pr_index)
{
  switch (pr_index)
  {
    //要先處理某些需要先特殊處理的參數, 降位要從5開始, 先-10是降位, 再-6是從6進位的意思
    case CCt:
      if(Preload.value[CCt]%10 > 6)
        Preload.value[CCt] -= (10-6);
      break;

    default:
      break;
  }
}

static void intoPr2(void)
{
  if(sFlag.Level1_value == Pr2_symbol)
  {
    //長按後7秒進入Pr2顯示後, 放開兩鍵則進入隱藏層
    Syscfg.mode = level2Mode;
    Syscfg.level2_index = 0;
  }
  KeySet.LongPressed = 0;
  KeySet.shortPressed = 0;
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
    // if(key.Cnt != 0)   //測試用: 用來查看短按最快是幾次, 3次
    //   printf("key.cnt = %d\r\n", key.Cnt);

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

//設定鍵相關的API

static void keysetshortpressed(void)
{

}

static void keysetconti_pressed(void)
{

}

static void keysetclearcnt(void)
{

}

static void key_set_function(void)
{
  static uint8_t api_sta=API_FREE;
  static int16_t pre_value[End];
  uint8_t pr2_index;
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
 if(api_sta==API_FREE)
 {
  if(KeySet.Cnt > KEY_cnt_2s)  //未放開達兩秒時的動作
  {
      switch (Syscfg.mode)
      {
        case homeMode:
        if(KeyDown.Cnt==0)
        {
          //進入修改設定值功能
          Syscfg.mode = settingMode;
          Syscfg.keymode.SET_value_flag = true;
          api_sta = API_BUSY2;
        }
        else
        {
          //進入用戶層第一層
          Syscfg.mode = level1Mode;
          Syscfg.level1_index = 0;
        }
        break;

        case recordMode:
        //TODO: 要顯示rSt三秒然後reset歷史極值
        if(KeySet.Cnt > KEY_cnt_2s)
        {
          //只要觸發閃爍rSt功能必須閃完三次, 不會被中斷
          Recordmode_clear = true;

          if(clear_Max_flag)
          {
            sys_table[RecordHigh] = Syscfg.pv;
            clear_Max_flag = false;
          }
          else if(clear_Min_flag)
          {
            sys_table[RecordLow] = Syscfg.pv;
            clear_Min_flag = false;
          }
        }
        break;

        case level1Mode:
        if((sFlag.Level1_value==Pr2_symbol) &&(KeyDown.Cnt==0))
        {
          intoPr2();
        }
        else if((KeySet.Cnt>KEY_cnt_7s)&&(KeyDown.Cnt!=0))
        {
          //進入隱藏層第二層, 顯示Pr2
          sFlag.Level1_value = Pr2_symbol;
        }
        else  //否則進過隱藏層後會卡在隱藏層, 清掉才能顯示用戶層
          sFlag.Level1_value = Vindex;
        break;

        case level2Mode:
        
        break;

        case settingMode:

        break;

        default:
        
        break;
      }
  }
  else if((KeySet.Cnt>0))   //當被按下時
  {
    if((KeyDown.Cnt>0) && (Syscfg.mode==level2Mode))  //代表要改變Pr1與Pr2的值
    {
      api_sta = API_BUSY1;
      pr2_index  = bytetable_pr2[Syscfg.level2_index];
      if(bytetable[pr2_index].Mode == Pr1)
      {
        bytetable[pr2_index].Mode = Pr2;
        printf("Syscfg.level2_index = %d\r\n", pr2_index);
        printf("更改後的mode = Pr2\r\n", Pr2);
      }
      else
      {
        bytetable[pr2_index].Mode = Pr1;
        printf("Syscfg.level2_index = %d\r\n", pr2_index);
        printf("更改後的mode = Pr1\r\n", Pr1);
      }
      printf("api_sta = %d\r\n", api_sta);
    }
    else if(Syscfg.mode==recordMode)
    {
      clear_Recordmode_sta();   //為了避免卡在歷史低點畫面
    }
  }
  else if(KeySet.LongPressed != 0) //放開後判斷為長按時的動作
  {
    intoPr2();
  }
  else if(KeySet.shortPressed != 0) //放開後判斷為短按時的動作
  {
      //TODO: 按下按鍵後的功能
      switch (Syscfg.mode)
      {
        case homeMode:
          Syscfg.mode = checkgMode;
          Syscfg.keymode.SET_value_flag = true;
        break;

        case recordMode:
          Syscfg.keymode.Max_flag = false;
          Syscfg.keymode.Min_flag = false;
          Syscfg.mode = homeMode;
        break;
        
        case level1Mode:
          if(KeyUp.Cnt!=0)  //離開level1的路徑, 短按SET鍵+上鍵 -> 離開
          {
            KeyUp.Cnt = 0;
            Syscfg.mode = homeMode;
          }
          else  //切換參數名稱or參數數值的地方
          // if(KeyUp.Cnt==0)  //切換參數名稱or參數數值的地方
          {
            if(sFlag.Level1_value == Vindex)
              sFlag.Level1_value = Vvalue;
            else
            {
              sFlag.Level1_value = Vindex;
              // upload_syscfg_data(bytetable_pr1[Syscfg.level1_index]);
              if(Syscfg.value[bytetable_pr1[Syscfg.level1_index]] != Preload.value[bytetable_pr1[Syscfg.level1_index]])
              {
                uint8_t eepaddr;
                eepaddr = check_index_for_eep(bytetable_pr1[Syscfg.level1_index], eeplength);

                printf("Preload_value[0] = %d\r\n", Preload_value[0]);
                printf("Preload_value[1] = %d\r\n", Preload_value[1]);
                printf("eepaddr = %d\r\n", eepaddr);
                printf("eeplength = %d\r\n", eeplength);
                R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
                I2C_EE_BufferWrite(Preload_value, eepaddr, eeplength);
                R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
                Syscfg.value[bytetable_pr1[Syscfg.level1_index]] = Preload.value[bytetable_pr1[Syscfg.level1_index]];
              }
              else
                printf("沒--進入eeprom\r\n");

              //從參數值換成顯示字符時, 往後加一組
              Syscfg.level1_index++;
              if(Syscfg.level1_index >= Pr1_size) Syscfg.level1_index= 0;

              //要先處理某些需要先特殊處理的參數
              Vindex_process_keyup(bytetable_pr1[Syscfg.level1_index]);

            }
          }
        break;

        case level2Mode:
          if(KeyUp.Cnt!=0)  //離開level2的路徑, 短按SET鍵+上鍵 -> 離開
          {
            KeyUp.Cnt = 0;
            Syscfg.mode = homeMode;
          }
          else  //切換參數名稱or參數數值的地方
          // if(KeyUp.Cnt==0)  //切換參數名稱or參數數值的地方
          {
            if(sFlag.Level2_value == Vindex)
              sFlag.Level2_value = Vvalue;
            else
            {
              sFlag.Level2_value = Vindex;
              upload_syscfg_data(bytetable_pr2[Syscfg.level2_index]);
              if(pre_value[Syscfg.level2_index] != Preload.value[Syscfg.level2_index])
              {
                //TODO: 若參數有變動則寫入eeprom內
              }

              //從參數值換成顯示字符時, 往後加一組
              Syscfg.level2_index++;
              if(Syscfg.level2_index >= Pr2_size) Syscfg.level2_index= 0;

              //要先處理某些需要先特殊處理的參數
              Vindex_process_keyup(bytetable_pr2[Syscfg.level2_index]);

            }
          }
        break;

        case settingMode:
          sFlag.leaveSet = true; //短按一次後離開set模式
        break;

        case checkgMode:
          Syscfg.keymode.SET_value_flag = false;
        break;

        default:
        break;
      }
      KeySet.shortPressed = 0;
  }

 }
 else if(KeySet.Cnt==0)
 {
  //怎麼進去的就必須按鍵都放開才能清除busy
  switch (api_sta)
  {
    case API_BUSY1:
      if(KeyDown.Cnt==0)
      {
        api_sta = API_FREE;
        printf("檢查KeySet.Cnt = %d\r\n", KeySet.Cnt);
        printf("API_BUSY1釋放旗標 = %d\r\n", api_sta);
      }
      break;
    case API_BUSY2:
        api_sta = API_FREE;
        //放開SET鍵進入修改設定值, 故清除長短按計數
        KeySet.LongPressed = 0;
        KeySet.shortPressed = 0;
        printf("檢查KeySet.Cnt = %d\r\n", KeySet.Cnt);
        printf("API_BUSY1釋放旗標 = %d\r\n", api_sta);
      break;
    
    default:
      break;
  }
 }

}

//上鍵相關的API
static void activateEnhancedCooling(void)
{
  api_sta = API_BUSY1;
  icon.Enhanced_Cooling_sta = icon_EC_enable;
}

static void keyupshortpressed(void)
{
  //上鍵短按時, 各模式下功能
  clear_Recordmode_sta();   //為了避免卡在歷史低點畫面

  switch (Syscfg.mode)
  {
    case homeMode:
      Recordmode_High = true;
      Syscfg.mode = recordMode;
    break;

    case recordMode:
      Recordmode_High = false;
      Syscfg.mode = homeMode;
    break;

    case level1Mode:
      levelmode_handle(&sFlag.Level1_value, true);
    break;

    case level2Mode:
      levelmode_handle(&sFlag.Level2_value, true);
    break;

    case settingMode:
      pre_table[Set]++;
      pre_table[Set] = check_Limit_Value(pre_table[Set], Set);
    break;

    default:
      clear_Recordmode_sta();   //為了避免卡在歷史低點畫面
    break;
  }
  KeyUp.shortPressed = 0;
}

static void keyupconti_pressed(void)
{
  int16_t data_bytetable;
  int8_t pr1_index, pr2_index;
  static uint32_t lastIncrementTime_up = 0;
  uint8_t AUTO_INCREMENT_DELAY;

  //檢測是否為連加, 數值大於onF的只可讀不可寫
  {
    //依據長按的時間不同, 按越久間隔越短
    if(KeyUp.Cnt > CONTI_PRESS_times_H)
      AUTO_INCREMENT_DELAY = AUTO_INCREMENT_DELAY_H;
    else if(KeyUp.Cnt > CONTI_PRESS_times_M)
      AUTO_INCREMENT_DELAY = AUTO_INCREMENT_DELAY_M;
    else
      AUTO_INCREMENT_DELAY = AUTO_INCREMENT_DELAY_L;

    //檢測是否到了自動累加的時間間隔
    if(tmr.Cnt_1ms - lastIncrementTime_up >= AUTO_INCREMENT_DELAY)
    {
      lastIncrementTime_up = tmr.Cnt_1ms;  //更新累加時間
      if((Syscfg.mode==level1Mode) && (pr1_index<=onF))
      {
        //處理長時間連加數值的動作
        if(sFlag.Level1_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是pre_table
          pr1_index = bytetable_pr1[Syscfg.level1_index];
          if(pr1_index <= onF)
            pre_table[pr1_index]++;    //onF以後的參數只能讀不能改

          //檢查最大最小值, index要放pr1的不是總表的
          // printf("-------------連加測試開始-------------\r\n");
          data_bytetable = pre_table[pr1_index];
          // printf("data_bytetable = %d\r\n", data_bytetable);
          pre_table[pr1_index] = check_Limit_Value(data_bytetable, pr1_index);
          // printf("keyup數值 = %d\r\n", pre_table[pr1_index]);
        }
      }
      else if((Syscfg.mode==level2Mode) && (pr2_index<=onF))
      {
        //處理長時間連加數值的動作
        if(sFlag.Level2_value == Vvalue)
        {
          //要被修改的index應該是pr2的table, 而不是Syscfg.value的table
          pr2_index = bytetable_pr2[Syscfg.level2_index];
          if(pr2_index <= onF)
            pre_table[pr2_index]++;    //onF以後的參數只能讀不能改

          //檢查最大最小值, index要放pr1的不是總表的
          // printf("-------------連加測試開始-------------\r\n");
          data_bytetable = pre_table[pr2_index];
          // printf("data_bytetable = %d\r\n", data_bytetable);
          pre_table[pr2_index] = check_Limit_Value(data_bytetable, pr2_index);
          // printf("keyup數值 = %d\r\n", pre_table[pr1_index]);
        }
      }
      else if(Syscfg.mode==settingMode)
      {
        pre_table[Set]++;
        pre_table[Set] = check_Limit_Value(pre_table[Set], Set);
      }
    }
  }
}

static void keyupclearcnt(void)
{
  switch (api_sta)
  {
    case API_BUSY1:
      KeyUp.LongPressed = 0;
      KeyUp.shortPressed = 0;
      KeyUp.conti_pressed = 0;
      api_sta = API_FREE;
      break;

    default:
      break;
  }
}

static void key_up_function(void)
{
  /*[上鍵功能]
  * Home狀態下, 長按3秒進入加強製冷模式(舉起flag)
  * Home狀態下, 單擊可查看最大值
  * Menu狀態下, 單擊向下(參數表)瀏覽參數
  * Setting狀態下, 單擊增加參數值
  * [組合鍵功能]
  * [上+下] = 鎖定or解鎖鍵盤
  * api_sta代表目前是否有觸發此按鍵功能
  */

  if(api_sta==API_FREE)
  {
    if((KeyUp.Cnt>KEY_cnt_3s) && (Syscfg.mode==homeMode))
      activateEnhancedCooling();
    else if(KeyUp.shortPressed != 0)
      keyupshortpressed();
    else if(KeyUp.conti_pressed==true)
      keyupconti_pressed();
  }
  else if(KeyUp.Cnt==0)
    keyupclearcnt();
}

//下鍵相關的API
static void keydownshortpressed(void)
{
  clear_Recordmode_sta();
  switch (Syscfg.mode)
  {
    case homeMode:
      Recordmode_Low = true;
      Syscfg.mode = recordMode;
    break;

    case recordMode:
      Recordmode_Low = false;
      Syscfg.mode = homeMode;
    break;

    case level1Mode:
      levelmode_handle(&sFlag.Level1_value, false);
    break;

    case level2Mode:
      levelmode_handle(&sFlag.Level2_value, false);
    break;

    case settingMode:
      pre_table[Set]--;
      pre_table[Set] = check_Limit_Value(pre_table[Set], Set);
    break;

    default:
      clear_Recordmode_sta();
    break;
  }
  KeyDown.shortPressed = 0;
}

static void keydownconti_pressed(void)
{
  int16_t data_bytetable;
  int8_t pr1_index, pr2_index;
  static uint32_t lastIncrementTime_down = 0;
  uint8_t AUTO_INCREMENT_DELAY;

  //檢測是否為連減, 數值大於onF的只可讀不可寫
  {
    //依據長按的時間不同, 按越久間隔越短
    if(KeyDown.Cnt > CONTI_PRESS_times_H)
      AUTO_INCREMENT_DELAY = AUTO_INCREMENT_DELAY_H;
    else if(KeyDown.Cnt > CONTI_PRESS_times_M)
    {
      //主要看一下間隔多久, 發現放在100ms內的話大約是200ms跑一次
      // printf("tmr.Cnt_1ms: %d\r\n", tmr.Cnt_1ms);
      // printf("lastIncrementTime_down: %d\r\n", lastIncrementTime_down);
      AUTO_INCREMENT_DELAY = AUTO_INCREMENT_DELAY_M;
    }
    else
      AUTO_INCREMENT_DELAY = AUTO_INCREMENT_DELAY_L;

    //檢測是否到了自動累減的時間間隔
    if (tmr.Cnt_1ms - lastIncrementTime_down >= AUTO_INCREMENT_DELAY)
    {
      lastIncrementTime_down = tmr.Cnt_1ms;  //更新累加時間
      if(Syscfg.mode==level1Mode)
      {
        //處理長時間連減數值的動作
        if(sFlag.Level1_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是Syscfg.value的table
          pr1_index = bytetable_pr1[Syscfg.level1_index];
          if(pr1_index<=onF)
          {
            pre_table[pr1_index]--;

            //檢查最大最小值, index要放pr1的不是總表的
            data_bytetable = pre_table[pr1_index];
            // printf("-------------連加測試開始-------------\r\n");
            // printf("data_bytetable = %d\r\n", data_bytetable);
            pre_table[pr1_index] = check_Limit_Value(data_bytetable, pr1_index);
            // printf("keyup數值 = %d\r\n", pre_table[pr1_index]);
          }
        }
      }
      else if(Syscfg.mode==level2Mode)
      {
        //處理長時間連加數值的動作
        if(sFlag.Level2_value == Vvalue)
        {
          //要被修改的index應該是pr1的table, 而不是Syscfg.value的table
          pr2_index = bytetable_pr2[Syscfg.level2_index];
          if(pr2_index<=onF)
          {
            pre_table[pr2_index]--;

            //檢查最大最小值, index要放pr1的不是總表的
            data_bytetable = pre_table[pr2_index];
            // printf("-------------連加測試開始-------------\r\n");
            // printf("data_bytetable = %d\r\n", data_bytetable);
            pre_table[pr2_index] = check_Limit_Value(data_bytetable, pr2_index);
            // printf("keyup數值 = %d\r\n", pre_table[pr1_index]);
          }
        }
      }
      else if(Syscfg.mode==settingMode)
      {
        pre_table[Set]--;
        pre_table[Set] = check_Limit_Value(pre_table[Set], Set);
      }
    }
  }

}

static void keydownclearcnt(void)
{
  KeyDown.LongPressed = 0;
  KeyDown.shortPressed = 0;
  KeyDown.conti_pressed = 0;
}

static void key_down_function(void)
{
  /*[下鍵功能]
  * Home狀態下, 單擊可查看最小值
  * Menu狀態下, 單擊向上(參數表)瀏覽參數
  * Setting狀態下, 單擊減少參數值
  */

  if(api_sta==API_FREE)
  {
    if(KeyDown.shortPressed)
      keydownshortpressed();
    else if(KeyDown.conti_pressed==true)
      keydownconti_pressed();
  }
  else
    keydownclearcnt();
}

//待機鍵相關的API
static void keystandbyshortpressed(void)
{
  switch (Syscfg.mode)
  {
    case recordMode:
      clear_Recordmode_sta();
      Syscfg.mode = homeMode;
    break;
  
  default:
    break;
  }
  KeyStandby.shortPressed = 0;
}

static void keystandbyclearcnt(void)
{
  KeyStandby.LongPressed = 0;
  KeyStandby.shortPressed = 0;
  KeyStandby.conti_pressed = 0;
}

static void key_standby_function(void)
{
  /*[待機鍵功能] TODO:功能還沒做
  * 在參數 onF=oFF時, 單擊進入待機功能;
  * 在參數 onF=ES時, 單擊進入節能運行功能;
  */

  if(KeyStandby.shortPressed)
    keystandbyshortpressed();
  else
    keystandbyclearcnt();
}

//燈泡鍵相關的API
static void keybulbshortpressed(void)
{
  switch (Syscfg.mode)
  {
    case recordMode:
      Syscfg.keymode.Max_flag = false;
      Syscfg.keymode.Min_flag = false;
      Syscfg.mode = homeMode;
    break;
  
  default:
    break;
  }
  KeyBulb.shortPressed = 0;
}

static void keybulbclearcnt(void)
{
  KeyBulb.LongPressed = 0;
  KeyBulb.shortPressed = 0;
  KeyBulb.conti_pressed = 0;
}

static void key_bulb_function(void)
{
  /*[燈開關鍵功能]
  * 此鍵不可用(無作用)
  */

 if(KeyBulb.shortPressed)
   keybulbshortpressed();
}

//融霜鍵相關的API
static void keydefrostlongpressed(void)
{
  keydefrostclearcnt();
}

static void keydefrostshortpressed(void)
{
  switch (Syscfg.mode)
  {
    case homeMode:
      sFlag.Defrost = true;
    break;

    case recordMode:
      clear_Recordmode_sta();
      Syscfg.mode = homeMode;
    break;
  
  default:
    break;
  }
  KeyDefrost.shortPressed = 0;
}

static void keydefrostclearcnt(void)
{
  KeyDefrost.LongPressed = 0;
  KeyDefrost.shortPressed = 0;
  KeyDefrost.conti_pressed = 0;
}

static void key_defrost_function(void)
{
  /*[融霜鍵功能]
  * Home狀態下, 單擊啟動一次手動融霜
  * Menu狀態下, 無作用
  */

 if(KeyDefrost.LongPressed != 0)
  keydefrostlongpressed();
 else if(KeyDefrost.shortPressed != 0)
  keydefrostshortpressed();
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
