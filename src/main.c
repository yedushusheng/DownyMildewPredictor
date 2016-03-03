/******************************************************************************
工程名称：温室大棚控制系统
现有功能：温室度显示正常、时间日期显示正常
日    期：2014.5.13   14
*******************************************************************************/
#include "msp430x14x.h"
#include "12864.h"
#include "Keypad.h"
#include "DHT11.h"
#include "ds1302.h"
#include "ADC.h"
#include "user.h"
#include "timer.h"
//#include "USART.h"
#include "BH1750.h"
/**************************** 端口定义区 ********************************/
#define LED_RED_ON    P2DIR |= BIT1;P2OUT &= ~BIT1
#define LED_RED_OFF   P2DIR |= BIT1;P2OUT |= BIT1

#define LED_WHITE_ON  P2DIR |= BIT2;P2OUT &= ~BIT2
#define LED_WHITE_OFF P2DIR |= BIT2;P2OUT |= BIT2

#define LED_GREEN_ON  P2DIR |= BIT3;P2OUT &= ~BIT3
#define LED_GREEN_OFF P2DIR |= BIT3;P2OUT |= BIT3

#define FAN_ON        P3DIR |= BIT2;P3OUT |= BIT2
#define FAN_OFF       P3DIR |= BIT2;P3OUT &= ~BIT2

#define PENG_ON       P3DIR |= BIT3;P3OUT |= BIT3
#define PENG_OFF      P3DIR |= BIT4;P3OUT &= ~BIT4
#define PENG_STAY     P3OUT &= ~BIT3;P3OUT &= ~BIT4

#define BEEP_ON       P3DIR |= BIT5;P3OUT |= BIT5
#define BEEP_OFF      P3DIR |= BIT5;P3OUT &= ~BIT5

#define WATER_ON      P3DIR |= BIT6;P3OUT |= BIT6
#define WATER_OFF     P3DIR |= BIT6;P3OUT &= ~BIT6
/**************************** 变量定义去 *******************************/
extern uchar times[7];     //用于存储时间数据
extern uchar Dis_Num[];
extern uchar Key_Val;
extern uchar Key_Flag;
extern uchar T_data_H,T_data_L,RH_data_H,RH_data_L,checkdata; 
extern uint Light;
extern volatile uchar Sec;
extern volatile unsigned long int RH_soil;;
extern unsigned long int  result;

uchar PENG_Flag = 0;     //0表示关着  1表示开着
uchar Dis_Flag = 0;     //切屏变量 
uchar T_MAX = 30;               //温度上限 初始值为30
uchar T_MIN = 15;               //温度下限 初始值为15
uchar PP=0;      //游标位置变量
uchar cflag=0;;  //是否处于修改状态标志位

/*******************************************************************************
函数功能：时钟初始化
入口参数：无
返回值：无
*******************************************************************************/
void Init_Clock(void)
{
  uchar k;
  BCSCTL1 &= ~XT2OFF;    //开启XT2晶振
  do 
  {
    IFG1 &= ~OFIFG;           // 清除振荡器失效标志
    for(k=0xee;k>0;k--){}      //延时
  
  }
  while((IFG1 & OFIFG)!=0);        //判断XT2是否起振
  BCSCTL2 = SELM_2 + SELS;     // 选择MCLK SMCLK 为XT2   8M
  
}   
/*******************************************************************************
函数功能：系统初始化
入口参数：无
返回值：无
*******************************************************************************/
void Init_System(void)
{
  Init_Clock();
  
  delay_ms(50);
  //Init_LCD();
 // delay_ms(50);
  //Init_Timer_A(); 
  delay_ms(50);
  //Init_USART0();
  delay_ms(50);
  Init_DS1302();
  delay_ms(50);
  Init_ADC();
  delay_ms(50);
  Init_Keypad();
  delay_ms(50);
  Init_BH1750();       //初始化BH1750
  
  P2IES = 0x01;  //P2.0选择下降沿中断
  P2IE = 0X01;  //打开中断使能
  P2DIR &= ~BIT0;  // P10-P13输入
}
/*******************************************************************************
函数功能：按键处理
入口参数：无
返回值：无
*******************************************************************************/
void Key_Process(void)
{
        if(Key_Flag == 1)
        {
          Key_Flag = 0;
          switch(Key_Val)
          {
            case 0x01:
              switch(PP++)      //确定游标地址
              {
                case 0: writeaddress(2,1);break;
                case 1: writeaddress(2,3);break;
                case 2: writeaddress(2,5);break;
                case 3: writeaddress(3,0);break;
                case 4: writeaddress(3,2);break;
                case 5: writeaddress(3,4);break;
                //case 6: writeaddress(1,1);break;
                default:break;
              }                  
              writecmd(0x0f);   //打开游标
              if(PP == 6) 
              PP = 0;
              cflag = 1;          //标志置位
              break;   
            case 0x02:
              if(cflag)
              {
                switch(PP)
                {
                  case 1:         //年
                        times[6]++;
                        if((times[6]&0x0f) == 0x0a)
                        {
                          times[6] += 0x06;       
                        }
                        if(times[6] > 0x99)
                        {
                          times[6] = 0x00;    
                        }
                        writedata(Dis_Num[times[6]>>4]);
                        writedata(Dis_Num[times[6]&0x0f]);
                        writeaddress(2,1); 
                                break;
                      case 2:         //月
                        times[4]++;
                        if((times[4]&0x0f) == 0x0a)
                        {
                          times[4] += 0x06;       
                        }
                        if(times[4]>0x12)
                        {
                          times[4]=0x01;       
                        }
                        writedata(Dis_Num[times[4]>>4]);
                        writedata(Dis_Num[times[4]&0x0f]);
                        writeaddress(2,3);
                        break;
                      case 3:         //日
                        times[3]++;
                        if((times[3]&0x0f) == 0x0a)
                        {
                          times[3] += 0x06;       
                        }
                        if(times[3] > 0x31)
                        {
                          times[3] = 0x01;    
                        }
                        writedata(Dis_Num[times[3]>>4]);
                        writedata(Dis_Num[times[3]&0x0f]);
                        writeaddress(2,5);
                        break;
                      case 4:         //时
                        times[2]++;
                        if((times[2]&0x0f) == 0x0a)
                        {
                          times[2] += 0x06;       
                        }
                        if(times[2] > 0x23)
                        {
                          times[2] = 0x00;    
                        }
                        writedata(Dis_Num[times[2]>>4]);
                        writedata(Dis_Num[times[2]&0x0f]);
                        writeaddress(3,0);
                        break;
                      case 5:         //分
                        times[1]++;
                        if((times[1]&0x0f) == 0x0a)
                        {
                          times[1] += 0x06;       
                        }
                        if(times[1] > 0x59)
                        {
                          times[1] = 0x00;    
                        }
                        writedata(Dis_Num[times[1]>>4]);
                        writedata(Dis_Num[times[1]&0x0f]);
                        writeaddress(3,2);
                        break;
                      case 0:         //秒
                        times[0]=0x00;
                        /* if((times[0]&0x0f) == 0x0a)
                        {
                          times[0] += 0x06;       
                        }
                        if(times[0] > 0x59)
                        {
                          times[0] = 0x00;    
                        }*/
                        writedata(Dis_Num[times[0]>>4]);
                        writedata(Dis_Num[times[0]&0x0f]);
                        writeaddress(3,4);
                        break;
                      default:
                        break;                                    
                    }
                 }
                  break;
                case 0x03:
                  if(cflag)
                  {
                    switch(PP-1)
                    {
                      case 0:         //年
                        times[6]--;
                        if((times[6]&0x0f) == 0x0f)
                        {
                          times[6] -= 0x06;       
                        }
                        if(times[6] ==0)
                        {
                          times[6] = 0x99;    
                        }
                        writedata(Dis_Num[times[6]>>4]);
                        writedata(Dis_Num[times[6]&0x0f]);
                        writeaddress(2,1); 
                        break;
                      case 1:         //月
                        times[4]--;
                        if((times[4]&0x0f) == 0x0f)
                        {
                          times[4] -= 0x06;       
                        }
                        if(times[4]==0x00)
                        {
                          times[4]=0x12;     
                        }
                        writedata(Dis_Num[times[4]>>4]);
                        writedata(Dis_Num[times[4]&0x0f]);
                        writeaddress(2,3);
                        break;
                      case 2:         //日
                        times[3]--;
                        if((times[3]&0x0f) == 0x0f)
                        {
                          times[3] -= 0x06;       
                        }
                        if(times[3] == 0x00)
                        {
                          times[3] = 0x31;    
                        }
                        writedata(Dis_Num[times[3]>>4]);
                        writedata(Dis_Num[times[3]&0x0f]);
                        writeaddress(2,5);
                        break;
                      case 3:         //时
                        times[2]--;
                        if((times[2]&0x0f) == 0x0f)
                        {
                          times[2] -= 0x06;       
                        }
                              
                        writedata(Dis_Num[times[2]>>4]);
                        writedata(Dis_Num[times[2]&0x0f]);
                        writeaddress(3,0);
                        break;
                      case 4:         //分
                        times[1]--;
                        if((times[1]&0x0f) == 0x0f)
                        {
                          times[1] -= 0x06;       
                        }
                                 
                        writedata(Dis_Num[times[1]>>4]);
                        writedata(Dis_Num[times[1]&0x0f]);
                        writeaddress(3,2);
                                break;
                      /* case 5:         //秒
                        times[0]=0x00;
                        if((times[0]&0x0f) == 0x0f)
                        {
                          times[0] -= 0x06;       
                        }
                              
                        writedata(Dis_Num[times[0]>>4]);
                        writedata(Dis_Num[times[0]&0x0f]);
                        writeaddress(3,3);
                        break;*/
                      default:
                        break;                                   
                     }
                   }
                 break;
               case 0x04:
                  if(cflag)
                  {
                    cflag = 0;
                    PP = 0;
                    writecmd(0x0c);   //关闭游标
                    Set_DS1302(times);
                    P1IFG=0x00;           
                  }
                  break;
               case 5:
                if(!PENG_Flag)
                {
                  PENG_ON;
                  delay_ms(10000);
                  PENG_STAY;
                  PENG_Flag = 1;
                }
                 break;
               case 6:
                if(PENG_Flag)
                {
                  PENG_OFF;
                  delay_ms(10000);
                  PENG_STAY;
                  PENG_Flag = 0;
                }
                 break;                 
               case 12:
                 Dis_Flag = 1;
                 break;
               case 0x10:
                 Dis_Flag = 2; 
                 break;                //按下16号键，自动跳到上一屏
               default:  break;                      //按下其他按键，不做任何操作       
               }
              }
}
/**************************** 主函数 ********************************/
void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Init_LCD();
  delay_ms(50);
  writeaddress(0,0);
  print("    欢迎使用    ");
  writeaddress(1,0);
  print("设施园艺监控系统");
  writeaddress(2,0);
  print("系统正在初始化");  
  writeaddress(3,0);
  print("请稍后..."); 
  Init_System();
  delay_ms(1000);
  _EINT();               //开中断
  while(1)
  {
    /************************ 第一屏：显示温湿度，时间日期 ********************/
    if(Dis_Flag == 0)
    {
      writecmd(0x01); //清除屏幕
      delay_ms(10);
      writeaddress(0,0);
      print("    监控系统    ");
      writeaddress(1,0);
      print("  T:  ℃  R:  % ");
      writeaddress(2,0);
      print("20  年  月  日");
      while(Dis_Flag == 0)
      {
        Key_Event();
        Key_Process();
        if(cflag == 0)
        {
          Get_DS1302(times);          //获取时间数据
          ShowTime();
          RH();
          writeaddress(1,2);
          writedata(Dis_Num[T_data_H/10]);
          writedata(Dis_Num[T_data_H%10]);
          writeaddress(1,6);
          writedata(Dis_Num[RH_data_H/10]);
          writedata(Dis_Num[RH_data_H%10]);
          if(T_data_H > T_MAX)
          {
            FAN_ON;
            LED_RED_ON;
            BEEP_ON;
            delay_ms(500);
            BEEP_OFF;
            delay_ms(500);
          }
          if(T_data_H < T_MIN)  
          {
            FAN_OFF;       //关风扇
            LED_RED_OFF;
            if(PENG_Flag)  //关棚
            {
              PENG_OFF;
              delay_ms(10000);
              PENG_STAY; 
              PENG_Flag = 0;
            }
          }
          if(RH_soil < 42)    //自动灌溉
          {
            WATER_ON;
            LED_GREEN_ON;
          }
          if(RH_soil > 80)
          {
            LED_GREEN_OFF;
            WATER_OFF;
          }
        }
      }
    }
    /************************ 第二屏：显示土壤湿度，光照强度 ********************/
    else if(Dis_Flag == 1)
    {
      writecmd(0x01); //清除屏幕
      delay_ms(10);
      Init_BH1750();       //初始化BH1750
      writeaddress(0,0);
      print("    监控系统    ");
      writeaddress(1,0);
      print("土壤湿度：    %");
      writeaddress(2,0);
      print("光照强度：");
      while(Dis_Flag == 1)
      {
        Key_Event();
        if(Key_Flag == 1)  //判断是否有按键被按下
        {
          if(Key_Val == 12)
            Dis_Flag = 0;
            Key_Flag = 0;
        }
        ADC12IE = 0x01;                 //开AD中断
        Multiple_Read_BH1750();       //连续读出数据，存储在BUF中
        
        writeaddress(1,5);            //显示土壤湿度
        writedata(Dis_Num[RH_soil/100]);
        writedata(Dis_Num[RH_soil%100/10]);
        writedata(Dis_Num[RH_soil%10]);
        
       /* writedata(Dis_Num[result/1000]);
        writedata(Dis_Num[result%1000/100]);
        writedata(Dis_Num[result%100/10]);
        writedata(Dis_Num[result%10]);*/
        
        writeaddress(2,5);            //显示光照强度
        writedata(Dis_Num[Light/10000]);
        writedata(Dis_Num[Light%10000/1000]);
        writedata(Dis_Num[Light%1000/100]);
        writedata(Dis_Num[Light%100/10]);
        writedata(Dis_Num[Light%10]);
        delay_ms(100); 
      }
    }   
    /************************ 第三屏：最高、低温度调节 ********************/
    else if(Dis_Flag == 2)
    {
      writecmd(0x01); //清除屏幕
      delay_ms(10);
      writeaddress(0,0);
      print("  大棚温度设置  ");
      writeaddress(1,0);
      print("最高温度：  ℃");
      writeaddress(2,0);
      print("最低温度：  ℃");
      while(Dis_Flag == 2)
      {
        Key_Event();     //扫描键盘
        if(Key_Flag == 1)  //判断是否有按键被按下
        {
          switch(Key_Val)
          {
            case 1:
              if(T_MAX++ > 40) T_MAX = T_MIN; break;   //这里我设置的最高温度不超过40，可以根据你自己的要求更改；
            case 2:
              if(T_MAX-- < T_MIN) T_MAX = 40; break;  //最低温度温度为0度，如果要做的零下 需要把T_MAX的类型换成 char
            case 3:
              if(T_MIN++ > T_MAX) T_MIN = 1; break; //如果最低温度高于最高温度，将最低温度设为1
            case 4:
              if(T_MIN-- == 0) T_MIN = T_MAX; break; //如果最低温度等于0，将最低温度设为最高温度
            case 16:
              Dis_Flag = 0; break;                //按下16号键，自动跳到上一屏
            default:  break;                      //按下其他按键，不做任何操作
          }
          Key_Flag = 0;   //清空按键标志位
        }
        writeaddress(1,5);                    //在12864上显示空调温度地址
        writedata(Dis_Num[T_MAX/10]);         //显示最高温度十位
        writedata(Dis_Num[T_MAX%10]);         //显示最高温度个位
        writeaddress(2,5);                    //在12864上显示空调温度
        writedata(Dis_Num[T_MIN/10]);         //显示最低温度十位
        writedata(Dis_Num[T_MIN%10]);         //显示最低温度个位   A1E6  
      }
    }
  }
}
/*------------------------------------------
P2口中断函数
如果降雨  关棚
------------------------------------------*/
#pragma vector=PORT2_VECTOR
__interrupt void PORT1_ISR(void)
{
  P2IFG=0x00;
  delay_ms(10);
  if(P2IN | BIT0)
  {
    if(PENG_Flag)
    {
      LED_WHITE_ON;
      PENG_OFF;
      delay_ms(10000);
      PENG_STAY; 
      PENG_Flag = 0;
      LED_WHITE_OFF;
    }
  }
}