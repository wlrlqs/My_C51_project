#include "car.h"

void steer()
{
	
	if(L_led==1&&R_led==1)
	{
		run();
	}
	else
	{
		if(L_led==0&&R_led==0)
		{
			back();
		}
		else
		{
			if(L_led==1&&R_led==0)
			{
				left();
			}
			else
			{
				if(L_led==0&&R_led==1)
				{
					right();
				}
			}
		}
	}
	
}