#include "BH1750.h"
#include "user.h"
/********************************* ���������� ************************************/
uint Light;
/**************************************
��ʼ�ź�
**************************************/
void BH1750_Start(void)
{ 
  SET_SCL;
  SET_SDA;
  delay_us(5);                 //��ʱ
  CLR_SDA;                //�����½���
  delay_us(5);                 //��ʱ
  CLR_SCL;                    //����ʱ����
}
/**************************************
ֹͣ�ź�
**************************************/
void BH1750_Stop(void)
{
  CLR_SDA;                    //����������
  SET_SCL;                    //����ʱ����
  delay_us(5);                 //��ʱ
  SET_SDA;                    //����������
  delay_us(5);                 //��ʱ
}
/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(uchar ACK)
{
  if(ACK)
   SET_SDA;
  else
   CLR_SDA;
  SET_SCL;                    //����ʱ����
  delay_us(5);                 //��ʱ
  CLR_SCL;                    //����ʱ����
  delay_us(5);                 //��ʱ
}
/**************************************
����Ӧ���ź�
**************************************/
uchar BH1750_RecvACK(void)
{
  uchar tmp = 0;    
  
  BH1750_SDA_IN;           //���ݿ�����
  
  SET_SCL;                    //����ʱ����
  delay_us(5);                 //��ʱ
  if(SDA_IN)
    tmp |= BIT0;                   //��Ӧ���ź�
  CLR_SCL;                    //����ʱ����
  delay_us(5);                 //��ʱ
  
  BH1750_SDA_OUT;           //���ݿ����
  
  return tmp;
}
/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void BH1750_SendByte(uchar dat)
{
  uchar i; 
  
  for (i=0; i<8; i++)         //8λ������
  {
    if(dat & BIT7)  //�ȷ������λ
      SET_SDA;
    else 
      CLR_SDA;
    dat <<= 1;              //�Ƴ����ݵ����λ
    SET_SCL;                //����ʱ����
    delay_us(5);             //��ʱ
    CLR_SCL;                //����ʱ����
    delay_us(5);             //��ʱ
  }
  BH1750_RecvACK();
}
/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
uchar BH1750_RecvByte(void)
{
  uchar i;
  uchar dat = 0;
  
  BH1750_SDA_IN;           //���ݿ�����
  
  for (i=0; i<8; i++)         //8λ������
  {
    dat <<= 1;
    SET_SCL;                //����ʱ����
    delay_us(5);             //��ʱ
    if(SDA_IN)
      dat |= BIT0;             //������ 
    else
      dat &= ~BIT0;
    CLR_SCL;                //����ʱ����
    delay_us(5);             //��ʱ
  }
  
  BH1750_SDA_OUT;           //���ݿ�����
  
  return dat;
}
//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
  BH1750_Start();                  //��ʼ�ź�
  BH1750_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
  BH1750_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
  //  BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
  BH1750_Stop();                   //����ֹͣ�ź�
}
//********���ֽڶ�ȡ*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  
  uchar REG_data;
  BH1750_Start();                          //��ʼ�ź�
  BH1750_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
  BH1750_SendByte(REG_Address);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
  BH1750_Start();                          //��ʼ�ź�
  BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
  REG_data=BH1750_RecvByte();              //�����Ĵ�������
  BH1750_SendACK(1);   
  BH1750_Stop();                           //ֹͣ�ź�
  return REG_data; 
}
*/
//*********************************************************
//
//��������BH1750�ڲ�����
//
//*********************************************************
void Multiple_Read_BH1750(void)
{   
  uchar   BUF[4];                         //�������ݻ�����  
  uchar i;
  
  BH1750_Start();                          //��ʼ�ź�
  BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	
  for (i=0; i<3; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
  {
    BUF[i] = BH1750_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
    if (i == 3)
    {
      BH1750_SendACK(1);                //���һ��������Ҫ��NOACK
    }
    else
    {		
      BH1750_SendACK(0);                //��ӦACK
    }
  }
  BH1750_Stop();                          //ֹͣ�ź�
  Light = BUF[0];
  Light = (Light << 8) + BUF[1];//�ϳ����� 
  Light = Light*10/12;
  
  delay_ms(5);
}
//��ʼ��BH1750��������Ҫ��ο�pdf�����޸�****
void Init_BH1750(void)
{
  BH1750_SCL_OUT;
  BH1750_SDA_OUT;
  Single_Write_BH1750(0x01);   // power on
  Single_Write_BH1750(0x10);   // H- resolution mode 
}

