//输出端子接单片机的io口,有遮挡输出0,无遮挡输出1*/
#include <REG52.H>
#include "RED.H"

//检测灯
unsigned LED1=0;
sbit Left_led = P3^7;
sbit Right_led = P3^6;

//感应到黑线 指示灯灭 输出1 没感应到黑线 指示灯亮 输出0
void xj_Run()
{
	//碰到黑线为1,没碰到黑线为0
	if(Left_led == 0 && Right_led==1)	//右边碰到黑线
	{
		TurnLeft();//左转
	}
	if(Left_led == 1 && Right_led==0)//左边碰到黑线
	{
		 TurnRight();//右转
	}
	if(Left_led ==0 && Right_led==0)//没有碰到
	{
		 GoStright();//前进
	}

}
