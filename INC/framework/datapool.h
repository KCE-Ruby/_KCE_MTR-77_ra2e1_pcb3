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
enum
{
  NaN,    //not a number
  Pr1,    //第一層可以看到的參數
  Pr2,    //第二層中才能看到的參數, 進入第二層可看到所有參數
}

/* typedef definitions -----------------------------------------------------*/
typedef struct
{
  uint8_t UserByte;
  float Range_Low;
  float Range_High;
  float DefaultValue;
  uint8_t Mode;
} ByteSettingTable;


#endif /* INC_DATAPOOL_H_ */