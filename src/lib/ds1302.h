#ifndef __DS1302_H
#define __DS1302_H

#include "msp430x14x.h"

#define uchar unsigned char
#define uint unsigned int
/************** 端口定义 ***************/
/**************宏定义***************/
#define DS_RST          BIT5        //DS_RST = P5.5    PIN5
#define DS_SDA          BIT6        //DS_SDA = P5.6    PIN6
#define DS_SCL          BIT7        //DS_SCL = P5.7    PIN7

#define DS_RST_IN       P5DIR &= ~DS_RST
#define DS_RST_OUT      P5DIR |= DS_RST
#define DS_RST0         P5OUT &= ~DS_RST
#define DS_RST1         P5OUT |= DS_RST
#define DS_SCL_IN       P5DIR &= ~DS_SCL
#define DS_SCL_OUT      P5DIR |= DS_SCL
#define DS_SCL0         P5OUT &= ~DS_SCL
#define DS_SCL1         P5OUT |= DS_SCL
#define DS_SDA_IN       P5DIR &= ~DS_SDA
#define DS_SDA_OUT      P5DIR |= DS_SDA
#define DS_SDA0         P5OUT &= ~DS_SDA
#define DS_SDA1         P5OUT |= DS_SDA
#define DS_SDA_BIT      P5IN & DS_SDA


/***************************** 函数声明 *************************************/
void ShowTime(void);
void Init_DS1302(void);
void Write1Byte(unsigned char wdata);
unsigned char Read1Byte(void);
void W_Data(unsigned char addr, unsigned char wdata);
unsigned char R_Data(unsigned char addr);
void BurstWrite1302(unsigned char *ptr);
void BurstRead1302(unsigned char *ptr);
void BurstWriteRAM(unsigned char *ptr);
void BurstReadRAM(unsigned char *ptr);
void Set_DS1302(unsigned char *ptr);
void Get_DS1302(unsigned char *ptr);

#endif /*__DS1302_H*/