#ifndef __12864_H
#define __12864_H

#include <msp430x14x.h>
/*---------------------------------------------------
���Ŷ�����:
P4^0:Ƭѡ�ź�CS(Ԥ��);
P4^1:�����ڷ�ʽѡ��PSB��1���ڣ�0���ڣ�;
P4^2:����ʱ��SCLK;
P4^3:��������SID;
----------------------------------------------------*/
//#define CS1 P4OUT |= BIT0
//#define CS0 P4OUT &= ~BIT0
#define SID1 P4OUT |= BIT1
#define SID0 P4OUT &= ~BIT1
#define SCLK1 P4OUT |= BIT2
#define SCLK0 P4OUT &= ~BIT2
//#define PSB0 P4OUT &= ~BIT3
//#define PSB1 P4OUT |= BIT3

/******************************* ���������� *******************************/
void sendbyte(unsigned char Sbyte);
void writecmd(unsigned char dicate);
void writeaddress(unsigned char x,unsigned char y);
void writedata(unsigned char dattemp);
void print(unsigned char *str);
void Init_LCD();

#endif /*__12864_H*/