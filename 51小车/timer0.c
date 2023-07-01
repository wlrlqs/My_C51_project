#include <REGX52.H>
#include "car.h"
void Timer0Init()    //定时器初始化函数
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。

	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
}

extern unsigned char Left_Speed_Ratio;
extern unsigned char Right_Speed_Ratio;
unsigned int time=0; 
extern unsigned char pwm_val_left;
extern unsigned char pwm_val_right;

void timer0()interrupt 1 using 2  //定时器中断函数，此处配置为1ms产生一次中断，对PWM的输出进行控制
{ 
	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;
	time++; 
	pwm_val_left++; 
	pwm_val_right++; 
	pwm_out_left_moto(); 
	pwm_out_right_moto(); 
}