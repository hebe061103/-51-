#ifndef _1DS18B20_H_
#define _1DS18B20_H_
#include "STC12C5A.h"

sbit DQ1=P0^1;

/*****************************1us��ʱ����******************************/
void delay_us1(unsigned char a)//12MHzʱ��ʱ��1΢����ʱ����
{
while (a--); 
}

/*****************************1ms��ʱ����******************************/
void delay_ms1(unsigned int a)//12MHzʱ��ʱ��1������ʱ����
{
unsigned int b;
while(a--)
{
  for(b=0;b<600;b++);
}
}

/******************************��λ����*******************************/
void DS18B201_Reset()
{
CY = 1;
while (CY)
{
  DQ1=0;//�ͳ��͵�ƽ��λ�ź�
  delay_us1(255);//��ʱ510us
  delay_us1(255);
  DQ1=1;//�ͷ�������
  delay_us1(60);//�ȴ�60us
  CY=DQ1;//����������
  delay_us1(200);//�ȴ��豸�ͷ�������
  delay_us1(150);
}
}

/******************************���ֽں���*******************************/
unsigned char DS18B201_ReadByte()
{
unsigned char i;
unsigned char dat = 0;
for(i=0;i<8;i++)//8λ������
  {
   dat>>=1;
   DQ1=0;//��ʼʱ��Ƭ
   delay_us1(1);//��ʱ�ȴ�
   DQ1=1;//׼������
   delay_us1(2);//������ʱ
   if (DQ1)
    dat|= 0x80;//��ȡ����
   delay_us1(42);//�ȴ�ʱ��Ƭ����
  }
return dat;
}

/****************************д�ֽں���*********************************/
void DS18B201_WriteByte(unsigned char dat)
{
unsigned char i;
for(i=0;i<8;i++)//8λ������
  {
   DQ1=0;//��ʼʱ��Ƭ
   delay_us1(12);//��ʱ�ȴ�
   dat>>=1;//�ͳ�����
   DQ1=CY;
   delay_us1(35);//�ȴ�ʱ��Ƭ����
   DQ1=1;//�ָ�������
   delay_us1(0);//�ָ���ʱ
  }
}
/*******************************��ȡ�¶Ⱥ���******************************/
int get_temper1()
{
unsigned int temp;
unsigned int wendu;//�¶�
unsigned char TPH;//����¶�ֵ�ĸ��ֽ�
unsigned char TPL;//����¶�ֵ�ĵ��ֽ�
DS18B201_Reset();//�豸��λ
DS18B201_WriteByte(0xCC);//����ROM����
DS18B201_WriteByte(0x44);//��ʼת������
delay_ms1(800);//�ȴ�800ms
DS18B201_Reset();//�豸��λ
DS18B201_WriteByte(0xCC);//����ROM����
DS18B201_WriteByte(0xBE);//���ݴ�洢������
TPL=DS18B201_ReadByte();//���¶ȵ��ֽ�
TPH=DS18B201_ReadByte();//���¶ȸ��ֽ�    
temp=(TPH<<8)|TPL;    //�ϲ�
if(TPH>200)//����Ǹ��¶� 
  temp=~temp+1;//ȡ��,��һ
wendu=temp*0.625;//ת����ʮ����
if(TPH>200)//����Ǹ��¶�
  wendu=-wendu;//ת���ɸ���
return(wendu);//�����¶�
}

#endif