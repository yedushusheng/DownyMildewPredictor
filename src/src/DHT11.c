#include "DHT11.h"
#include "user.h"

/********************************* 变量定义区 *********************************/
uchar T_data_H,T_data_L,RH_data_H,RH_data_L,checkdata;

/*******************************************************************************
*******************************************************************************/
/*******************************************************************************
函数功能：读取DHT11字节数据
入口参数：无
返回值：读取数据
*******************************************************************************/
uchar COM(void)
{
  uchar i;
  uchar temp,comdata;
  
  for(i=0;i<8;i++)	   
  { 
    //判断数据位是0还是1
    while(!(PIN0&0X01));
    delay_us(10);        
    temp=0;
    delay_us(30);
    if(PIN0&0X01) 
      temp=1;
    while(PIN0&0X01); 	    	   
    // 如果高电平高过预定0高电平值则数据位为 1 	 
    comdata<<=1;
    comdata|=temp;
  } 
  return(comdata);
}
/*******************************************************************************
函数功能：读取温湿度数据
入口参数：无
返回值：无
*******************************************************************************/
void RH(void)
{
  uchar RH_data_H_temp,RH_data_L_temp,T_data_H_temp,T_data_L_temp,checkdata_temp;
  //主机拉低18ms 
  SET_L;
  delay_ms (18);      
  SET_H;
  //总线由上拉电阻拉高 主机延时20us
  delay_us(20);              
  //主机设为输入 判断从机响应信号 
  SET_IN; 
  //while(PIN0&0X01);  
  //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行  
  if(!(PIN0&0X01))
  {
    //判断从机是否发出 80us 的低电平响应信号是否结束 
    delay_us(60);
    while(!(PIN0&0X01));
    //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
    while((PIN0&0X01));
    //数据接收状态		
    RH_data_H_temp = COM();
    RH_data_L_temp = COM();
    T_data_H_temp = COM();
    T_data_L_temp = COM();
    checkdata_temp = COM();
    SET_H;
    //数据校验 
    if(checkdata_temp == (T_data_H_temp+T_data_L_temp+RH_data_H_temp+RH_data_L_temp))
    {
      RH_data_H=RH_data_H_temp;
      RH_data_L=RH_data_L_temp;
      T_data_H=T_data_H_temp;
      T_data_L=T_data_L_temp;
      checkdata=checkdata_temp;
    }
  }
}