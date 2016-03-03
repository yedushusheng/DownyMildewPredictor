#include "DHT11.h"
#include "user.h"

/********************************* ���������� *********************************/
uchar T_data_H,T_data_L,RH_data_H,RH_data_L,checkdata;

/*******************************************************************************
*******************************************************************************/
/*******************************************************************************
�������ܣ���ȡDHT11�ֽ�����
��ڲ�������
����ֵ����ȡ����
*******************************************************************************/
uchar COM(void)
{
  uchar i;
  uchar temp,comdata;
  
  for(i=0;i<8;i++)	   
  { 
    //�ж�����λ��0����1
    while(!(PIN0&0X01));
    delay_us(10);        
    temp=0;
    delay_us(30);
    if(PIN0&0X01) 
      temp=1;
    while(PIN0&0X01); 	    	   
    // ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 	 
    comdata<<=1;
    comdata|=temp;
  } 
  return(comdata);
}
/*******************************************************************************
�������ܣ���ȡ��ʪ������
��ڲ�������
����ֵ����
*******************************************************************************/
void RH(void)
{
  uchar RH_data_H_temp,RH_data_L_temp,T_data_H_temp,T_data_L_temp,checkdata_temp;
  //��������18ms 
  SET_L;
  delay_ms (18);      
  SET_H;
  //������������������ ������ʱ20us
  delay_us(20);              
  //������Ϊ���� �жϴӻ���Ӧ�ź� 
  SET_IN; 
  //while(PIN0&0X01);  
  //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������  
  if(!(PIN0&0X01))
  {
    //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ���� 
    delay_us(60);
    while(!(PIN0&0X01));
    //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
    while((PIN0&0X01));
    //���ݽ���״̬		
    RH_data_H_temp = COM();
    RH_data_L_temp = COM();
    T_data_H_temp = COM();
    T_data_L_temp = COM();
    checkdata_temp = COM();
    SET_H;
    //����У�� 
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