#ifndef __DHT11_H
#define __DHT11_H

#include "msp430x14x.h"
#include "user.h"
/******************************* �˿ڶ��� ***********************************/
#define SET_H   P5DIR|=BIT0;P5OUT|=BIT0      //SETH
#define SET_L   P5DIR|=BIT0;P5OUT&=~BIT0     //SETL
#define PIN0    P5IN   
#define SET_IN  P5DIR&=~BIT0; 

/***************************** ���������� *******************************/
uchar COM(void);
void RH(void);

#endif /*__DHT11_H*/