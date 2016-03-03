#include "Keypad.h"
/***************全局变量***************/
uchar Key_Pressed;      //按键是否被按下:1--是，0--否
uchar Key_Val;          //存放键值
uchar Key_Flag;         //按键是否已放开：1--是，0--否
//设置键盘逻辑键值与程序计算键值的映射
uchar key_Map[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

/*******************************************
函数名称：beep()
功    能：蜂鸣器
参    数：无
返回值  ：无
********************************************/
void beep(void)
{  
  uint t; 
  FMQ_DIR_Out;
  for(t=0;t<200;t++)
  {
    Set_FMQ;
    delay500us();
    CLR_FMQ;
    delay500us();
  }
}
/*******************************************
函数名称：Init_Keypad
功    能：初始化扫描键盘的IO端口
参    数：无
返回值  ：无
********************************************/
void Init_Keypad(void)
{     
    /* 按键采用中断的方式，以下是对中断的一些配置 */
   // P1IES = 0x0f;  //P1.0~P1.3选择下降沿中断  
   // P1IE = 0X0f;  //打开中断使能
    //P1DIR &= 0xf0;  // P10-P13输入
    
    P1DIR = 0xf0;       //P1.0~P1.3设置为输入状态, P1.4~P1.7设置为输出状态
    P1OUT |= 0x00;      // P1.4~P1.7输出低电平
    Key_Flag = 0;       
    Key_Pressed = 0;   
    Key_Val = 0;
}
/*******************************************
函数名称：Check_Key
功    能：扫描键盘的IO端口，获得键值
参    数：无
返回值  ：无
********************************************/
void Check_Key(void)
{
    uchar row ,col,tmp1,tmp2;
    
    tmp1 = 0x80;
    for(row = 0;row < 4;row++)              //行扫描
    {
        P1OUT = 0xf0;                      //P1.4~P1.7输出全1
        P1OUT -= tmp1;                      //P1.4~p1.7输出四位中有一个为0
        tmp1 >>=1;                              
        if ((P1IN & 0x0f) < 0x0f)           //是否P1IN的P1.0~P1.3中有一位为0
        {                       
            tmp2 = 0x01;                         // tmp2用于检测出那一位为0  
            for(col = 0;col < 4;col++)              // 列检测 
            {          
                if((P1IN & tmp2) == 0x00)           // 是否是该列,等于0为是
                {          
                    Key_Val = key_Map[row * 4 + col];  // 获取键值
                    return;                         // 退出循环
                }
                tmp2 <<= 1;                        // tmp2右移1位 
            }
        }
    }      
}
/*******************************************
函数名称：Key_Event
功    能：检测按键，并获取键值
参    数：无
返回值  ：无
********************************************/
void Key_Event(void)
{     
    uchar tmp;
     
    P1OUT &= 0x00;              // 设置P1OUT全为0，等待按键输入
    tmp = P1IN;                 // 获取 p1IN
    if ((Key_Pressed == 0x00)&&((tmp & 0x0f) < 0x0f)) //如果有键按下
    {                     
        Key_Pressed = 1;        // 如果有按键按下，设置Key_Pressed标识
        delay_ms(10);                //消除抖动
        Check_Key();            // 调用check_Key(),获取键值 
        beep();                 //蜂鸣器
    }     
    else if ((Key_Pressed == 1)&&((tmp & 0x0f) == 0x0f)) //如果按键已经释放
    {                     
        Key_Pressed = 0;        // 清除Key_Pressed标识
        Key_Flag    = 1;        // 设置Key_Flag标识
    }
    else 
    {
        _NOP();
    }     
}
