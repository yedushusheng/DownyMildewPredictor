#include "ADC.h"

/************************ 变量定义 ************************/
unsigned long int  result;
volatile unsigned long int  RH_soil;
/*--------------------------------------------------------
函数功能：模数转换器初始化
入口参数：无
返回值：无
--------------------------------------------------------*/
void Init_ADC()
{
  ADC12CTL0 = SHT0_2+ADC12ON+MSC;      //设置AD采样时间，打开AD
  ADC12CTL1 = SHP+CONSEQ_3;             //采样定时器，时钟MCLK，多通道多次转换
  ADC12MCTL0 = INCH_0;                      // ref+=AVcc, channel = A0
  //ADC12MCTL1 = INCH_1;                      // ref+=AVcc, channel = A1
  //ADC12MCTL2 = INCH_2;                      // ref+=AVcc, channel = A2
  //ADC12IE = 0x01;                 //开AD中断
  ADC12CTL0 |= ENC;                 //转换使能
  ADC12CTL0 |= ADC12SC;            // 开始转换
  P6SEL |= BIT0+BIT1;                  // P6.0选择第二功能，AD通道1输入
}
/*-------------------------------------------------------
AD中断
--------------------------------------------------------*/
#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR(void)
{
  result = ADC12MEM0;                   // Move results, IFG is cleared
  RH_soil = result*100/4095;
  ADC12IE &= ~0x01;                 //开AD中断
}