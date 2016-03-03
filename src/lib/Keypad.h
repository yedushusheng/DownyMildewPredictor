#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "msp430x14x.h"
#include "user.h"
/*************************** �˿ڶ�����  *************************/
#define FMQ_DIR_Out P6DIR |= BIT7
#define Set_FMQ P6OUT |= BIT7 
#define CLR_FMQ P6OUT &= ~BIT7 
/*************************** ���������� *************************/
void beep(void);
void Init_Keypad(void);
void Check_Key(void);
void Key_Event(void);

#endif /*__KEYPAD_H*/