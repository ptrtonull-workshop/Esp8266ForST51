
//ͷ�ļ�
#include "STC12c5A.h"
#include<head_file.h>

#define FOSC 11059200L		//ϵͳʱ��



bit  FlagStart,DUST_Warn;



//ȫ�ֱ������壬
const char ADC =   0x00;	    //P1^0
	           
uint Counter;
uint DUST_SET;	            //�����������ֵ
float   DUST_Value; 

uchar ADC_Get[10]={0}; 	  //����AD��������
uchar num=0;

//��������
void delay_us(uint t);
void delay_ms(uint t);
void Data_Init();
void Timer0_Init();
void Port_Init();
void ADC_Init();
uchar GetADVal(uchar);

void UART_init(void); 


void delay_us(uint t)
{   
 	  uint T;
	  T=t;
	  while(T!=0)
	  T--;
}

void delay_ms(uint t)
{
     
	uint T;
	T=t;
	while(T--)
	{
		
	     
		delay_us(245);
		delay_us(245);
	     delay_us(245);
		delay_us(245);
	}
}


void UART_init(void)
{
   PCON &= 0x7f;  //�����ʲ�����
   SCON = 0x50;  //8λ���ݣ��ɱ䲨����
   BRT = 0xFD;    //���������ʲ�������ֵ
   AUXR |= 0x04;  //ʱ������Ϊ1Tģʽ
   AUXR |= 0x01;  //ѡ����������ʲ�����
   AUXR |= 0x10;  //���������ʲ���
}



//����ADת�����õ���ǰ8λADֵ
uchar GetADVal(uchar CH)
{
   
   ADC_CONTR=ADC_POWER|ADC_SPEEDLL|CH|ADC_START;
   //delay_ms(1);        
   while(!(ADC_CONTR&ADC_FLAG)); //�ȴ�ADCת������
   ADC_CONTR &= ~ADC_FLAG;       //�ر�ADC     
   return ADC_RES;
}


//���ݳ�ʼ��
void Data_Init()
{
   Counter = 0;
   FlagStart=0;
   DUST_Value=0;
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
	EA  = 1;     
}

//��ʱ��0�ж�
void Timer0_ISR (void) interrupt 1 using 0
{
    uint j;	
	TL0 = (65536-30000)/256;     
	TH0 = (65536-30000)%256;   
  
	
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


}

//�˿ڹ�������
void Port_Init()
{
   P1M0 = 0x03;  //DS--88ҳIO�ڹ������ã���Ϊ��©11 
   P1M1 = 0x03;  //   
}

void ADC_Init()  //DS-282ҳ
{
   
   P1ASF = 0x01;   //����ͨ��0,1 
   ADC_RES = 0;              
   ADC_CONTR=ADC_POWER|ADC_SPEEDLL; //ÿ��ת����Ҫ420��ʱ������	         
   delay_ms(10);

}

//��ֵ�˲�
//�㷨���Ƚ�������Ȼ��������м�ֵ��Ϊ��ǰֵ���ء�
uchar Error_Correct(uchar *str,uchar num) 
{
   unsigned char i=0;
   unsigned char j=0;
   uchar Temp=0;
   
   //����
   for(i=0;i<num-1;i++)
     {
	  for(j=i+1;j<num;j++)
	   {
	     if(str[i]<str[j])
	   	{
		   Temp=str[i];
		   str[i]=str[j];
		   str[j]=Temp;
		
		}
	   
	   }
	}
    //ȥ����ȡ�м�ֵ
    return str[num/2];

}
//�����趨ֵ  ���趨ֵ�洢��24C02��



void main()
{
   uint DUST=0;
   EA = 0;          //���жϹر�
   	     
   Data_Init();     //���ݳ�ʼ��
   set_wifi();
   Port_Init();     //�˿ڳ�ʼ��
   UART_init();
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

	  delay_ms(100);
	  FlagStart=0;

     }
	    RH();
	  SendData(DUST);    
	 
  }
   
}