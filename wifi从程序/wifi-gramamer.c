/***************************

�Ȱ�wifiģ������flash��  AT+CWMODE=1   AT+CWJAP="ESP8266","123456789" 

�޸Ĳ�����9600

***************************/
#include<reg52.h>
#include"string.h"
#include <ctype.h>
#define uchar unsigned char
#define uint unsigned int
#define	RX1_Lenth	16			//���ڽ��ջ��峤��
uchar	RX1_Cnt=0;	                //���ռ���
bit flag=0;
void Tranfer(uchar *s)
{
	 ES=0; 
   while(*s!='\0')
   {
     SBUF= toascii(*s);
		 while(!TI);
	   TI=0;
     s++;
   }
   ES=1;
}						 
void Tranfer_f(uchar *s,uchar Lenth)
{
	 ES=0; 
   while(Lenth--!=0)
   {
     SBUF= toascii(*s++);
		 while(!TI);
	   TI=0;
   }
   ES=1;
}
uchar idata RX1_Buffer[RX1_Lenth]=0;	//���ջ���

void Delay2(unsigned long cnt)
{
	long i;
 	for(i=0;i<cnt*10;i++);
}
/* ����wifi */
void set_wifi()
{
	Delay2(500);
  Tranfer("AT+RST\r\n");	
	Delay2(1000);
	Tranfer("AT+CIPMUX=0\r\n");	 //������
	Delay2(500);
  Tranfer("AT+CIPSTART=\"TCP\",\"192.168.4.1\",5000\r\n");
	Delay2(1000);
  Tranfer("AT+CIPMODE=1\r\n");	 //͸��ģʽ
	Delay2(500);
	Tranfer("AT+CIPSEND=10\r\n");
	Delay2(500);
}
void main()
{
  SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ1
	TH1=0XFD;		    //��������ʼֵ���ã�ע�Ⲩ������9600
	TL1=0XFD;	
	TR1=1;						    //�򿪼�����
	set_wifi();
	ES=1;						//�򿪽����ж�
	EA=1;					//�����ж�
	while(1)
	{
	  /*һֱ�ڲɼ���Ϣ���ɼ�����Ϣ ��������*/
	  /*  ʵ���õ���ʾ   */
		Delay2(500);
	  Tranfer("AT+CIPSEND=11\r\n");
		Delay2(200);
	  Tranfer("R432.240.1E");   //	 ����������ʮһ���ֽڵ�����
		Delay2(500);
	  if(flag==1)
    {  
	    Tranfer_f(RX1_Buffer,11);	//	 ����ʮһ���ֽ����ݸ���λ��	 �ڶ�λID
		  flag=0;
	  }
	}
}
void Usart() interrupt 4
{   
	if(RI)
	{
	   RI=0;
	   RX1_Buffer[RX1_Cnt]=SBUF;	//���ջ���   ������������ʾ
	   if(RX1_Buffer[0]=='T')	// T
	   {
				RX1_Cnt++;
	   }
		 else
		 {  
				RX1_Cnt=0;
		 }
		 if(RX1_Cnt>=10)
		 {
				RX1_Cnt=0;
				flag=1;	   
		}   
	}
}