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
void SendData(uint);



/*DHT��������*/
void  Delay(uint j);
void  Delay_10us(void);
void  COM(void);
void  RH(void);
sbit  P2_0  = P1^0 ;//���¡�ʪ�ȴ����� �ĵ�������
static bit RHT_flag;
#endif



