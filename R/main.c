/***WS2812��������***/
/*���ߣ�YUAN HUA FEI*/
#include <STC15F2K60S2.H>
#include "UART1.h"
#include "led_task.h"

#define  num 253//���ݸ���

sbit DO = P0^1;//��������ڿ�
sbit key = P0^3;
sbit gnd = P0^0;

void delays(unsigned int m)
{
  unsigned char i,j;
  while(--m)
  {
	  for(i=6;i>0;i--)
	    for(j=250;j>0;j--);
  }
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

	while(1)
	{
		EA=0;
		/*switch(state)
		{
			case 0:
				for(i=0;i<30;i++) WS2812WR(0,250,0);
			  break;
			case 1:
				waiting();
			  break;
			case 2:
	      for(i=0;i<253;i++) 
			  {
	       	if(color==2) WS2812WR(0,0,250);
		      else WS2812WR(250,0,0);
	      }
				break;
			case 3:
				complete();
			  state=0;
		}*/
		
		for(i=0;i<11;i++){
			if(key) WS2812WR(250,0,0);
			else WS2812WR(0,0,250);
		}
		
		delays(100);
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
				WS2812WR(200,0,200);  //�ۺ�

				if(i==j-1)
				WS2812WR(0,200,200);  //����

				if(i==j)
				WS2812WR(200,200,0);  //�Ȼ�

				if(i==j+1)
				WS2812WR(200,200,200);//����
				
				else
				WS2812WR(0x22,0,0);//��ɫ��ɫ
			}		
      			
				delays(100);//����ÿ������һ��
//����Ҫ��ʱһ��ʱ��ģ���ÿ����32bit����
//IO����Ϊ�ͣ���ֻҪ�����ʱ�¾͸�λ��			
		}*/
