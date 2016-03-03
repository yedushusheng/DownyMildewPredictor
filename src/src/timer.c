#include "timer.h"
#include "user.h"
/*************************** ���������� ******************************/
volatile uchar Hour = 0;      //Сʱ�����ӣ���
volatile uchar Min = 0;
volatile uchar Sec = 0;
volatile uchar n = 0;
/***************************************************************
�������ܣ���ʼ��TIMER_A
��ڲ�������
����ֵ����
***************************************************************/
void Init_Timer_A(void)
{
  TACTL |= TASSEL_1 + MC_2;                  // SMCLK, contmode
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 32768;
}
// Timer A0 interrupt service routine
/**************************************************************
�������ܣ�Timer_A �ж�
**************************************************************/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  n++;
  P2OUT ^= BIT7;
  if(n == 20)
  {
    n = 0;
    Sec++;    
    if(Sec == 60)
    { 
      Sec = 0;
      Min++;
      if(Min == 60)
      {
        Min = 0;
        Hour++;
        if(Hour == 24)
        {
          Hour = 0;
        } 
      }
    }
  }
  CCR0 += 32768;                            // Add Offset to CCR0
}

