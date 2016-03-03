#ifndef __BH1750_H
#define __BH1750_H

#include "msp430x14x.h"
#include "user.h"

#define	  SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为0x47

/******************************** 端口定义 ************************************/
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
/********************************** 函数声明区 ***********************************/
void  Init_BH1750(void);
void  Single_Write_BH1750(uchar REG_Address);               //单个写入数据
uchar Single_Read_BH1750(uchar REG_Address);                //单个读取内部寄存器数据
void  Multiple_Read_BH1750(void);                               //连续的读取内部寄存器数据
void  BH1750_Start(void);                    //起始信号
void  BH1750_Stop(void);                     //停止信号
void  BH1750_SendACK(uchar ack);           //应答ACK
uchar BH1750_RecvACK(void);                  //读ack
void  BH1750_SendByte(uchar dat);         //IIC单个字节写
uchar BH1750_RecvByte(void);                 //IIC单个字节读

#endif
