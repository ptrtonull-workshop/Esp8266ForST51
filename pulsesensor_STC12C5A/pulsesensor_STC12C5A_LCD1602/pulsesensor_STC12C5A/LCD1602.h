#ifndef LCD1602_H
#define LCD1602_H

void delay(unsigned int uiCount);	//��ʱ����
void LCD1602_CheckBusy(void);	//Һ��æ���
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//��Һ����д���ݻ���д���0Ϊ���1Ϊ����
void LCD1602_Init(void);	//Һ����ʼ��
void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//Һ���������ƶ���ָ��λ��
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//��Һ��ָ��λ����ʾ�ַ�
void LCD1602_DisplayString(unsigned char *ucStr);	//��Һ������ʾ�ַ���
#endif  