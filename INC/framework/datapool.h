/**
 ******************************************************************************
 * @file    datapool.h
 * @author  KCE R&D Team
 * @date    2024-08-29
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */


#define INC_DATAPOOL_H_ 
#ifdef INC_DATAPOOL_H_

/* Enum type definitions -----------------------------------------------------*/
enum Levelflag
{
  Vindex,
  Vvalue,
  Pr2_symbol,
};

enum MenuLevel
{
  NaN,    //not a number
  Pr1,    //第一層可以看到的參數
  Pr2,    //第二層中才能看到的參數, 進入第二層可看到所有參數
};

enum DegreeUnit
{
  degree_C,
  degree_F,
};

enum dashcover
{
  disp_P1,
  disp_P2,
  disp_P3,
  disp_P4,
  disp_SET,
  disp_dtr,
};

enum sensorchoose
{
  defrost_np,
  defrost_p1,
  defrost_p2,
  defrost_p3,
  defrost_p4,
};

enum defrosttype
{
  type_EL,
  type_in,
};

enum defrostshow
{
  defrost_rt,
  defrost_it,
  defrost_SET,
  defrost_dEF,
};

enum defrostwork
{
  work_no,
  work_yes,
};

enum fanwork
{
  fan_c_n,
  fan_o_n,
  fan_c_y,
  fan_o_y,
};

enum alarmset
{
  alarm_rE,
  alarm_Ab,
};

enum polartype
{
  polar_CL,
  polar_OP,
};

enum alarmdelay
{
  alarm_EAL,
  alarm_bAL,
  alarm_PAL,
  alarm_dor,
  alarm_dEF,
  alarm_AUS,
  alarm_Htr,
  alarm_FAn,
  alarm_ES,
};

enum outstate
{
  state_no,
  state_Fan,
  state_CPr,
  state_F_C,
};

enum sensortype
{
  type_Ptc,
  type_ntc,
};

enum standbybutton
{
  button_nu,
  button_oFF,
  button_ES,
};

/* typedef definitions -----------------------------------------------------*/
typedef struct
{
  uint8_t UserByte;
  float Range_Low;
  float Range_High;
  float DefaultValue;
  uint8_t Mode;
} ByteSettingTable;

/* task function protocol -----------------------------------------------*/
void offset_EEtoSYS(void);
void get_Pv(void);
void get_HistoryMax(void);
void get_HistoryMin(void);
uint8_t get_bytetable_pr1(void);
uint8_t get_bytetable_pr2(void);
int16_t check_Limit_Value(int16_t data, int8_t index);
int16_t celsius_to_fahrenheit(int16_t pv);
void upload_syscfg_data(int8_t pr_index);

#endif /* INC_DATAPOOL_H_ */