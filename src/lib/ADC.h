#ifndef __ADC_H
#define __ADC_H

#include "msp430x14x.h"
#include "user.h"

/******************** 电压系数 ***********************/
#define Voltage_Adjust  256

/*-----------------------------------------
函数声明区
-----------------------------------------*/
void Init_ADC(void);

#endif /*__ADC_H*/