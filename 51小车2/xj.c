#include <REG52.H>
#include "CheLun.h"
#include "RED.h"
#include "HR_SC.h"
extern unsigned char pwm_val_left;
extern unsigned char pwm_val_right;

void xj()
{
	Init_Timer0();
	while(1)
	{
		xj_Run();	
	}
}
