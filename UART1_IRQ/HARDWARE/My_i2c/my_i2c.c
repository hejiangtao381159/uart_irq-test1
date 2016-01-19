#include"my_i2c.h"
#include "sys.h"
#include"lsm303dlhc.h"
#include "delay.h"

void i2c_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOB,ENABLE);//使能GPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,  ENABLE);//使能I2C时钟

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);//映射引脚
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;    //设置I2C为I2C模式      
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//设置I2C的占空比为快速模式，
  I2C_InitStructure.I2C_OwnAddress1 =ACC_I2C_ADDRESS;      //设置第一个设备的自身地址
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;    //使能应答
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;   //应答地址为7位格式
  I2C_InitStructure.I2C_ClockSpeed = 100000;  	//100K速度
  I2C_Cmd(I2C1, ENABLE);//使能I2C1
  I2C_Init(I2C1, &I2C_InitStructure); 
  I2C_AcknowledgeConfig(I2C1, ENABLE);//允许一字节1应答模式
}
 

 void i2c_read_bytes(uint16_t DeviceAddr,uint8_t RegAddr,uint8_t* pBuffer,u16 len)
 {	
	 while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	 
	 I2C_AcknowledgeConfig(I2C1, ENABLE);//允许一字节1应答模式
	 
	 I2C_GenerateSTART(I2C1,ENABLE);//发送起始信号
	 
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//等待释放I2C总线
	 
	 I2C_Send7bitAddress(I2C1,DeviceAddr,I2C_Direction_Transmitter);//从器件加速度传感器的地址+写
	 
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//从器件应答
	 
   I2C_SendData(I2C1, RegAddr);  //发送从器件存储器地址
	 
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//发送完成
	 
	 I2C_GenerateSTART(I2C1, ENABLE);//起始位
	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//等待释放I2C总线
	 
   I2C_Send7bitAddress(I2C1,DeviceAddr, I2C_Direction_Receiver); //发送读操作
	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//从器件应答
	 
   I2C_AcknowledgeConfig(I2C1, DISABLE); 
	 
   I2C_GenerateSTOP(I2C1, ENABLE); 
	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); 
	 
   *pBuffer = I2C_ReceiveData(I2C1);
	 
   I2C_AcknowledgeConfig(I2C1, ENABLE);
}
 
void i2c_wirte_byte(uint16_t DeviceAddr,uint8_t RegAddr,uint8_t REG_data){
	
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	 
  I2C_AcknowledgeConfig(I2C1, ENABLE);//允许一字节1应答模式
	
	I2C_GenerateSTART(I2C1,ENABLE);//起始信号

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));//等待释放I2C总线

  I2C_Send7bitAddress(I2C1,DeviceAddr,I2C_Direction_Transmitter);//从器件加速度传感器的地址+写

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));////从器件应答

  I2C_SendData(I2C1,RegAddr);//发送从器件存储器地址

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//发送完成

  I2C_SendData(I2C1,REG_data);//写入数据

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//发送完成

  I2C_GenerateSTOP(I2C1,ENABLE);

}





