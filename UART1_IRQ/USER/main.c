#include "usart.h"
#include"sys.h"
#include"led.h"
#include "delay.h"
#include"ringbuf.h"
#include"stdio.h"
#include"string.h"
#define BUF_LEN (100)
char buf[BUF_LEN];
struct ringbuf ring_rx;//���ջ�����
struct ringbuf ring_tx;//���ͻ�����
char temp1[100][100];
char temp[100]={0};
char *loc_temp0[2]={"sensor","set_charger"}; 
char *loc_temp1[10]={"SOC","CC","CV","START","STOP"};
int main(void)
{  
	  int cnt=0;//��־λ
	  int rev_done = 0;//������ɱ�־λ
	  int i,j,k;
	  int pos1,pos2,pos3;
	  int print_flag=0;//��ӡ��־λ
	  int CMD_Code=0;//һ��������
	  int CMD_Code_Sec=0;//���������
	  int a=1;
	  i=j=k=0;
	  pos1=pos2=pos3=0;
	  ringbuf_init(&ring_rx,buf,BUF_LEN);
	  ringbuf_init(&ring_tx,buf,BUF_LEN);
	  uart2_init(9600); 
	  
	   USART_ITConfig(USART2,USART_IT_TXE,DISABLE);  //���ж� 
		
	//ringbuf_put(&ring_tx,'a');
	//ringbuf_put(&ring_tx,'a');
 
	 
	while(1){	
		//���������ж�
		 //USART_ITConfig(USART2,USART_IT_TXE,ENABLE);  //ʹ�ܷ����ж�  
	   //cnt=0;
		//�����ַ���
		// 1.��ѯ�������Ƿ����ַ����յ����оͶ����� 
		 // 
		if(ringbuf_elements(&ring_rx)>0){
				//USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//�رս����ж�
				temp[cnt]=ringbuf_get(&ring_rx);
				if(temp[cnt]==13){
					rev_done = 1;
					uart2_putc(10);
				}
				cnt ++;
			}
	   
			// parse string
		if(rev_done==1){
		    while(k<cnt-1){
	         if(temp[k]==32 && temp[k+1]!=32){ 
						   temp1[i][j]='\0';
               i++;
			         j=0;
		        }
	         else{
              temp1[i][j++]=temp[k];
	          }
	          k++;  
	      }	
				  print_flag=1;
			    rev_done =0; 
				  i=j=k=0;
		 }	
		
   if(print_flag==1){
		   while(pos2<cnt-1){
		      if(temp1[pos1][pos3]=='\0'){
		 	        pos1++;
			        pos3=0;
			        uart2_putc(10);
					    uart2_putc(13);
		      }
		     else{
			        uart2_putc(temp1[pos1][pos3]);
			        pos3++;
		      }
		     pos2++;
	    }
		uart2_putc(10);
		uart2_putc(13);
    uart2_puts(temp1[1]);
		uart2_putc(10);
		uart2_putc(13);	
			//ƥ������
		
			while(j<2 && strcmp(loc_temp0[j],temp1[0])!=0){
				j++;
			}
			CMD_Code=j;
				switch(CMD_Code)
				 {
					case 0 :
						   if(strcmp("get",temp1[1])==0){
								 uart2_puts("sensor is called");
							 }
							 else{
								 //uart2_puts(temp1[1]);
								 uart2_puts("sensor is not called");
							 }
							 break;
					case 1 :
						   while(k<6 && strcmp(loc_temp1[k],temp1[1])!=0){
								 k++;
							 }
					       CMD_Code_Sec=k;
					       i++;
					     uart2_puts(loc_temp0[1]);
					      switch( CMD_Code_Sec)
								{
									case 1 :
										   if(strcmp(loc_temp1[1],temp1[i])==0){
									     uart2_puts( loc_temp1[1]);
									     uart2_puts(" is called");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
									     break;
									case 2 :
										   if(strcmp(loc_temp1[2],temp1[i])==0){
												 uart2_puts( loc_temp1[2]);
									       uart2_puts(" is called");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
									     break;
									case 3 :
										   if(strcmp(loc_temp1[3],temp1[i])==0){
												 uart2_puts( loc_temp1[3]);
									       uart2_puts(" is called");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
										    break;
									case 4 :
										   if(strcmp(loc_temp1[4],temp1[i])==0){
												 uart2_puts( loc_temp1[4]);
									       uart2_puts(" is called");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
										    break;
									case 5 :
										   if(strcmp(loc_temp1[5],temp1[i])==0){
												 uart2_puts( loc_temp1[5]);
									       uart2_puts(" is called");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
										   break;
								}
								break;
				 }
		   print_flag=0;
			 cnt=0; 
	   	 pos1=pos2=pos3=0; 
			 i=j=k=0; 
		}  
	}  
}	




	 
	 

		      
	 
 
	 
