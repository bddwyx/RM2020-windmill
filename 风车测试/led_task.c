#ifndef _LED_c
#define _LED_c

#endif

#include <STC15F2K60S2.H>
#include <intrins.h>
#include "UART1.h"
#include "led_task.h"
#define Fosc 11059200

sbit DO = P0^2;//数据输出口口

void delay1us()
{
    unsigned char a;
    for(a=3;a>0;a--);
}

/**目前测试STC15系列9～30MHz整得成**/
/**适当调整_NOP_(),最低8MHz也整得成***/
/*调整Re、Gr、Bl的比例可搭配出各种颜色*/
/********0～255,共256级亮度 *********/
/*****调整数字大小可调整色调及亮度*****/
//括号内分别代表（红、绿、蓝）数据
void WS2812WR(unsigned char Re,unsigned char Gr,unsigned char Bl)
{
	unsigned char color=3,dat,i=8;
	DO=0;
	//依次输出24bit GRB数据
	while(color--)
	{
	  if(color==2)dat=Gr;
	  if(color==1)dat=Re;
	  if(color==0)dat=Bl;
		while(i--)
		{
			if(dat & 0X80)//写1
			{
				DO=1;delay1us();
				DO=0;_nop_();
			}
			else          //写0
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