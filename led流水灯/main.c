#include <REGX52.H>
#include <INTRINS.H>
#define LED P2//定义P2端口为LED
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
	int i=0;
	LED=~0x01;
	while(1)
	{
		for(i=7;i>0;i--)
		{
			LED=_crol_(LED,1);
			Delay(100);
		}
		for(i=7;i>0;i--)
		{
			LED=_cror_(LED,1);
			Delay(100);
		}
		
	}
}