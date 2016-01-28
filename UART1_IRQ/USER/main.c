#include "usart.h"
#include"my_i2c.h"
#include"sys.h"
#include"led.h"
#include "delay.h"
#include"ringbuf.h"
#include"stdio.h"
#include"string.h"
#include"lsm303dlhc.h"

#define BUF_LEN (100)
uint8_t buf[BUF_LEN];
void uart2_SendDatas(char* pchar);
struct ringbuf ring_rx;//���ջ�����
struct ringbuf ring_tx;//���ͻ�����
char temp1[100][100];
char temp[100]={0};
char buf_mag[10]={0};
char buf_acc[10]={0};
char ring_puts[]="abc"; //�����ַ�����������
char *loc_temp0[2]={"sensor","set_charger"}; 
char *loc_temp1[10]={"SOC","CC","CV","START","STOP"};
int main(void)
{  
	  int cnt=0;//��־λ
	  int rev_done = 0;//������ɱ�־λ
	  int cur_state=0;//״̬λ��־
	  int i,j,k;
	  int16_t pdata_mag[3]={0,0,0};
	  int16_t pdata_acc[3]={0,0,0};
	  int pos1,pos2,pos3;
	  int print_flag=0;//��ӡ��־λ
	  int CMD_Code=0;//һ��������
	  int CMD_Code_Sec=0;//���������
	  int a=0;
		char* pchar;
	  i=j=k=0;
	  pos1=pos2=pos3=0;
	  ringbuf_init(&ring_rx,buf,BUF_LEN);
	  ringbuf_init(&ring_tx,buf,BUF_LEN);
	  uart2_init(9600); 
	  i2c_init();
		LSM303DLHC_MagInit();
		LSM303DLHC_AccInit();
	  USART_ITConfig(USART2,USART_IT_TXE,DISABLE);  //���ж� 
		
	 
 /*  LSM303DLHC_AccReadXYZ(pdata_acc);//ȡ�����ٶ�ֵ
	 Demo_CompassReadMag(pdata_mag);//ȡ������ֵ
	 uart2_puts("MAG X,Y,Z value:");
	 uart2_putc(10);
	 uart2_putc(13);
	 for(a=0;a<3;a++){
		  sprintf(buf_mag,"%d",pdata_mag[a]);
		  uart2_puts(buf_mag);
		  uart2_putc(10);
	    uart2_putc(13);
	 }
	 uart2_putc(10);
	 uart2_putc(13);
	 uart2_puts("ACC X,Y,Z value:");
	 uart2_putc(10);
	 uart2_putc(13);
	 for(a=0;a<3;a++){
		  sprintf(buf_acc,"%d",pdata_acc[a]);
		  uart2_puts(buf_acc);
		  uart2_putc(10);
		  uart2_putc(13);
	 }
	 */
	 //�����жϣ��û������
 
	uart2_SendDatas(ring_puts);
  
	while(1){	
	   
		//�����ַ���
		// 1.��ѯ�������Ƿ����ַ����յ����оͶ����� 
		if(ringbuf_elements(&ring_rx)>0){
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
			 switch(cur_state){
				 case 0:                     //��ʼ��״̬
					  if(temp[k]==32){
							cur_state=0;
							k++;
							break;
						}
						else{
							cur_state=1;         //�л������ַ���״̬
							break;
						}
			  case 1:
					 if(temp[k]!=32){
						 temp1[i][j++]=temp[k];
						 cur_state=1;          //�����մ�״̬
						 k++;
						 break;
					 }
					 else{
						 cur_state=2;          //�л����ո�״̬
						 break;
					 }
				case 2:
					if(temp[k]==32){
						cur_state=2;
						k++;
						break;
					}
					else{
						temp1[i][j]='\0';
						i++;
						j=0;
						cur_state=1;         //�л����մ�״̬
						break;
					}
			 }	
		 }
				  temp1[i][j]='\0';
				  print_flag=1;
			    rev_done =0; 	
		}	
		//uart2_puts(temp1[1]);
    if(print_flag==1){
			for(pos2=0;pos2<=i;pos2++){
				uart2_puts(temp1[pos2]);
				uart2_putc(10);
				uart2_putc(13);
			}
		
			//ƥ������
		  i=j=k=0;
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
							 uart2_putc(32);
					      switch( CMD_Code_Sec)
								{
									case 0:
										   if(strcmp(loc_temp1[0],temp1[i])==0){
									     uart2_puts( loc_temp1[0]);
									     uart2_puts(" is called ");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
									     break;
									case 1 :
										   if(strcmp(loc_temp1[1],temp1[i])==0){
												 uart2_puts( loc_temp1[1]);
									       uart2_puts(" is called ");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
									     break;
									case 2:
										   if(strcmp(loc_temp1[2],temp1[i])==0){
												 uart2_puts( loc_temp1[2]);
									       uart2_puts(" is called ");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
										    break;
									case 3:
										   if(strcmp(loc_temp1[3],temp1[i])==0){
												 uart2_puts( loc_temp1[3]);
									       uart2_puts(" is called ");
											 }
											 else{
												 uart2_puts("This command is not called");
											 }
										    break;
									case 4 :
										   if(strcmp(loc_temp1[4],temp1[i])==0){
												 uart2_puts( loc_temp1[4]);
									       uart2_puts(" is called ");
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



	 
	 

		      
	 
 
	 
