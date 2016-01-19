#ifndef _MY_I2C_H
#define _MY_I2C_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#define ACC_I2C_ADDRESS                      0x32
#define MAG_I2C_ADDRESS                      0x3C

extern void i2c_init(void);
extern void i2c_read_bytes(uint16_t DeviceAddr,uint8_t RegAddr,uint8_t* pBuffer,u16 len);
extern void i2c_wirte_byte(uint16_t DeviceAddr,uint8_t RegAddr,uint8_t REG_data);
extern void LSM303DLHC_accInit(void);
#endif