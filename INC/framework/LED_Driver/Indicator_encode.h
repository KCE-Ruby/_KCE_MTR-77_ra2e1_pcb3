/**
 ******************************************************************************
 * @file    Indicator_encode.h
 * @author  KCE R&D Team
 * @date    2024-06-26
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_INDICATOR_ENCODE_H_ 
#ifdef INC_INDICATOR_ENCODE_H_

/* Enum type definitions -----------------------------------------------------*/
enum digitis
{
  dig_0 =  0,
  dig_1 =  1,
  dig_2 =  2,
  dig_3 =  3,
  dig_4 =  4,
  dig_5 =  5,
  dig_6 =  6,
  dig_7 =  7,
  dig_8 =  8,
  dig_9 =  9,

  dig_A = 10,
  dig_b = 11,
  dig_C = 12,
  dig_c = 13,
  dig_d = 14,
  dig_E = 15,
  dig_F = 16,
  dig_G = 17,
  dig_H = 18,
  dig_I = 19,
  dig_i = 20,
  dig_L = 21,
  dig_M = 22,
  dig_n = 23,
  dig_O = 24,
  dig_o = 25,
  dig_P = 26,
  dig_r = 27,
  dig_S = 28,
  dig_t = 29,
  dig_U = 30,
  dig_u = 31,
  dig_y = 32,

  dig_off = 33,
  dig_on = 34,
  dig_minus = 35,
  dig_underscore = 36,
};

/* typedef definitions -----------------------------------------------------*/
typedef union
{
  struct
  {
    bool M1_a:1;
    bool M1_b:1;
    bool M1_c:1;
    bool M1_d:1;
    bool M1_e:1;
    bool M1_f:1;
    bool M1_g:1;
    bool M1_minus:1;
  }scan1;
  uint8_t pincombine;
} LED_SCAN1;

typedef union
{
  struct
  {
    bool M2_a:1;
    bool M2_b:1;
    bool M2_c:1;
    bool M2_d:1;
    bool M2_e:1;
    bool M2_f:1;
    bool M2_g:1;
    bool M2_dp:1;
  }scan2;
  uint8_t pincombine;
} LED_SCAN2;

typedef union
{
  struct
  {
    bool M3_a:1;
    bool M3_b:1;
    bool M3_c:1;
    bool M3_d:1;
    bool M3_e:1;
    bool M3_f:1;
    bool M3_g:1;
    bool M3_dp:1;
  }scan3;
  uint8_t pincombine;
} LED_SCAN3;

typedef union
{
  struct
  {
    bool bulb:1;
    bool defrost:1;
    bool energy_saving:1;
    bool alarm:1;
    bool refrigerate:1;
    bool aux:1;
    bool enhanced_cooling:1;
    bool clock:1;
  }scan4;
  uint8_t pincombine;
} ICON_SCAN4;

typedef union
{
  struct
  {
    bool fan:1;
    bool ret_1:1;        //reserved
    bool degress_c:1;
    bool degress_f:1;
    uint8_t ret:4;        //reserved
  }scan5;
  uint8_t pincombine;
} ICON_SCAN5;

/* Public function protocol -----------------------------------------------*/
void NumToDisplay(int16_t temp);
void CharToDisplay(uint8_t _char);

#endif /* INC_INDICATOR_ENCODE_H_ */