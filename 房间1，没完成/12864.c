#include<reg52.h>
#include<head_file.h>



void print(unsigned char *s)
{
  
   while(*s!='\0')
   {
      write(1,*s);
	  s++;
   }
  

}

void lcdinit(void)	//��ʼ��LCD 
{
  delaynms(10); //�����ȴ�����LCM���빤��״̬
  RESET=0; delaynms(1); RESET=1; // ��λLCD
  CS=1;
  write(0,0x30);  //8 λ���棬����ָ�
  write(0,0x0c);  //��ʾ�򿪣����أ����׹�
  write(0,0x01);  //��������DDRAM�ĵ�ַ����������  
}

void write(bit start, unsigned char ddata) //дָ�������
{
  unsigned char start_data,Hdata,Ldata;
  if(start==0) start_data=0xf8;	 //дָ��
    else       start_data=0xfa;  //д����
  
  Hdata=ddata&0xf0;		  //ȡ����λ
  Ldata=(ddata<<4)&0xf0;  //ȡ����λ
  sendbyte(start_data);	  //������ʼ�ź�
  delaynms(5); //��ʱ�Ǳ����
  sendbyte(Hdata);	      //���͸���λ
  delaynms(1);  //��ʱ�Ǳ����
  sendbyte(Ldata);		  //���͵���λ
  delaynms(1);  //��ʱ�Ǳ����
}

void sendbyte(unsigned char bbyte) //����һ���ֽ�
{
 unsigned char i;
 for(i=0;i<8;i++)
   {
   SID=bbyte&0x80; //ȡ�����λ
   SCLK=1;
   SCLK=0;
   bbyte<<=1; //����
   }  
}

void delaynms(unsigned int di) //��ʱ
{
 unsigned int da,db;
 for(da=0;da<di;da++)
   for(db=0;db<10;db++);
}



 