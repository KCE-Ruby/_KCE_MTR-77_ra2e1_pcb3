/*
 * i2c_ee.h
 *
 *  Created on: 2024年7月30日
 *      Author: steve
 */

#ifndef EEPROM_I2C_EE_H_
#define EEPROM_I2C_EE_H_

#define EEPROM_PAGESIZE           8
#define I2C_Register_SR1                ((uint8_t)0x14)

/* Enum type definitions -----------------------------------------------------*/
enum eeprom_index
{
  //每個參數都有2個byte的儲存位置, 最高位元0x8000用來做正負值得判斷, true則代表負數, false代表正數
  eep_Set_low,
  eep_Set_high,
  eep_Hy_low,
  eep_Hy_high,

  //以下參數因為沒有預設值, 所以不會被reset, 只能手動清除
  //若要新增記得要在datapool.c的bytetable陣列內新增數值
  eep_max_low,
  eep_max_high,
  eep_min_low,
  eep_min_high,

  eep_end,
};

enum eeprom_datasize
{
  eep_size_max = 2,
  eep_size_min = 2,

  eep_size_end,
};

/* typedef definitions -----------------------------------------------------*/
typedef struct
{
  

};

/* task function protocol -----------------------------------------------*/
void i2c_callback (i2c_master_callback_args_t * p_args);
void I2C_EE_Init();
void I2C_EE_ByteWrite(unsigned char address, unsigned char byte);
void I2C_EE_Writepage(unsigned char* ptr_write , unsigned char WriteAddr,unsigned char len);
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr,uint16_t NumByteToWrite);
void I2C_EE_BufferRead(unsigned char* ptr_read,unsigned char address,unsigned char byte);
void I2C_EE_WaitState(void);
void I2C_EE_Writederase();
uint8_t I2C_Test(void);


#endif /* EEPROM_I2C_EE_H_ */
