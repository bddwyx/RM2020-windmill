/***WS2812驱动程序***/
/*作者：YUAN HUA FEI*/
#include <STC8.H>
#include <intrins.h>
#include "UART1.h"
#include "led_task.h"

#define  num 253//灯泡个数

sbit DO = P1^6;//数据输出
sbit key = P1^7;
sbit gnd = P0^0;

void delays(unsigned int m)
{
	do{
		unsigned char i, j;

		_nop_();
		_nop_();
		i = 15;
		j = 90;
		do
		{
			while (--j);
		} while (--i);
	} while (--m);
}

void main()
{
	unsigned char i;
	
	P0M0=0XFF;P0M1=0;
	gnd=0;key=1;
	
	TMOD=0X00;
	TH0 = 0XF8;
	TL0 = 0XF0;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
	
	uart_config(115200);
	SBUF = 'A';

	while(1)
	{
		EA=0;
		switch(state)
		{
			case 0:
				for(i=0;i<253;i++) WS2812WR(0,0,0);
				break;
			case 1:
				waiting();
				break;
			case 2:
				for(i=0;i<253;i++) {
					if(color == 3) WS2812WR(color_buf[0], color_buf[1], color_buf[2]);
					else if(color==2) WS2812WR(0,0,250);
					else WS2812WR(250,0,0);
				}
				break;
			case 3:
				complete();
				state=0;
				break;
		}
		
		delays(50);
		EA=1;
	}
}
		
void inter_timer_0() interrupt 1
{
	static unsigned char buf=0,stable=0,scnt=0;
	
	stable<<=1;
	buf<<=1;
	if(stable == 0)
	{
		if(scnt == 0) buf|=~key;
		else 
		{
			scnt--;
//			stable = 0xff;
		}
	}
	
	if(buf&0x01 == 0x01) 
	{
	//	uart_write("1");
		SBUF = '1';
		state = 2;
		stable = 0xff;
		buf = 0;
		scnt = 20;
	}
}
		
		
		
		
/*		for(j=0;j<num;j++)
		{
			for(i=0;i<num;i++)
			{
				if(i==j-2)
				WS2812WR(200,0,200);  //粉红

				if(i==j-1)
				WS2812WR(0,200,200);  //青兰

				if(i==j)
				WS2812WR(200,200,0);  //橙黄

				if(i==j+1)
				WS2812WR(200,200,200);//正白
				
				else
				WS2812WR(0x22,0,0);//蓝色底色
			}		
      			
				delays(100);//反正每次整完一组
//都是要延时一段时间的，而每发完32bit数据
//IO总是为低，那只要随便延时下就复位了			
		}*/
