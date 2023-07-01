#include <REGX52.H>
#include "DELAY.H"
void main()
{
	P0=~P0;
	while(1)	
	{
		int x=6;
		while(x)
		{
			Delay(30);
			P0=~P0;
			x--;
		}

		Delay(500);
	}

}