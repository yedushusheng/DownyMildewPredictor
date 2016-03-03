#ifndef __BH1750_H
#define __BH1750_H

#include "msp430x14x.h"
#include "user.h"

#define	  SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0x47

/******************************** �˿ڶ��� ************************************/
/*#define BH1750_DIR P3DIR
#define BH1750_OUT P3OUT
#define BH1750_IN  P3IN

#define BH1750_SCL  BIT0
#define BH1750_SDA  BIT1*/

#define BH1750_SCL_OUT  P3DIR |= BIT0
#define BH1750_SDA_OUT  P3DIR |= BIT1
#define BH1750_SDA_IN   P3DIR &= ~BIT1

#define SET_SCL   P3OUT |= BIT0
#define CLR_SCL   P3OUT &= ~BIT0
#define SET_SDA   P3OUT |= BIT1
#define CLR_SDA   P3OUT &= ~BIT1
#define SDA_IN    P3IN & BIT1
/********************************** ���������� ***********************************/
void  Init_BH1750(void);
void  Single_Write_BH1750(uchar REG_Address);               //����д������
uchar Single_Read_BH1750(uchar REG_Address);                //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_BH1750(void);                               //�����Ķ�ȡ�ڲ��Ĵ�������
void  BH1750_Start(void);                    //��ʼ�ź�
void  BH1750_Stop(void);                     //ֹͣ�ź�
void  BH1750_SendACK(uchar ack);           //Ӧ��ACK
uchar BH1750_RecvACK(void);                  //��ack
void  BH1750_SendByte(uchar dat);         //IIC�����ֽ�д
uchar BH1750_RecvByte(void);                 //IIC�����ֽڶ�

#endif
