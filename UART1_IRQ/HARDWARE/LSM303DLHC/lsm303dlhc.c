#include"lsm303dlhc.h"
#include"accelero.h"
#include"my_i2c.h"

// LSM303DLHC芯片初始化ACC(加速度)寄存器函数

void LSM303DLHC_AccInit(void){
  ACCELERO_InitTypeDef LSM303DLHCAcc_InitStructure;
	ACCELERO_FilterConfigTypeDef LSM303DLHCFilter_InitStructure;
	
	uint8_t ctrl1 = 0x00,ctrl2=0x00,ctrl4 = 0x00;
	//ctrl1设置
	LSM303DLHCAcc_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE; //功耗模式
  LSM303DLHCAcc_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ; //输出速率
  LSM303DLHCAcc_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;  //使能三轴输出
	//ctrl2设置
	LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection =LSM303DLHC_HPM_NORMAL_MODE;//0x80
	LSM303DLHCFilter_InitStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;//0x10
	LSM303DLHCFilter_InitStructure.HighPassFilter_AOI1 = LSM303DLHC_HPF_AOI1_DISABLE;
	LSM303DLHCFilter_InitStructure.HighPassFilter_AOI2 = LSM303DLHC_HPF_AOI2_DISABLE; 
	//ctrl4设置
  LSM303DLHCAcc_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;  //0x00
  LSM303DLHCAcc_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous; //0x00
  LSM303DLHCAcc_InitStructure.Endianness=LSM303DLHC_BLE_LSB;  //0x00
  LSM303DLHCAcc_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;    //0x08
	i2c_init();
	
	ctrl1 |= (uint8_t) (LSM303DLHCAcc_InitStructure.Power_Mode | 
	  LSM303DLHCAcc_InitStructure.AccOutput_DataRate | LSM303DLHCAcc_InitStructure.Axes_Enable);

  ctrl2 |= (uint8_t) (LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection |
	  LSM303DLHCFilter_InitStructure.HighPassFilter_CutOff_Frequency |
		LSM303DLHCFilter_InitStructure.HighPassFilter_AOI1 | LSM303DLHCFilter_InitStructure.HighPassFilter_AOI2);
		
	ctrl4 |= (uint8_t) (LSM303DLHCAcc_InitStructure.BlockData_Update | 
	  LSM303DLHCAcc_InitStructure.Endianness | LSM303DLHCAcc_InitStructure.AccFull_Scale|
	  LSM303DLHCAcc_InitStructure.High_Resolution);   

	i2c_wirte_byte(ACC_I2C_ADDRESS ,LSM303DLHC_CTRL_REG1_A,ctrl1);//初始化REG1_A并配置
	i2c_wirte_byte(ACC_I2C_ADDRESS ,LSM303DLHC_CTRL_REG2_A,ctrl2);//初始化REG2_A，并配置
	i2c_wirte_byte(ACC_I2C_ADDRESS ,LSM303DLHC_CTRL_REG4_A,ctrl4);//初始化REG4_A寄存器
	
	
	
 }
 
 // LSM303DLHC芯片初始化MAG(磁力)寄存器函数
 
void LSM303DLHC_MagInit(void){
	Mag_InitTypeDef  LSM303DLHCMag_InitStructure;
  uint8_t cra_regm = 0x00, crb_regm = 0x00, mr_regm = 0x00;
	
	  
  LSM303DLHCMag_InitStructure.Temperature_Sensor = LSM303DLHC_TEMPSENSOR_DISABLE; //0x00
  LSM303DLHCMag_InitStructure.MagOutput_DataRate =LSM303DLHC_ODR_30_HZ ;    //0x14
  LSM303DLHCMag_InitStructure.MagFull_Scale = LSM303DLHC_FS_8_1_GA;    //0xE0
  LSM303DLHCMag_InitStructure.Working_Mode = LSM303DLHC_CONTINUOS_CONVERSION;    //0x00
  i2c_init();
	
  cra_regm |= (uint8_t) (LSM303DLHCMag_InitStructure.Temperature_Sensor | LSM303DLHCMag_InitStructure.MagOutput_DataRate);
  
  crb_regm |= (uint8_t) (LSM303DLHCMag_InitStructure.MagFull_Scale);  
  
  mr_regm |= (uint8_t)  (LSM303DLHCMag_InitStructure.Working_Mode);   
  
	i2c_wirte_byte(MAG_I2C_ADDRESS , LSM303DLHC_CRA_REG_M,cra_regm);//初始化A
  i2c_wirte_byte(MAG_I2C_ADDRESS , LSM303DLHC_CRB_REG_M,crb_regm);//初始化B
	i2c_wirte_byte(MAG_I2C_ADDRESS , LSM303DLHC_MR_REG_M, mr_regm);//初始化MR
}
 

//从LSM303DLHC中取出三轴磁力数值(Mag),并进行单位处理

  void Demo_CompassReadMag (int16_t* pfData)
{
  static uint8_t buffer[6] = {0};
  uint8_t CTRLB = 0;
  uint16_t Magn_Sensitivity_XY = 0, Magn_Sensitivity_Z = 0;
  uint8_t i =0;
  i2c_read_bytes(MAG_I2C_ADDRESS, LSM303DLHC_CRB_REG_M, &CTRLB, 1);
  
  i2c_read_bytes(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, buffer,   1);
  i2c_read_bytes(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M, buffer+1, 1);
  i2c_read_bytes(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M, buffer+2, 1);
  i2c_read_bytes(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M, buffer+3, 1);
  i2c_read_bytes(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M, buffer+4, 1);
  i2c_read_bytes(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M, buffer+5, 1);
 
  
  switch(CTRLB & 0xE0)
  {
  case LSM303DLHC_FS_1_3_GA:
   
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_3Ga; //设置为1100
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_3Ga;   //980
    break;
  case LSM303DLHC_FS_1_9_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_9Ga;//855
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_9Ga; //760
    break;
  case LSM303DLHC_FS_2_5_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_2_5Ga; //670
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_2_5Ga; //600
    break;
  case LSM303DLHC_FS_4_0_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4Ga; //450
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4Ga; //400
    break;
  case LSM303DLHC_FS_4_7_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4_7Ga; //400
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4_7Ga;//355
    break;
  case LSM303DLHC_FS_5_6_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_5_6Ga;//330
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_5_6Ga;//295
    break;
  case LSM303DLHC_FS_8_1_GA:
    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;//230
    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;//205
    break;
  }
 
  for(i=0; i<2; i++)
  {
    pfData[i]=((int16_t)(((uint16_t)buffer[2*i] << 8) + buffer[2*i+1])*1000)/Magn_Sensitivity_XY;
  }
  pfData[2]=((int16_t)(((uint16_t)buffer[4] << 8) + buffer[5])*1000)/Magn_Sensitivity_Z;
}
 
//从LSM303DLHC取出三轴加速度数值，并进行处理

void LSM303DLHC_AccReadXYZ(int16_t* pData)
{
  int16_t pnRawData[3];
  uint8_t ctrlx[2];
  uint8_t buffer[6];
  uint8_t i = 0;
  uint8_t sensitivity = LSM303DLHC_ACC_SENSITIVITY_2G;
  
  /* Read the acceleration control register content */
  i2c_read_bytes(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A,ctrlx,1);
	
  i2c_read_bytes(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A,buffer,  1);
  i2c_read_bytes(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_H_A,buffer+1,1);
	i2c_read_bytes(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_A,buffer+2,1);
	i2c_read_bytes(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_A,buffer+3,1);
	i2c_read_bytes(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_A,buffer+4,1);
  i2c_read_bytes(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_A,buffer+5,1);
  /* Read output register X, Y & Z acceleration */

   
  /* Check in the control register4 the data alignment*/
  if(!(ctrlx[0] & LSM303DLHC_BLE_MSB)) 
  {
    for(i=0; i<3; i++)
    {
      pnRawData[i]=((int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i]);
    }
  }
  else /* Big Endian Mode */
  {
    for(i=0; i<3; i++)
    {
      pnRawData[i]=((int16_t)((uint16_t)buffer[2*i] << 8) + buffer[2*i+1]);
    }
  }
  
  /* Normal mode */
  /* Switch the sensitivity value set in the CRTL4 */
  switch(ctrlx[0] & LSM303DLHC_FULLSCALE_16G)
  {
  case LSM303DLHC_FULLSCALE_2G:
    sensitivity = LSM303DLHC_ACC_SENSITIVITY_2G;
    break;
  case LSM303DLHC_FULLSCALE_4G:
    sensitivity = LSM303DLHC_ACC_SENSITIVITY_4G;
    break;
  case LSM303DLHC_FULLSCALE_8G:
    sensitivity = LSM303DLHC_ACC_SENSITIVITY_8G;
    break;
  case LSM303DLHC_FULLSCALE_16G:
    sensitivity = LSM303DLHC_ACC_SENSITIVITY_16G;
    break;
  }
  
  /* Obtain the mg value for the three axis */
  for(i=0; i<3; i++)
  {
    pData[i]=(pnRawData[i] *sensitivity);
  }
}

