#include<reg52.h>
#include<head_file.h>
#include"math.h"
#include"string.h"
#include <ctype.h>

#define	RX1_Lenth	32			//���ڽ��ջ��峤��

uchar	RX1_Cnt=0;	                //���ռ���
bit flag=0;


sbit  yanwu=P1^1;
uchar   yanwu_f;
uchar Dis_f;
extern  uchar  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
uchar code senddata[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
uchar idata RX1_Buffer[RX1_Lenth];	//���ջ���
uchar idata RX_Buffer[RX1_Lenth];	//���ջ���
uchar idata PM[8];


uchar time=0;
void Delay2(unsigned long cnt)
{
	long i;
 	for(i=0;i<cnt*50;i++);
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


/* *******************����wifi******************/
void set_wifi(void)
{
    write(0,0x80);
	print("ϵͳ��ʼ��...");
    Delay2(500);
    Tranfer("AT+RST\r\n");
	write(0,0x80);
	print("ϵͳ��ʼ��....");	
	Delay2(1000);
	Tranfer("AT+CIPMUX=0\r\n");	 //������
	write(0,0x80);
	print("ϵͳ��ʼ��ʧ��");
	Delay2(500);
    Tranfer("AT+CIPSTART=\"TCP\",\"192.168.4.1\",5000\r\n");
	write(0,0x80);
	print("Ӧ�óɹ���~~~~~~"); 
	
	Delay2(500);
	write(0,0x80);
	print("����������....  ");Delay2(500);
	write(0,0x80);
	print("                ");
    
}
/********************ϵͳ��ʼ��**************************/
void SysInit()
{
   lcdinit();
    SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X21;			//���ü�����������ʽ1
	TH1=0XFD;		    //��������ʼֵ���ã�ע�Ⲩ������9600
	TL1=0XFD;
	TH0=0X3C;
	TL0=0XB0;	
	TR1=1;
	ET0=1;						    //�򿪼�����
	set_wifi();						//�򿪽����ж�
	EA=1;					//�����ж�
	ES=1;
	TR0=1; 
   U8checkdata=0;	
}

/******************************��ʾ�������յ����ݲ���ʾ�ٳ����򿪶�ʱ��**************************************************/
void Display(uchar a,uchar b,uchar c, uchar d,uchar e,uchar f)
{
 uchar i;
  if(flag)
	  {	
	   if(Dis_f==1)
		   {
			   write(0,0x80);
			   print("      �ٳ�      ");
			   write(0,0x90);
			   print("�¶�:");
			   write(1,RX1_Buffer[2]);
			   write(1,RX1_Buffer[3]);
			   write(1,RX1_Buffer[4]);
			   write(1,RX1_Buffer[5]);
			   write(0,0x88);
			   print("ʪ��:");
			   write(1,RX1_Buffer[6]);
			   write(1,RX1_Buffer[7]);
			   write(1,RX1_Buffer[8]);
			   write(1,RX1_Buffer[9]);
			   write(0,0x98);
			   print("PM2.5:");
			   for(i=0;i<5;i++)
			    write(1,PM[i]);
		   }
	  }
  if(Dis_f==2)
	  {
	   write(0,0x80);
	   print("      ������    ");
	   write(0,0x90);
	   print("�¶�:");
	   write(1,a+48);
	   write(1,b+48);
	   write(1,'.');
	   write(1,c+48);
	   write(0,0x88);
	   print("ʪ��:");
	   write(1,d+48);
	   write(1,e+48);
	   write(1,'.');
	   write(1,f+48);
	   write(0,0x98);
	   print("����:");
	   write(1,yanwu_f+48);
	  }	   	 
}
/***************���������յ����ݿ�ʼ��������ʾ��������ʪ��********************/
void Dealdata()
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
     Display(a,b,c,d,e,f);
	 SendData(a,b,c,d,e,f);
   }
}
void SendData(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f)
{
       ES=0;TR0=0;
	   Tranfer("AT+CIPSEND=10\r\n");
       Tranfer("R2");
	   SBUF=senddata[a];     Delay2(1);
	   SBUF=senddata[b];	 Delay2(1);
	   SBUF=0x2e;            Delay2(1);
	   SBUF=senddata[c];	 Delay2(1);
	   SBUF=senddata[d];	 Delay2(1);
	   SBUF=senddata[e];	 Delay2(1);
	   SBUF=0x2e;            Delay2(1);
	   SBUF=senddata[f];     Delay2(20);
	   Tranfer("AT+CIPSEND=10\r\n");
	   Tranfer("R2YWCGQ");
	   SBUF=senddata[yanwu_f];  Delay2(1);
	   SBUF=senddata[0];	 Delay2(1);
	   SBUF=senddata[0];	 Delay2(20);
	   TR0=1;
	   ES=1;
}

/*********************��ʱ������****************************/
void time0() interrupt 1
{
    TH0=0X3C;
	TL0=0XB0;
    time++;
	if(time==40)
	{
		 Dis_f++;
	  if(Dis_f>2)
	  {
	  	 Dis_f=1;
	  }
	 
	}
}

/*********************�жϺ���****************************/
void Usart() interrupt 4
{
 uchar i=0,m=0;
   TR0=0; 
	if(RI)
	{
	   RI=0;
	   RX_Buffer[RX1_Cnt]=SBUF;	//���ջ���   ������������ʾ
	   if(RX_Buffer[0]==0X52)	// R
	   {
			RX1_Cnt++;
	   }
		else
		{
		    
			RX1_Cnt=0;
		}
		if(RX1_Cnt>=12)
		{
		  if(RX_Buffer[1]==0x32)
		  {
		   if(RX_Buffer[2]==0X50&&RX_Buffer[3]==0X4D&&RX_Buffer[4]==0X41)//����PMAУ����	 PM2.5
				{
				  for(m=5;m<10;m++)
						{
							PM[i]= RX_Buffer[m];
							i++;
						}
				} 
			 else
			 {
			   for(m=0;m<10;+m++)
			   {
			   	 RX1_Buffer[m]=RX_Buffer[m];
				 flag=1;
			   }
			 }
		     RX1_Cnt=0;
		  }
		  else
		   RX1_Cnt=0; 	 	   
		}   
	}
	TR0=1;
}

/************************������****************************/
void main()
{
   SysInit(); 
	while(1)
	{
	  /*һֱ�ڲɼ���Ϣ���ɼ�����Ϣ ��������*/
	   RH();
	   Dealdata();
	   if(yanwu)
	   {
	     yanwu_f=1;
		}
	   else
	   {
	     yanwu_f=0;
	   }	   
	}

}
