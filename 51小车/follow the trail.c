#include "car.h"

//sbit BOTTOM_L_led=P3^6;
//sbit BOTTOM_R_led=P3^7;

void follow_the_trail()
	
{

	if(BOTTOM_L_led==0&&BOTTOM_R_led==0)
	{
		run();
	}
	else
	{
		if(BOTTOM_L_led==0&&BOTTOM_R_led==1)
		{
			left();
		}
		else
		{
			if(BOTTOM_L_led==1&&BOTTOM_R_led==0)
			{
				right();
			}
			else
			{
				if(BOTTOM_L_led==1&&BOTTOM_R_led==1)
				{
					back();
				}
			}
		}
	}

}