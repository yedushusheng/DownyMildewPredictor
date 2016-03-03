#include "ADC.h"

/************************ �������� ************************/
unsigned long int  result;
volatile unsigned long int  RH_soil;
/*--------------------------------------------------------
�������ܣ�ģ��ת������ʼ��
��ڲ�������
����ֵ����
--------------------------------------------------------*/
void Init_ADC()
{
  ADC12CTL0 = SHT0_2+ADC12ON+MSC;      //����AD����ʱ�䣬��AD
  ADC12CTL1 = SHP+CONSEQ_3;             //������ʱ����ʱ��MCLK����ͨ�����ת��
  ADC12MCTL0 = INCH_0;                      // ref+=AVcc, channel = A0
  //ADC12MCTL1 = INCH_1;                      // ref+=AVcc, channel = A1
  //ADC12MCTL2 = INCH_2;                      // ref+=AVcc, channel = A2
  //ADC12IE = 0x01;                 //��AD�ж�
  ADC12CTL0 |= ENC;                 //ת��ʹ��
  ADC12CTL0 |= ADC12SC;            // ��ʼת��
  P6SEL |= BIT0+BIT1;                  // P6.0ѡ��ڶ����ܣ�ADͨ��1����
}
/*-------------------------------------------------------
AD�ж�
--------------------------------------------------------*/
#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR(void)
{
  result = ADC12MEM0;                   // Move results, IFG is cleared
  RH_soil = result*100/4095;
  ADC12IE &= ~0x01;                 //��AD�ж�
}