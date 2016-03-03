#ifndef __USART0_H
#define __USART0_H

#include <msp430x14x.h>

#define uchar unsigned char
#define uint unsigned int
/*------------------------------ º¯ÊýÉùÃ÷Çø ---------------------------------*/
void Init_USART0(void);
void Init_USART1(void);
void USART0_Send_Byte(uchar Byte);
void USART1_Send_Byte(uchar Byte);
void USART0_Send_String(uchar *ptr);
void USART1_Send_String(uchar *ptr);
uchar USART0_Rev_Byte(void);
uchar USART1_Rev_Byte(void);
//uchar USART0_Rev_String(void);
//uchar USART0_Rev_String(void);

#endif /*__USART0_H*/