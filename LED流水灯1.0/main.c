#include <REGX52.H>
#include <INTRINS.H>

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}


void main()
{
	while(1)
	{
		P0=0x01;
		Delay100ms();
		P0=0x02;
		Delay100ms();
		P0=0x04;
		Delay100ms();
		P0=0x08;
		Delay100ms();
		P0=0x10;
		Delay100ms();
		P0=0x20;
		Delay100ms();
		P0=0x40;
		Delay100ms();
		P0=0x80;
		Delay100ms();
	
		P0=0x40;
		Delay100ms();
		P0=0x20;
		Delay100ms();
		P0=0x10;
		Delay100ms();
		P0=0x08;
		Delay100ms();
		P0=0x04;
		Delay100ms();
		P0=0x02;
		Delay100ms();
	}
}