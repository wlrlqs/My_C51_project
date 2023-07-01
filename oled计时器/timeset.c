
#include "reg52.h"
//   0 0:0 0:0 0
uint a,b,c,d,e,f=0;
void jishi()
{
	f++;
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
				if((c=6)
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

switch(f)
{
	case 0:
		OLED_P8x16Str(86,2,"0");
	case 1:
		OLED_P8x16Str(86,2,"1");
	case 2:
		OLED_P8x16Str(86,2,"2");
	case 3:
		OLED_P8x16Str(86,2,"3");
	case 4:
		OLED_P8x16Str(86,2,"4");
	case 5:
		OLED_P8x16Str(86,2,"5");
	default:
		break;
}
switch(e)
{
	case 0:
		OLED_P8x16Str(79,2,"0");
	case 1:
		OLED_P8x16Str(79,2,"1");
	case 2:
		OLED_P8x16Str(79,2,"2");
	case 3:
		OLED_P8x16Str(79,2,"3");
	case 4:
		OLED_P8x16Str(79,2,"4");
	case 5:
		OLED_P8x16Str(79,2,"5");
	default:
		break;
}
switch(d)
{
	case 0:
		OLED_P8x16Str(65,2,"0");
	case 1:
		OLED_P8x16Str(65,2,"1");
	case 2:
		OLED_P8x16Str(65,2,"2");
	case 3:
		OLED_P8x16Str(65,2,"3");
	case 4:
		OLED_P8x16Str(65,2,"4");
	case 5:
		OLED_P8x16Str(65,2,"5");
	default:
		break;
}
switch(c)
{
	case 0:
		OLED_P8x16Str(55,2,"0");
	case 1:
		OLED_P8x16Str(55,2,"1");
	case 2:
		OLED_P8x16Str(55,2,"2");
	case 3:
		OLED_P8x16Str(55,2,"3");
	case 4:
		OLED_P8x16Str(55,2,"4");
	case 5:
		OLED_P8x16Str(55,2,"5");
	default:
		break;
}
switch(b)
{
	case 0:
		OLED_P8x16Str(41,2,"0");
	case 1:
		OLED_P8x16Str(41,2,"1");
	case 2:
		OLED_P8x16Str(41,2,"2");
	case 3:
		OLED_P8x16Str(41,2,"3");
	case 4:
		OLED_P8x16Str(41,2,"4");
	case 5:
		OLED_P8x16Str(41,2,"5");
	default:
		break;
}
switch(a)
{
	case 0:
		OLED_P8x16Str(33,2,"0");
	case 1:
		OLED_P8x16Str(33,2,"1");
	case 2:
		OLED_P8x16Str(33,2,"2");
	case 3:
		OLED_P8x16Str(33,2,"3");
	case 4:
		OLED_P8x16Str(33,2,"4");
	case 5:
		OLED_P8x16Str(33,2,"5");
	default:
		break;
}
}

