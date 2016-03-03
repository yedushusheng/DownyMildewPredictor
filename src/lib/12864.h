#ifndef __12864_H
#define __12864_H

#include <msp430x14x.h>
/*---------------------------------------------------
引脚定义区:
P4^0:片选信号CS(预留);
P4^1:串并口方式选择PSB（1并口，0串口）;
P4^2:串行时钟SCLK;
P4^3:串行数据SID;
----------------------------------------------------*/
//#define CS1 P4OUT |= BIT0
//#define CS0 P4OUT &= ~BIT0
#define SID1 P4OUT |= BIT1
#define SID0 P4OUT &= ~BIT1
#define SCLK1 P4OUT |= BIT2
#define SCLK0 P4OUT &= ~BIT2
//#define PSB0 P4OUT &= ~BIT3
//#define PSB1 P4OUT |= BIT3

/******************************* 函数声明区 *******************************/
void sendbyte(unsigned char Sbyte);
void writecmd(unsigned char dicate);
void writeaddress(unsigned char x,unsigned char y);
void writedata(unsigned char dattemp);
void print(unsigned char *str);
void Init_LCD();

#endif /*__12864_H*/