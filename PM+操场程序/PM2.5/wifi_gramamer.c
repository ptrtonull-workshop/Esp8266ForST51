#include<reg52.h>
#include<head_file.h>
#include"string.h"
#include <ctype.h>

#define	RX1_Lenth	32			//串口接收缓冲长度
uchar	RX1_Cnt=0;	                //接收计数
bit flag=0,flag2=0;
extern uchar  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;//传感器采集数据的保存地方都是八位数据
void Delay2(unsigned long cnt)
{
	long i;
 	for(i=0;i<cnt*100;i++);
}
void Tranfer(uchar *s)
{
   while(*s!='\0')
   {
     SBUF= toascii(*s);
	 while(!TI);
	 TI=0;
     s++;
	 Delay2(1);
   }
}						 

uchar idata RX1_Buffer[RX1_Lenth]=0;	//接收缓冲


/* 配置wifi */
void set_wifi(void)
{
    Delay2(1000);
	Tranfer("AT+CWMODE=1\r\n");
	Delay2(1000);	
    Tranfer("AT+RST\r\n");	
	Delay2(1000);
	Tranfer("AT+CIPMUX=0\r\n");	 //单链接
	Delay2(1000);	
    Tranfer("AT+CWJAP=\"ESP8266\",\"0123456789\"\r\n");
	Delay2(3000);
	Tranfer("AT+CIFSR\r\n");	
	Delay2(500);
	Tranfer("AT+CIPSTART=\"TCP\",\"192.168.4.1\",5000\r\n");
	Delay2(2000);
	Tranfer("AT+CIPMODE=1\r\n");   //透传模式
	Delay2(1000);
	Tranfer("AT+CIPSEND\r\n");
	Delay2(1000);

}

void SendData()
{
 uchar a,b,c,d,e,f;
  if(U8checkdata)
	  {
	   U8T_data_H%=100;
	   a=U8T_data_H/10;
	   b=U8T_data_H%10;
	   c=U8T_data_L%10;
	   U8RH_data_H%=100;
	   d=U8RH_data_H/10;
	   e=U8RH_data_H%10;
	   f=U8RH_data_L%10;
	   Tranfer("R4");
	   SBUF=a;   Delay2(1);
	   SBUF=b;	 Delay2(1);
	   SBUF='.'; Delay2(1);
	   SBUF=c;	 Delay2(1);
	   SBUF=d;	 Delay2(1);
	   SBUF=e;	 Delay2(1);
	   SBUF='.'; Delay2(1);
	   SBUF=f;   Delay2(20);
	  }
}
void main()
{
  
    SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式1
	TH1=0XFD;		    //计数器初始值设置，注意波特率是9600
	TL1=0XFD;	
	TR1=1;						    //打开计数器
	set_wifi();
	while(1)
	{
	  /*一直在采集信息，采集到信息 发送数据*/
	   RH();
	  SendData();  	 
	}

}



