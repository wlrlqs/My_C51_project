#include <REGX52.H>
sbit moto=P1^0;
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
	moto=0;
	while(1)
	{
		Delay(50);
		if(P3_1==0)
		{
			moto=~moto;
			while(P3_1==0)
			{
				Delay(20);
			}
		}
			if(P3_2==0)
		{
			while(P3_2==0)
			{
				Delay(20);
			}
			while(P3_2==1)
			{
				Delay(100);
				moto=~moto;
				if(P3_2==0)
				{break;}
			}
			moto=0;
			while(P3_2==0)
			{
				Delay(20);
			}
		}
		
		
	}
}

