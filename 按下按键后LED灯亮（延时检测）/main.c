#include <REGX52.H>

void Delay(unsigned int xms)		//@11.0592MHz
{
	unsigned char i, j;

	while(xms)
	{
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
	P0=0x00;
	while(1)
	{
			if(P1_0==0||P1_1==0||P1_2==0||P1_3==0)
		{
			Delay(20);
			
			Delay(20);
			P0=~P0;
			while(P1_0==0||P1_1==0||P1_2==0||P1_3==0);
			Delay(20);
			P0=~P0;
		}
	}
	
}

