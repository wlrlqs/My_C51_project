#include "reg52.h"

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
#define SMG P0
unsigned char num_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

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

void smg_switch(int x)//位选到第x个数码管
{
	x=8-x;
	LSA=x%2;
	x=x/2;
	LSB=x%2;
	x=x/2;
	LSC=x%2;
}

void main()
{
	int i;
	while(1)
	{
		for(i=1;i<9;i++)
		{
			smg_switch(i);
			SMG=num_code[i-1];
			Delay(200);
			SMG=0x00;
		}
	}
}