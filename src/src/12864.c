#include "12864.h"
#include "user.h"

uchar Dis_Num[] = {"0123456789."};

/*------------------------------------------------
函数功能；发送1字节数据
------------------------------------------------*/
void sendbyte(uchar Sbyte)
{
  uchar i;
  for(i=0;i<8;i++)
  {
    SCLK0;  //串行数据置0
    if(Sbyte & 0x80) SID1; 
    else SID0;
    // delay1ms(1);
    SCLK1;  //串行数据置1
    // delay1ms(1);
    SCLK0;  //串行数据置0
    //delay1ms(1);
    Sbyte = Sbyte << 1;
  }
}
/*---------------------------------------------
写指令
---------------------------------------------*/
void writecmd(uchar dicate)
{ 
  //CS1;     //片选信号置1
  //delay1ms(1);
  sendbyte(0xf8);      //1111 1000   RW(0),RS(0)
  sendbyte(0xf0 & dicate);   //传高四位数据
  sendbyte((0x0f&dicate)<<4);   //传第四位数据
  //delay1ms(1);
  //CS0;
}
/*----------------------------------------------
函数功能：将显示地址写入12864
入口参数：显示行x,显示列y,从零开始计数，例如（0，0）为第一行第一列
返回值：无
-----------------------------------------------*/
void writeaddress(uchar x,uchar y)
{
  if(x==0)
    writecmd(0x80|y);
  if(x==1)
    writecmd(0x90|y);
  if(x==2)
    writecmd((0x80|y)+8);  //需要加8，参照1602的使用方法和12864的指令
  if(x==3)
    writecmd((0x90|y)+8);
}
/*---------------------------------------------
写数据
---------------------------------------------*/
void writedata(uchar dattemp)
{
  //CS1;     //片选信号置1
  //delay1ms(1);
  sendbyte(0xfa);     //1111 1010  RW(0),RS(1)
  sendbyte(0xf0 & dattemp);   //传高四位数据
  sendbyte((0x0f&dattemp)<<4);   //传第四位数据
  //delay1ms(1);
  //CS0;
 }
/*--------------------------------------------
显示字符串
--------------------------------------------*/
void print(uchar *str)
{
  while(*str!='\0')
  {
     writedata(*str);
    str++;
  }
}
/*---------------------------------------------
12864初始化
----------------------------------------------*/
void Init_LCD()
{
  P4DIR |= 0x06;       //0000 0110
  //PSB0;          //选择串行通信模式
  delay_ms(40);
  writecmd(0x30); //功能设定：8为MPU接口，基本指令集
  _NOP();
  writecmd(0x30);
  _NOP();
  writecmd(0xc); //显示开关设置：整体显示开，游标显示关，正常显示
  _NOP();
  writecmd(0x01); //清除屏幕
  delay_ms(10);
  writecmd(0x06);  //进入设定点：游标右移，DDRAM地址计数器（AC）加1
}