#include "sys.h"
#include "usart.h"	
#include "ringbuf.h"
#include "led.h"
#include "delay.h"

void uart2_init(u32 bound){
   
GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	//PA2:USART2_TX  ; PA3:USART2_RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 

	USART_Init(USART2, &USART_InitStructure);

	//USART_ClearFlag(USART2, USART_FLAG_TC);
              
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); //使能接收中断   	
	USART_Cmd(USART2, ENABLE);
	
	   
      
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   //中断优先级分组 
      
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);  
}

void USART2_IRQHandler(void)  
{  
  uint8_t data;  
 // uint8_t err;  
	
   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //产生接收中断
    {    
			  data=USART_ReceiveData(USART2); 
        ringbuf_put(&ring_rx,data);   // 把接收到数据放入缓冲区中  
        USART2->DR=data;//回显		
    }  
   else if(USART2->SR &USART_FLAG_TXE)   //产生发送中断
    { 
				if(ringbuf_elements(&ring_tx)==0){                        //判断缓冲区是否为空
					  //USART_ITConfig(USART2, USART_IT_TXE| USART_IT_TC, DISABLE); //没有数据发送，关闭中断  
					 USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
					return;
				 }
				else{
				 USART2->DR=ringbuf_get(&ring_tx);
					USART_ITConfig(USART2,USART_IT_TXE,DISABLE);  //关中断 
					return ;
         }  
     }     		                                           
 }


void uart2_putc(char c)
{

	while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET )
	{
		;
	}
	USART2->DR = c;
}

void uart2_puts(const char *str)
{
    if(str == 0)
    {
        return;
    }
    while(*str)
    {
        uart2_putc(*str);
        if(*str == '\n')
        {
            uart2_putc('\r');
        }
        str++;
    }
}





