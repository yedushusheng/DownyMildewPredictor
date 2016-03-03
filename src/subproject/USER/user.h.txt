#ifndef __USER_H
#define __USER_H

#include "msp430x14x.h"

#define uchar unsigned char
#define uint unsigned int

/******************************** 用于延时 ************************************/
#define CPU_F ((double)8000000)         //当前CPU的频率
//#define CPU_F ((double)1000000)         //当前CPU的频率
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

/************************ 函数声明区 ***************************/
void delay500us(void);

#endif /*__USER_H*/