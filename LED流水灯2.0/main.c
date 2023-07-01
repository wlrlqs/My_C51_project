#include <REGX52.H>
#include <INTRINS.H>

void Delay1ms(unsigned int xms)		//@11.0592MHz
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

void main()
{
	  while(1) 
	{
	  P0=0x01;
		Delay1ms(500);
		P0=0x02;
		Delay1ms(500);
		P0=0x04;
		Delay1ms(500);
		P0=0x08;
		Delay1ms(500);
		P0=0x10;
		Delay1ms(500);
		P0=0x20;
		Delay1ms(500);
		P0=0x40;
		Delay1ms(500);
		P0=0x80;
		Delay1ms(500);
	
		P0=0x40;
		Delay1ms(500);
		P0=0x20;
		Delay1ms(500);
		P0=0x10;
		Delay1ms(500);
		P0=0x08;
		Delay1ms(500);
		P0=0x04;
		Delay1ms(500);
		P0=0x02;
		Delay1ms(500);
	}
	
}