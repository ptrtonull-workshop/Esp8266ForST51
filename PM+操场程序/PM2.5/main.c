
//ͷ�ļ�
#include "STC12c5A.h"
#include "1602.h"
#include "2402.h"

#define uchar unsigned char
#define uint  unsigned int


//���尴��

sbit Key_Up_   = P3^4;
sbit Key_Down  = P3^5;
//�������
sbit fs = P2^0;


//���������LED����

sbit DUST_Warn = P3^3;
sbit  LED1 = P2^3;
sbit  LED2 = P2^4;
//�����ʶ
uchar FlagStart = 0;  
sbit  LED = P3^2;


//ȫ�ֱ������壬
const char ADC =   0x00;	    //P1^0
	           
uint Counter;
uint DUST_SET;	            //�����������ֵ
float   DUST_Value; 

uchar ADC_Get[10]={0}; 	  //����AD��������
uchar num=0;

//��������

void Data_Init();
void Timer0_Init();
void Port_Init();
void ADC_Init();
uchar GetADVal(uchar);
void KeyProcess(void);



//���ݳ�ʼ��
void Data_Init()
{
   Counter = 0;
   FlagStart=0;
   DUST_Value=0;
   DUST_Warn=1;
   LED=0;
   fs=1;
}

//DS---185ҳ  ��ʱ����ʼ�� ��ʱ10ms
void Timer0_Init()
{
	
	//AUXR=0x80; //Time work in 1T mode      
	TMOD = 1;      
	TL0 = (65536-30000)/256;     
	TH0 = (65536-30000)%256;    
	TR0 = 1;
	ET0 = 1;   
	EA = 1;     
}

//��ʱ��0�ж�
void Timer0_ISR (void) interrupt 1 using 0
{
  uint i,j;	
	TL0 = (65536-30000)/256;     
	TH0 = (65536-30000)%256;   
    	LED=1;					 //�����������ģ̣ţ�
	
     for (j=0;j<222;j++);  //0.28ms  //��ʱ0.28ms
	
	ADC_Get[num]=GetADVal(ADC);	  //����ADC�ɼ�
	num++;
	if(num>9)
	{
	  FlagStart=1;
	  num=0;
	  TR0 = 0;   //�ȹرն�ʱ��0
	  EA = 0; 
	}						   //�ɼ�10�Σ��رն�ʱ��0���������ݴ���
//	for (j=0;j<25;j++);
							   //�رմ�����LED
	LED=0;

}

//�˿ڹ�������
void Port_Init()
{
   P1M0 = 0x03;  //DS--88ҳIO�ڹ������ã���Ϊ��©11 
   P1M1 = 0x03;  //   
}

void ADC_Init()  //DS-282ҳ
{
   
   P1ASF = 0x03;   //����ͨ��0,1 
   ADC_RES = 0;              
   ADC_CONTR=ADC_POWER|ADC_SPEEDLL; //ÿ��ת����Ҫ420��ʱ������	         
   delay_ms(10);

}


//�����趨ֵ  ���趨ֵ�洢��24C02��
void Save_Setting(void)
{
   uchar Save;
   Save =DUST_SET/256;
   IIC_Write(0x00, Save);
   Save =DUST_SET%256;
   delay_ms(10);
   IIC_Write(0x01, Save);	     
   
}

//�����趨ֵ  ��24C02�ж�ȡ�趨ֵ
void Load_Setting()
{
   
   DUST_SET=0;
   DUST_SET = IIC_Read(0x00);
   DUST_SET*=256;
   delay_ms(10);
   DUST_SET+= IIC_Read(0x01);
   if ((DUST_SET>=760)||(DUST_SET<=0)) DUST_SET =100;
}

//�����������
void KeyProcess(void)
{
   uchar Temp;
   uint i,j;
   Temp=P3;
   Temp&=0xf0;
   if(Temp!=0xf0)
    {
    	 
	 //��ʱ
	 //for (i=0;i<500;i++)
      //for (j=0;j<254;j++);
	 Temp=P3;
      Temp&=0xf0;
    	 if(Temp!=0xf0)
	 {
	    switch(Temp)
	   {
	   case 0xE0:  DUST_SET++;   break;		  //	 �趨ֵ+
	   case 0XD0:  DUST_SET--;   break;		  //	 �趨ֵ-
	   default: break;
	
	   }
	   //ֵ�޶�
	  
	   
	    if (DUST_SET>=760)
	        DUST_SET =760;
	    if (DUST_SET<=1)
	        DUST_SET =0;
	   Save_Setting();

	 }
	 
   }
}
void main()
{
   uchar read; 
   uint DUST;
   EA = 0;          //���жϹر�	     
   Data_Init();     //���ݳ�ʼ��
   Port_Init();     //�˿ڳ�ʼ��
   IIC_Init();

   LCD_Init(); 	//Һ����ʼ��
   LCD_Clear();//���� 
   LCD_Write_String(4,0,"Welcome");
   LCD_Write_String(0,1,"PM2.5  Detector");
   delay_ms(2000);
  
   LCD_Clear();//����
   delay_ms(10); 
   LCD_Write_String(0,0,"PM2.5:     mg/m3");
   LCD_Write_String(0,1,"Alarm:     mg/m3");
   
   //�����趨ֵ
   Load_Setting();
   
   ADC_Init();        //ADC��ʼ��
   delay_ms(10);
   Timer0_Init();   //��ʱ��0��ʼ��
   num=0;	
   DUST=0;
   FlagStart=0;
   
   
   while(1)
   {
   	 if(FlagStart==1)       //10�βɼ����
	 {
	  //////////////////////////////
	  DUST=Error_Correct(ADC_Get,10);		//��ȡ10��AD������ֵ
	  DUST_Value=(DUST/256.0)*5000;		//ת���ɵ�ѹֵMV
	  DUST_Value=DUST_Value*0.172-99.9; //DUST_Value=((DUST)*38)/4-202;//	//������������Ũ�ȼ��� Y=0.172*X-0.0999      X--������ѹV
	  if(DUST_Value<0)	      DUST_Value=0;
	  if(DUST_Value>760)	 DUST_Value=760;	//��λ
	  DUST=(uint)DUST_Value;					 //
	  
	  LCD_Show4(6,0,DUST);
	  LCD_Show4(6,1,DUST_SET);			     //��ʾ���

	  if(DUST>DUST_SET)
	  { 
	  	DUST_Warn=0;
		fs=0;
		LED1=1;
		LED2=0;  
		}      //������ֵ������
	  else
	 { DUST_Warn=1;
	 	fs=1;
	  	LED1=0;
	  	LED2=1;
		}

	  
	  KeyProcess();
	  delay_ms(100);
	  TL0 = (65536-30000)/256;     
	  TH0 = (65536-30000)%256; 
	  TR0 = 1;   //������ʱ��0
	  EA = 1;
	  FlagStart=0;
	 }
	 
	 
   }
   
}