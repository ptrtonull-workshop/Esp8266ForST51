#ifndef _head_file_h_
#define _head_file_h_
/***************************
WIFI�ͽӵ�Ƭ������

��ʪ�����ݿڽ�P1.0
****************************/
#define uchar unsigned char
#define uint unsigned int

/*WIFI��������*/
void Delay2(unsigned long cnt);
void Tranfer(uchar *s);
void set_wifi(void);




/*12864��������*/
void print(unsigned char *s);
void write(bit start, unsigned char ddata);
void sendbyte(unsigned char bbyte);
void delaynms(unsigned int di);
void lcdinit(void);
sbit CS  =  P2^0;	//Ƭѡ �ߵ�ƽ��Ч ��ƬLCDʹ��ʱ�ɹ̶��ߵ�ƽ	 rs
sbit SID  = P2^1;	//����	rw
sbit SCLK = P2^2;	//ʱ��	 e
sbit RESET = P2^3;	//LCD��λ��LCDģ���Դ���λ��·���ɲ���
void Display(uchar,uchar,uchar,uchar,uchar,uchar);
void SendData(uchar,uchar,uchar,uchar,uchar,uchar);
/*DHT��������*/
void  Delay(uint j);
void  Delay_10us(void);
void  COM(void);
void  RH(void);
sbit  P2_0  = P1^1 ;//���¡�ʪ�ȴ����� �ĵ�������
static bit RHT_flag;
#endif



