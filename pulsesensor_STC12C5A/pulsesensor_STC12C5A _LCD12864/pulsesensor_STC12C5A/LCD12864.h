#ifndef LCD12864_H
#define LCD12864_H

void LCD12864_Init(void);		  //LCD12864��ʼ������
void LCD12864_WriteInfomation(unsigned char ucData,bit bComOrData);	   //��LCD12864д�����ݣ�bComOrDataΪ1ʱд��������ݣ�0ʱд���������
void LCD12864_CheckBusy(void);		//æ��⺯��
void LCD12864_DisplayOneLine(unsigned char ucPos,unsigned char *ucStr);	  //��LCD12864д��һ������
void LCD12864_set_pos(unsigned char X,unsigned char Y);
void LCD_disp_list_char(unsigned char X, unsigned char Y, char *DData);
void delay(unsigned int uiCount);
#endif  