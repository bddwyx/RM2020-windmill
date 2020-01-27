#ifndef _LED_c
#define _LED_c

#endif

#include <STC15F2K60S2.H>
#include <intrins.h>
#include "UART1.h"
#include "led_task.h"
#define Fosc 11059200

sbit DO = P0^2;//��������ڿ�

void delay1us()
{
    unsigned char a;
    for(a=3;a>0;a--);
}

/**Ŀǰ����STC15ϵ��9��30MHz���ó�**/
/**�ʵ�����_NOP_(),���8MHzҲ���ó�***/
/*����Re��Gr��Bl�ı����ɴ����������ɫ*/
/********0��255,��256������ *********/
/*****�������ִ�С�ɵ���ɫ��������*****/
//�����ڷֱ�����졢�̡���������
void WS2812WR(unsigned char Re,unsigned char Gr,unsigned char Bl)
{
	unsigned char color=3,dat,i=8;
	DO=0;
	//�������24bit GRB����
	while(color--)
	{
	  if(color==2)dat=Gr;
	  if(color==1)dat=Re;
	  if(color==0)dat=Bl;
		while(i--)
		{
			if(dat & 0X80)//д1
			{
				DO=1;delay1us();
				DO=0;_nop_();
			}
			else          //д0
			{
				DO=1;_nop_();
				DO=0;delay1us();
			}
			  dat <<= 1;
		}
        i=8;
  }
}

unsigned char code arr[3][5]={{2,0,1,0,2},
                     {0,2,2,2,0},
										 {1,1,0,1,1}};
unsigned char color = 1,state = 1;

void array()
{
/*	uchar i;
	for(i=0;i<125;i++) WS2812WR(0,100,0);*/
	
	static unsigned char cnt=0;
	static unsigned char times=0;
	static unsigned char i,j;
	for(i=0;i<5;i++)
	{
		for(j=0;j<25;j++)
		{
			if(cnt%3==1)
			{
				if(j%3==arr[cnt/3][i])
				{
		      if(color==2) WS2812WR(0,0,250);
	      	else WS2812WR(250,0,0);
      	}
		    else WS2812WR(0,0,0);
			}
			else if(cnt%3==0)
			{
				if((j%3==arr[cnt/3][i])||(j%3==arr[(cnt/3>0)?(cnt/3-1):(cnt/3+2)][i])) 
				{
		      if(color==2) WS2812WR(0,0,100);
	      	else WS2812WR(100,0,0);
	      }
		    else WS2812WR(0,0,0);
			}
			else
			{
				if((j%3==arr[cnt/3][i])||(j%3==arr[(cnt/3<2)?(cnt/3+1):(cnt/3-2)][i])) 
				{
		      if(color==2) WS2812WR(0,0,100);
	      	else WS2812WR(100,0,0);
	      }
		    else WS2812WR(0,0,0);
			}
		}
	}
	times++;
	if(times>=1)
	{
		times=0;
		cnt++;
		if(cnt>8) cnt=0;
	}
}

void complete()
{
	unsigned char i,j;
	for(j=0;j<5;j++)
	{
    for(i=0;i<253;i++)
    {
		  if(color==2) WS2812WR(0,0,250);
		  else WS2812WR(250,0,0);
	  }
	  delays(249);
		for(i=0;i<253;i++) WS2812WR(0,0,0);
		delays(249);
	}
}

void waiting()
{
	unsigned char i;

	for(i=0;i<32;i++) WS2812WR(0,0,0);
	for(i=32;i<67;i++) 
	{
		if(color==2) WS2812WR(0,0,250);
		else WS2812WR(250,0,0);
	}
	for(i=67;i<97;i++) WS2812WR(0,0,0);
	array();
	for(i=222;i<253;i++)
	{
		if(color==2) WS2812WR(0,0,250);
		else WS2812WR(250,0,0);
	}
}