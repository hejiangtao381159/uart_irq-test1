#include"my_i2c.h"
#include "sys.h"
#include"lsm303dlhc.h"
#include "delay.h"

void i2c_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,  ENABLE);//ʹ��I2Cʱ��

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);//ӳ������
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;    //����I2CΪI2Cģʽ      
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//����I2C��ռ�ձ�Ϊ����ģʽ��
  I2C_InitStructure.I2C_OwnAddress1 =ACC_I2C_ADDRESS;      //���õ�һ���豸�������ַ
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;    //ʹ��Ӧ��
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;   //Ӧ���ַΪ7λ��ʽ
  I2C_InitStructure.I2C_ClockSpeed = 100000;  	//100K�ٶ�
  I2C_Cmd(I2C1, ENABLE);//ʹ��I2C1
  I2C_Init(I2C1, &I2C_InitStructure); 
  I2C_AcknowledgeConfig(I2C1, ENABLE);//����һ�ֽ�1Ӧ��ģʽ
}
 

 void i2c_read_bytes(uint16_t DeviceAddr,uint8_t RegAddr,uint8_t* pBuffer,u16 len)
 {	
	 while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	 
	 I2C_AcknowledgeConfig(I2C1, ENABLE);//����һ�ֽ�1Ӧ��ģʽ
	 
	 I2C_GenerateSTART(I2C1,ENABLE);//������ʼ�ź�
	 
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//�ȴ��ͷ�I2C����
	 
	 I2C_Send7bitAddress(I2C1,DeviceAddr,I2C_Direction_Transmitter);//���������ٶȴ������ĵ�ַ+д
	 
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//������Ӧ��
	 
   I2C_SendData(I2C1, RegAddr);  //���ʹ������洢����ַ
	 
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//�������
	 
	 I2C_GenerateSTART(I2C1, ENABLE);//��ʼλ
	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//�ȴ��ͷ�I2C����
	 
   I2C_Send7bitAddress(I2C1,DeviceAddr, I2C_Direction_Receiver); //���Ͷ�����
	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//������Ӧ��
	 
   I2C_AcknowledgeConfig(I2C1, DISABLE); 
	 
   I2C_GenerateSTOP(I2C1, ENABLE); 
	 
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); 
	 
   *pBuffer = I2C_ReceiveData(I2C1);
	 
   I2C_AcknowledgeConfig(I2C1, ENABLE);
}
 
void i2c_wirte_byte(uint16_t DeviceAddr,uint8_t RegAddr,uint8_t REG_data){
	
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	 
  I2C_AcknowledgeConfig(I2C1, ENABLE);//����һ�ֽ�1Ӧ��ģʽ
	
	I2C_GenerateSTART(I2C1,ENABLE);//��ʼ�ź�

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));//�ȴ��ͷ�I2C����

  I2C_Send7bitAddress(I2C1,DeviceAddr,I2C_Direction_Transmitter);//���������ٶȴ������ĵ�ַ+д

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));////������Ӧ��

  I2C_SendData(I2C1,RegAddr);//���ʹ������洢����ַ

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//�������

  I2C_SendData(I2C1,REG_data);//д������

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));//�������

  I2C_GenerateSTOP(I2C1,ENABLE);

}





