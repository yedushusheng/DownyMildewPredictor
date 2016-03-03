#include "ds1302.h"
#include "12864.h"
#include "user.h"

/************************* 变量定义去 ********************************/
uchar times[7];     //用于存储时间数据
extern uchar Dis_Num[];
/*******************************************
函数名称：Reset_DS1302
功    能：对DS1302进行复位操作
参    数：无
返回值  ：无
********************************************/
void Init_DS1302(void)
{
  uchar flag;
  DS_RST_OUT;  //RST对应的IO设置为输出状态
  DS_SCL_OUT;  //SCLK对应的IO设置为输出状态
  DS_SCL0;     //SCLK=0
  DS_RST0;     //RST=0
  //delay_ms(1);
  DS_SCL1;    //SCLK=1
  flag = R_Data(0x81); //判断时钟芯片是否关闭 1为关闭
  if(flag&0x80)
  {  
    W_Data(0x8e,0x00);                //根据写状态寄存器命令字，写入不保护指令
    W_Data(0x80,((56/10)<<4|(56%10)));  //根据写秒寄存器命令字，写入秒的初始值
    W_Data(0x82,((59/10)<<4|(59%10)));  //根据写分寄存器命令字，写入分的初始值
    W_Data(0x84,((23/10)<<4|(23%10)));	 //根据写时寄存器命令字，写入时的初始值
    W_Data(0x86,((29/10)<<4|(29%10)));  //根据写日寄存器命令字，写入日的初始值
    W_Data(0x88,((12/10)<<4|(12%10)));	 //根据写月寄存器命令字，写入月的初始值
    W_Data(0x8c,((11/10)<<4|(11%10)));	 //根据写年寄存器命令字，写入年的初始值
    W_Data(0x90,0xa5);				 //打开充电模式，选择2K电阻充电模式
    W_Data(0x8e,0x80);				 //根据写状态寄存器命令字，写入保护指令
  }	
}
/*******************************************
函数名称：Write1Byte
功    能：对DS1302写入1个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write1Byte(uchar wdata) 
{ 
  uchar i;
  
  DS_SDA_OUT;     //SDA对应的IO设置为输出状态
  DS_RST1;        //REST=1;
  
  for(i=8; i>0; i--) 
  {
    if(wdata&0x01)  DS_SDA1;
    else            DS_SDA0;
    DS_SCL0;
   // delay_ms(1); 
    DS_SCL1;
   // delay_ms(1); 
    wdata >>=1; 
  } 
}
/*******************************************
函数名称：Read1Byte
功    能：从DS1302读出1个字节的数据
参    数：无
返回值  ：读出的一个字节数据
********************************************/
uchar Read1Byte(void) 
{ 
    uchar i;
    uchar rdata=0X00;

    DS_SDA_IN;  //SDA对应的IO设置为输入状态
    DS_RST1;    //REST=1;

    for(i=8; i>0; i--)
    {
        DS_SCL1;
        delay_ms(1);
        DS_SCL0;
        delay_ms(1);
        rdata >>=1;
        if(DS_SDA_BIT)  rdata |= 0x80;
    } 
    
    return(rdata); 
}
/*******************************************
函数名称：W_Data
功    能：向某个寄存器写入一个字节数据
参    数：addr--寄存器地址
          wdata--写入的数据
返回值  ：无
********************************************/
void W_Data(uchar addr, uchar wdata)
{
    DS_RST0; 
    DS_SCL0;   
    _NOP();
    DS_RST1;
    Write1Byte(addr);   //写入地址
    Write1Byte(wdata);  //写入数据
    DS_SCL1;
    DS_RST0;
} 
/*******************************************
函数名称：R_Data
功    能：从某个寄存器读出一个字节数据
参    数：addr--寄存器地址
返回值  ：读出的数据
********************************************/
uchar R_Data(uchar addr)
{
    uchar rdata;
    
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(addr);    //写入地址
    rdata = Read1Byte();  //读出数据 
    DS_SCL1;
    DS_RST0;
    
    return(rdata);
}
/*******************************************
函数名称：BurstWrite1302
功    能：以burst方式向DS1302写入批量时间数据
参    数：ptr--指向时间数据存放地址的指针
返回值  ：读出的数据
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstWrite1302(uchar *ptr)
{
    uchar i;
    
    W_Data(0x8e,0x00);      //允许写入     
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xbe);       // 0xbe:时钟多字节写入命令 
    for (i=8; i>0; i--) 
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0;
    W_Data(0x8e,0x80);      // 禁止写入
} 
/*******************************************
函数名称：BurstRead1302
功    能：以burst方式从DS1302读出批量时间数据
参    数：ptr--指向存放时间数据地址的指针
返回值  ：无
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstRead1302(uchar *ptr)
{
    uchar i;
  
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xbf);             //0xbf:时钟多字节读命令
    for (i=8; i>0; i--) 
    {
       *ptr++ = Read1Byte(); 
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
函数名称：BurstWriteRAM
功    能：以burst方式向DS1302的RAM中写入批量数据
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：共写入31个字节的数据
********************************************/
void BurstWriteRAM(uchar *ptr)
{
    uchar i;
    
    W_Data(0x8e,0x00);         //允许写入
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xfe);          //0xfe:RAM多字节写命令 
    for (i = 31; i>0; i--)     //RAM共有31个字节
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0; 
    W_Data(0x8e,0x80);          //禁止写入
} 
/*******************************************
函数名称：BurstReadRAM
功    能：以burst方式从DS1302的RAM中读出批量数据
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：共读出31个字节的数据
********************************************/
void BurstReadRAM(uchar *ptr)
{
    uchar i;
  
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xff);             //0xff:RAM的多字节读命令 
    for (i=31; i>0; i--) 
    {
       *ptr++ = Read1Byte();   
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
函数名称：Set_DS1302
功    能：设置DS1302内部的时间
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：写入数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Set_DS1302(uchar *ptr) 
{
    uchar i;
    uchar addr = 0x80; 
    
    W_Data(0x8e,0x00);    //允许写入
    
    for(i =7;i>0;i--)
    { 
        W_Data(addr,*ptr++); 
        addr += 2;
    }
    W_Data(0x8e,0x80);      //禁止
}
/******************************************************************** 
*
* 名称: Get_DS1302
* 说明: 
* 功能: 读取DS1302当前时间
* 调用: R_Data(uchar addr) 
* 输入: ucCurtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年 
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
/*******************************************
函数名称：Get_DS1302
功    能：读取DS1302内部的时间
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：读出数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Get_DS1302(uchar *ptr) 
{
    uchar i;
    uchar addr=0x81;
    
    for(i=0;i<7;i++)
    {
        ptr[i]=R_Data(addr);
        addr+=2;
    }
}
/********************************************
函数名称：ShowTime
功    能：将DS1302的时间转换成10进制显示
参    数：无
返回值  ：无
********************************************/    
void ShowTime(void)
{  
   uchar h1[14];       //第1行显示数据
   uchar h2[8];        //第2行显示数据
    
   h1[0] = Dis_Num[2];
   h1[1] = Dis_Num[0];
   h1[2] = Dis_Num[times[6]>>4];     //年
   h1[3] = Dis_Num[times[6]&0x0f];
   writeaddress(2,1);
   writedata(h1[2]);
   writedata(h1[3]);
   // h1[4] = 0x2d;       //"-"
   h1[5] = Dis_Num[times[4]>>4];     //月
   h1[6] = Dis_Num[times[4]&0x0f];
   writeaddress(2,3);
   writedata(h1[5]);
   writedata(h1[6]);
   h1[7] = 0x2d;       //"-"
   h1[8] = Dis_Num[times[3]>>4];     //日
   h1[9] = Dis_Num[times[3]&0x0f];
   writeaddress(2,5);
   writedata(h1[8]);
   writedata(h1[9]);
   h1[10] = 0x20;      //" "
   h1[11] = 0x2a;      //"*"
   h1[12] = Dis_Num[times[5]];       //周
   h1[13] = 0x2a;      //"*"   //在第一行显示
   
   h2[0] = Dis_Num[times[2]>>4];     //时
   h2[1] = Dis_Num[times[2]&0x0f];
   writeaddress(3,0);
   writedata(h2[0]);
   writedata(h2[1]);
   print(": ");
    //h2[2] = 0x30;       //":"
   h2[3] = Dis_Num[times[1]>>4];     //分
   h2[4] = Dis_Num[times[1]&0x0f];
   writeaddress(3,2);
   writedata(h2[3]);
   writedata(h2[4]);
   print(": ");
   //h2[5] = 0x30;       //":"
   h2[6] = Dis_Num[times[0]>>4];     //秒
   h2[7] = Dis_Num[times[0]&0x0f];
   writeaddress(3,4);
   writedata(h2[6]);
   writedata(h2[7]);
}
