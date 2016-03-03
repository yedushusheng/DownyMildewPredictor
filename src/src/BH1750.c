#include "BH1750.h"
#include "user.h"
/********************************* 变量定义区 ************************************/
uint Light;
/**************************************
起始信号
**************************************/
void BH1750_Start(void)
{ 
  SET_SCL;
  SET_SDA;
  delay_us(5);                 //延时
  CLR_SDA;                //产生下降沿
  delay_us(5);                 //延时
  CLR_SCL;                    //拉低时钟线
}
/**************************************
停止信号
**************************************/
void BH1750_Stop(void)
{
  CLR_SDA;                    //拉低数据线
  SET_SCL;                    //拉高时钟线
  delay_us(5);                 //延时
  SET_SDA;                    //产生上升沿
  delay_us(5);                 //延时
}
/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(uchar ACK)
{
  if(ACK)
   SET_SDA;
  else
   CLR_SDA;
  SET_SCL;                    //拉高时钟线
  delay_us(5);                 //延时
  CLR_SCL;                    //拉低时钟线
  delay_us(5);                 //延时
}
/**************************************
接收应答信号
**************************************/
uchar BH1750_RecvACK(void)
{
  uchar tmp = 0;    
  
  BH1750_SDA_IN;           //数据口输入
  
  SET_SCL;                    //拉高时钟线
  delay_us(5);                 //延时
  if(SDA_IN)
    tmp |= BIT0;                   //读应答信号
  CLR_SCL;                    //拉低时钟线
  delay_us(5);                 //延时
  
  BH1750_SDA_OUT;           //数据口输出
  
  return tmp;
}
/**************************************
向IIC总线发送一个字节数据
**************************************/
void BH1750_SendByte(uchar dat)
{
  uchar i; 
  
  for (i=0; i<8; i++)         //8位计数器
  {
    if(dat & BIT7)  //先发出最高位
      SET_SDA;
    else 
      CLR_SDA;
    dat <<= 1;              //移出数据的最高位
    SET_SCL;                //拉高时钟线
    delay_us(5);             //延时
    CLR_SCL;                //拉低时钟线
    delay_us(5);             //延时
  }
  BH1750_RecvACK();
}
/**************************************
从IIC总线接收一个字节数据
**************************************/
uchar BH1750_RecvByte(void)
{
  uchar i;
  uchar dat = 0;
  
  BH1750_SDA_IN;           //数据口输入
  
  for (i=0; i<8; i++)         //8位计数器
  {
    dat <<= 1;
    SET_SCL;                //拉高时钟线
    delay_us(5);             //延时
    if(SDA_IN)
      dat |= BIT0;             //读数据 
    else
      dat &= ~BIT0;
    CLR_SCL;                //拉低时钟线
    delay_us(5);             //延时
  }
  
  BH1750_SDA_OUT;           //数据口输入
  
  return dat;
}
//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
  BH1750_Start();                  //起始信号
  BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
  BH1750_SendByte(REG_Address);    //内部寄存器地址，请参考中文pdf22页 
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf22页 
  BH1750_Stop();                   //发送停止信号
}
//********单字节读取*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  
  uchar REG_data;
  BH1750_Start();                          //起始信号
  BH1750_SendByte(SlaveAddress);           //发送设备地址+写信号
  BH1750_SendByte(REG_Address);                   //发送存储单元地址，从0开始	
  BH1750_Start();                          //起始信号
  BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
  REG_data=BH1750_RecvByte();              //读出寄存器数据
  BH1750_SendACK(1);   
  BH1750_Stop();                           //停止信号
  return REG_data; 
}
*/
//*********************************************************
//
//连续读出BH1750内部数据
//
//*********************************************************
void Multiple_Read_BH1750(void)
{   
  uchar   BUF[4];                         //接收数据缓存区  
  uchar i;
  
  BH1750_Start();                          //起始信号
  BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	
  for (i=0; i<3; i++)                      //连续读取6个地址数据，存储中BUF
  {
    BUF[i] = BH1750_RecvByte();          //BUF[0]存储0x32地址中的数据
    if (i == 3)
    {
      BH1750_SendACK(1);                //最后一个数据需要回NOACK
    }
    else
    {		
      BH1750_SendACK(0);                //回应ACK
    }
  }
  BH1750_Stop();                          //停止信号
  Light = BUF[0];
  Light = (Light << 8) + BUF[1];//合成数据 
  Light = Light*10/12;
  
  delay_ms(5);
}
//初始化BH1750，根据需要请参考pdf进行修改****
void Init_BH1750(void)
{
  BH1750_SCL_OUT;
  BH1750_SDA_OUT;
  Single_Write_BH1750(0x01);   // power on
  Single_Write_BH1750(0x10);   // H- resolution mode 
}

