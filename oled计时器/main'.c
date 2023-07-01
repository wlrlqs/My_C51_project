#include "intrins.h"
#include "codetab.h"
#include "LQ12864.h"
#include "shijian.h"
#define uint unsigned int
#define uchar unsigned char

void Delayms(uint xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms)
	{
	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
	xms--;
	}
}

//   0 0:0 0:0 0
int a=0,b=0,c=0,d=0,e=0,f=0;
void shijian()
{
	f=f+2;
	if(f=10)
	{
		f=0;
		e++;
		if(e=6)
		{
			e=0;
			d++;
			if(d=10)
			{
				d=0;
				c++;
				if(c=6)
				{
					c=0;
					b++;
					if(b=6)
					{
						b=0;
						a++;
						if(a=24)
						{
							a=0;
						}
					}
				}
			}
		}
	}

}



void Timer0_Init()//Initialization using timer 0 only
{
	//1ms
	TMOD=0x01;//Set the mode of timer 0
	TF0=0;//Interrupt flag,The end of the timer becomes 1 and an interrupt is requested
	TR0=1;//Allowed to count
	TH0=64535/256;//High eight digit
	TL0=64535%256;//Low eight digit
	ET0=1;//Open the interrupt
	EA=1;//Interrupt master switch
	PT0=0;//Priority setting 1=High 0=Low
}

void main()
{
	Delayms(100);
	OLED_Init(); //oeld初始化
	Delayms(100);
	Timer0_Init();
	OLED_P8x16Str(29,0,"LQS'Clock");
	OLED_P8x16Str(33,2,"  :  :  ");
	while(1)
	{

	}

}

uint T0count=0;
void T0_interrput() interrupt 1
{
	T0count++;
	TH0=64535/256;//High eight digit
	TL0=64535%256;//Low eight digit
	if(T0count=2000)
	{
		shijian();
		T0count=0;
		
	}
}