/**
 ******************************************************************************
 * @file    Display.c
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 * 調用應用層指令, 組合成使用者手冊的功能
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "INC/board_interface/board_layer.h"
#include "INC/framework/Display.h"

/* extern variables -----------------------------------------------------------------*/
extern __IO r_tmr tmr;

/* variables -----------------------------------------------------------------*/
static bool min_f = false;
static const uint8_t ByteTable[][3] = 
{
  {dig_off,   dig_off,    dig_off},    // 0 xxx
  //7.1 調節參數
  {dig_H,     dig_y,      dig_off},    // 1 Hy 
  {dig_L,     dig_S,      dig_off},    // 2 LS 
  {dig_U,     dig_S,      dig_off},    // 3 US 
  {dig_o,     dig_t,      dig_off},    // 4 ot 
  {dig_P,       dig_2,      dig_P},    // 5 P2P
  {dig_o,     dig_E,      dig_off},    // 6 oE 
  {dig_P,       dig_3,      dig_P},    // 7 P3P
  {dig_o,     dig_3,      dig_off},    // 8 o3 
  {dig_P,       dig_4,      dig_P},    // 9 P4P
  {dig_o,     dig_4,      dig_off},    //10 o4 
  {dig_o,       dig_d,      dig_S},    //11 odS
  {dig_A,     dig_C,      dig_off},    //12 AC 
  {dig_r,       dig_t,      dig_r},    //13 rtr
  {dig_C,       dig_C,      dig_t},    //14 CCt
  {dig_C,       dig_C,      dig_S},    //15 CCS
  {dig_C,       dig_o,      dig_n},    //16 Con
  {dig_C,       dig_o,      dig_F},    //17 CoF
  //7.2 顯示參數
  {dig_C,     dig_F,      dig_off},    // 1 CF 
  {dig_r,       dig_E,      dig_S},    // 2 rES
  {dig_L,       dig_o,      dig_d},    // 3 Lod
  {dig_r,       dig_E,      dig_d},    // 4 rEd   --TODO:影片中沒看到
  {dig_d,       dig_L,      dig_y},    // 5 dLy
  {dig_d,       dig_t,      dig_r},    // 6 dtr
  //7.3 融霜參數
  {dig_d,       dig_F,      dig_P},    // 1 dFP   --TODO:影片中與tdF換順序
  {dig_t,       dig_d,      dig_F},    // 2 tdF   --TODO:影片中與dFP換順序
  {dig_d,       dig_t,      dig_E},    // 3 dtE
  {dig_i,       dig_d,      dig_F},    // 4 idF
  // {dig_d,       dig_i,      dig_d},    // 5 did   --TODO:影片中沒看到
  // {dig_n,       dig_P,      dig_S},    // 6 nPS   --TODO:影片中沒看到
  // {dig_O,       dig_d,      dig_C},    // 7 OdC   --TODO:影片中沒看到
  // {dig_r,       dig_r,      dig_d},    // 8 rrd   --TODO:影片中沒看到
  // {dig_H,       dig_E,      dig_S},    // 9 HES   --TODO:影片中沒看到
  {dig_M,       dig_d,      dig_F},    //10 MdF
  {dig_d,       dig_S,      dig_d},    //11 dSd
  {dig_d,       dig_F,      dig_d},    //12 dFd
  {dig_d,       dig_A,      dig_d},    //13 dAd
  {dig_F,       dig_d,      dig_t},    //14 Fdt
  {dig_d,       dig_P,      dig_o},    //15 dPo
  {dig_d,       dig_A,      dig_F},    //16 dAF
  //7.4 蒸發器風扇參數
  {dig_F,       dig_n,      dig_C},    // 1 FnC
  {dig_F,       dig_n,      dig_d},    // 2 Fnd
  {dig_F,       dig_C,      dig_t},    // 3 FCt
  {dig_F,       dig_S,      dig_t},    // 4 FSt
  {dig_F,       dig_o,      dig_n},    // 5 Fon
  {dig_F,       dig_O,      dig_F},    // 6 FOF
  {dig_F,       dig_A,      dig_P},    // 7 FAP
  //7.5 報警參數
  {dig_A,       dig_L,      dig_C},    // 1 ALC
  {dig_A,       dig_L,      dig_U},    // 2 ALU
  {dig_A,       dig_L,      dig_L},    // 3 ALL
  {dig_A,       dig_F,      dig_H},    // 4 AFH
  {dig_A,       dig_L,      dig_d},    // 5 ALd
  {dig_d,       dig_A,      dig_o},    // 6 dAo
  //7.6 冷凝溫度報警
  {dig_A,       dig_P,      dig_2},    // 1 AP2
  {dig_A,       dig_L,      dig_2},    // 2 AL2
  {dig_A,       dig_U,      dig_2},    // 3 AU2
  {dig_A,       dig_H,      dig_2},    // 4 AH2
  {dig_A,       dig_d,      dig_2},    // 5 Ad2
  {dig_d,       dig_A,      dig_2},    // 6 dA2
  {dig_b,       dig_L,      dig_L},    // 7 bLL
  {dig_A,       dig_C,      dig_2},    // 8 AC2
  //7.7 數字輸入參數
  {dig_i,       dig_1,      dig_P},    // 1 i1P
  {dig_i,       dig_1,      dig_F},    // 2 i1F
  {dig_d,       dig_i,      dig_d},    //63 did
  {dig_n,       dig_P,      dig_S},    //64 nPS
  {dig_O,       dig_d,      dig_C},    //65 OdC
  {dig_r,       dig_r,      dig_d},    //66 rrd
  {dig_H,       dig_E,      dig_S},    //67 HES
  //7.8 其他參數
  {dig_A,       dig_d,      dig_r},    // 1 Adr
  {dig_P,       dig_b,      dig_C},    // 2 PbC
  {dig_O,       dig_n,      dig_F},    // 3 OnF
  {dig_d,       dig_P,      dig_1},    // 4 dp1
  {dig_d,       dig_P,      dig_2},    // 5 dp2
  {dig_d,       dig_P,      dig_3},    // 6 dp3
  {dig_d,       dig_P,      dig_4},    // 7 dp4
  {dig_r,       dig_S,      dig_E},    // 8 rSE
  {dig_r,       dig_E,      dig_L},    // 9 rEL
  {dig_P,       dig_t,      dig_b},    //10 Ptb
};

static const uint8_t LED_0F[] =    // show字節
{ 
  //dp, g, f, e, d, c, b, a (7段顯示器bit值排序轉換16進制)

  // 0     1     2     3     4     5     6     7     8     9   
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
  //A(10) b(11) C(12) d(13) E(14) F(15) 16off 17on  18dp
    0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00, 0xFF, 0x80,
  //G(19) H(20) K(21) L(22) n(23) o(24) P(25) r(26) t(27)
    0x3D, 0x76, 0x38, 0x38, 0x54, 0x5C, 0x73, 0x50, 0x78,
  //U(28) V(29) Y(30) -(31) i(32) W(33) J(34) _(35) c(36)
    0x3E, 0x1C, 0x6E, 0x40, 0x30, 0x3E, 0x0E, 0x08, 0x58,
  //d(37) u(38) M(39)
    0x5E, 0x1C, 0x37
};
volatile uint8_t m,v595_scan, scan, mode;                    //5950為給第一顆595的SER值；5951為給第二顆SER的值
volatile uint8_t dsp[6] = {0};

volatile uint16_t dp;
__IO uint16_t r_agt_1ms_cnt, r_agt_5ms_cnt, r_agt_1s_cnt;
__IO bool r_agt_1ms_flag;
__IO uint8_t testmode;
__IO uint16_t index_cnt, TEST_cnt[4];

//key
  bsp_io_level_t readlevel;
  fsp_err_t status,Full;
  volatile uint8_t keylevel,nkey[5],keyin,keycnt[5];

/* Private function protocol -----------------------------------------------*/
static void led_init(void);
static void single_led_test(void);
static void all_led_scan(void);
static void setSegment(char segment, bool state);
static void LED_all_ON(void);
static void set_data_to_pin(uint8_t pin_data);


static void LED_display(void);

/* Function definitions ------------------------------------------------------*/
void ShowTable(uint8_t table)
{
  dsp[Scan_2] = dig_minus; //百位
  dsp[Scan_1] = dig_minus; //十位
  dsp[Scan_0] = dig_minus; //十位

//table跟scan的位置沒有對到, 所以要轉換一下
  if(table != xxx)
  {
    dsp[Scan_2] = ByteTable[table][table_2];
    dsp[Scan_1] = ByteTable[table][table_1];
    dsp[Scan_0] = ByteTable[table][table_0];
  }
}

void ShowNumber(int32_t num)
{
  uint16_t value;
  uint8_t digi = 0;
  bool IsNotFirstDigi = false;
  /**
   * 數字有負數, xx.x, 一律放大10倍為xxx, 
   * 例如12.3 放大為 123
   * **/

  //設定十, 百位數的初始值為不顯示
  dsp[4] = dig_off; //icon
  dsp[3] = dig_off; //icon
  dsp[Scan_2] = dig_off; //百位
  dsp[Scan_1] = dig_off; //十位
  dsp[Scan_0] = dig_off; //個位

  //判斷正負
  min_f = (num<0)? true:false;
 
  // ------- whether Temp is negative -------//
  if(min_f == 1)
    num = num*(-1); //turn negative number to positive number

  num = (num > 999)? 999:num;
  value = num;
  //轉換數字  個十百
  //百位數
  digi = value / 100;
  if((digi > 0) || IsNotFirstDigi)
  {
    value = num / 100;
    dsp[Scan_2] = value % 10;
    IsNotFirstDigi = true;
  }
  //十位數
  digi = value / 10;
  if((digi > 0) || IsNotFirstDigi)
  {
    value = num / 10;
    dsp[Scan_1] = value % 10;
    IsNotFirstDigi = true;
  }
  //個位數
  dsp[Scan_0] = num%10;
}


/* Static Function definitions ------------------------------------------------------*/
void LED_Display_main(void)
{
  led_init();

  testmode=2;
  switch (testmode)
  {
    case 0:
      single_led_test();
      TEST_cnt[0]++;
      break;
    case 1:
      all_led_scan();
      TEST_cnt[1]++;
      break;
    case 2:
      LED_display();
      TEST_cnt[2]++;
      break;

    default:
      TEST_cnt[3]++;
      break;
  }
}

/*
  顯示邏輯說明及測試步驟, **需確認595推0的時候為Enable還是推1為Enable**, (硬體設計邏輯)
  以後如果還有595重新開發, 建議使用spi串口, 硬體執行效率更佳
  1. 8888, 共4個7段顯示器
  2. 上層3AF21B, 下層EDHCG0 (A~H為PIN, 0~3為INDEX)
  3. 設定map值(應用層), 轉換成595值, 更換595buffer
  4. 顯示掃描每1ms持續進行

//以下測試以硬體設計1為Enable執行
  測試1: 點亮Index=0(最左的8)
  測試2: 顯示4個6
  測試3: 以正規模式顯示數字(1234等等)
  測試4: 以正規模式顯示字節
*/

static void led_init(void)
{
  static bool firsttime = true;
  if(firsttime == true)
  {
    SMG_1_CLOSE;
    SMG_2_CLOSE;
    SMG_3_CLOSE;
    SMG_4_CLOSE;
    SMG_5_CLOSE;
    SMG_6_CLOSE;

    SMG_A_CLOSE;
    SMG_B_CLOSE;
    SMG_C_CLOSE;
    SMG_D_CLOSE;
    SMG_E_CLOSE;
    SMG_F_CLOSE;
    SMG_G_CLOSE;
    SMG_DP_CLOSE;
    firsttime = false;
  }
}

static void single_led_test(void)
{
  static uint8_t seg=4;
  uint8_t S_end=0;
  switch (S_end)
  {
    case 0:
      SMG_6_CLOSE;
      SMG_1_OPEN;
      break;
    case 1:
      SMG_1_CLOSE;
      SMG_2_OPEN;
      break;
    case 2:
      SMG_2_CLOSE;
      SMG_3_OPEN;
      break;
    case 3:
      SMG_3_CLOSE;
      SMG_4_OPEN;
      break;
    case 4:
      SMG_4_CLOSE;
      SMG_5_OPEN;
      break;
    case 5:
      SMG_5_CLOSE;
      SMG_6_OPEN;
      break;
    
    default:
      break;
  }

  switch (seg)
  {
    case 0:
      SMG_DP_CLOSE;
      SMG_A_OPEN;
      break;
    case 1:
      SMG_A_CLOSE;
      SMG_B_OPEN;
      break;
    case 2:
      SMG_B_CLOSE;
      SMG_C_OPEN;
      break;
    case 3:
      SMG_C_CLOSE;
      SMG_D_OPEN;
      break;
    case 4:
      SMG_D_CLOSE;
      SMG_E_OPEN;
      break;
    case 5:
      SMG_E_CLOSE;
      SMG_F_OPEN;
      break;
    case 6:
      SMG_F_CLOSE;
      SMG_G_OPEN;
      break;
    case 7:
      SMG_G_CLOSE;
      SMG_DP_OPEN;
      break;
    
    default:
      break;
  }
}

static void all_led_scan(void)
{
  setSegment('a', true);   //全亮

  switch (tmr.Cnt_1s%6)
  {
    case 0:
      SMG_6_CLOSE;
      SMG_1_OPEN;
      break;
    case 1:
      SMG_1_CLOSE;
      SMG_2_OPEN;
      break;
    case 2:
      SMG_2_CLOSE;
      SMG_3_OPEN;
      break;
    case 3:
      SMG_3_CLOSE;
      SMG_4_OPEN;
      break;
    case 4:
      SMG_4_CLOSE;
      SMG_5_OPEN;
      break;
    case 5:
      SMG_5_CLOSE;
      SMG_6_OPEN;
      break;
    
    default:
      break;
  }
}

static void setSegment(char segment, bool state)
{
  switch(segment)
  {
    case 'a':
      if(state)
        SMG_A_OPEN;
      else
        SMG_A_CLOSE;
      break;
    case 'b':
      if(state)
        SMG_B_OPEN;
      else
        SMG_B_CLOSE;
      break;
    case 'c':
      if(state)
        SMG_C_OPEN;
      else
        SMG_C_CLOSE;
      break;
    case 'd':
      if(state)
        SMG_D_OPEN;
      else
        SMG_D_CLOSE;
      break;
    case 'e':
      if(state)
        SMG_E_OPEN;
      else
        SMG_E_CLOSE;
      break;
    case 'f':
      if(state)
        SMG_F_OPEN;
      else
        SMG_F_CLOSE;
      break;
    case 'g':
      if(state)
        SMG_G_OPEN;
      else
        SMG_G_CLOSE;
      break;
    case 'dp':
      if(state)
        SMG_DP_OPEN;
      else
        SMG_DP_CLOSE;
      break;
      
    default:
        SMG_A_OPEN;
        SMG_B_OPEN;
        SMG_C_OPEN;
        SMG_D_OPEN;
        SMG_E_OPEN;
        SMG_F_OPEN;
        SMG_G_OPEN;
        SMG_DP_OPEN;
      break;
  }
}

static void set_data_to_pin(uint8_t pin_data)
{
  (pin_data & 0x01) ? SMG_A_OPEN : SMG_A_CLOSE;  
  (pin_data & 0x02) ? SMG_B_OPEN : SMG_B_CLOSE;
  (pin_data & 0x04) ? SMG_C_OPEN : SMG_C_CLOSE;
  (pin_data & 0x08) ? SMG_D_OPEN : SMG_D_CLOSE;  
  (pin_data & 0x10) ? SMG_E_OPEN : SMG_E_CLOSE;
  (pin_data & 0x20) ? SMG_F_OPEN : SMG_F_CLOSE;
  (pin_data & 0x40) ? SMG_G_OPEN : SMG_G_CLOSE;
  (pin_data & 0x80) ? SMG_DP_OPEN : SMG_DP_CLOSE;
}

uint8_t pin_data=0, seg_data=0;
static void LED_display(void)
{
  m=mode%5;
  switch(m)
  {
    case 0:
      SMG_5_CLOSE;
      SMG_1_OPEN;
    break;
    case 1:
      SMG_1_CLOSE;
      SMG_2_OPEN;
    break;
    case 2:
      SMG_2_CLOSE;
      SMG_3_OPEN;
    break;
    case 3:
      SMG_3_CLOSE;
      SMG_4_OPEN;
    break;
    case 4:
      SMG_4_CLOSE;
      SMG_5_OPEN;
    break;
    default: break;
  }
  seg_data = dsp[m];    //顯示各位數數字, 位置的值
  pin_data = LED_0F[seg_data];
  // ------- whether Temp is negative -------//
  if((min_f==1) && (m==0))
    pin_data |= 0x80; //顯示負號(-), 負號是個位數的dp
  set_data_to_pin(pin_data); //拉各7-seg的腳位
  mode++;
}

