#include <REG52.H>
#include "CheLun.h"
#include <intrins.h>
/**********************/
/****相关端口定义******/
/**********************/
sbit ENA = P1^0;
sbit Left_IN1 = P1^1;
sbit Left_IN2 = P1^2;
sbit Right_IN3 =P1^4;
sbit Right_IN4 =P1^3;
sbit ENB = P1^5;
//左右轮PWM当前值 左右轮PWM比较值
unsigned char pwm_val_left;
unsigned char pwm_val_right;
unsigned char push_val_left;
unsigned char push_val_right;

//改变这里的值就是改变左右轮子的占空比,实现调转速
 unsigned char Left_Speed_Ratio =18;  	//左电机转速设定
 unsigned char Right_Speed_Ratio =18 ;  //右电机转速设定

//左右电机标志位
bit Left_moto_flag = 1;
bit Right_moto_flag = 1;
void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
/*左边电机调速函数*/
void pwm_out_left_moto()     
{ 
	if(Left_moto_flag) 
		{ 
			if(pwm_val_left<=push_val_left) 	//当前值和设定的比较值进行比较,来控制使能端口,进而控制车轮
			ENA=1; 
			else 
			ENA=0; 
				if(pwm_val_left>20) 						//PWM当前值自增到20之后变为0,相当于一个周期
				pwm_val_left=0; 
		}		 
	else 
			ENA=0; 
}
/*右边电机调速函数*/
void pwm_out_right_moto()     
{ 
	if(Left_moto_flag) 
		{ 
			if(pwm_val_right<=push_val_right) 
			ENB=1; 
			else 
			ENB=0; 
				if(pwm_val_right>20) 
				pwm_val_right=0; 
		}		 
	else 
			ENB=0; 
}
//左右轮前进后退停止,根据自己小车视情况而测
void Left_moto_Go()   {Left_IN1 = 0,Left_IN2 = 1;}
void Left_moto_Back() {Left_IN1 = 1,Left_IN2 = 0;}
void Left_moto_Stop() {Left_IN1 = 1,Left_IN2 = 1;}
void Right_moto_Go()  {Right_IN3 = 1,Right_IN4 = 0;}
void Right_moto_Back(){Right_IN3 = 0;Right_IN4 = 1;}
void Right_moto_Stop(){Right_IN3 = 1;Right_IN4 = 1;}


void GoStright()     //小车前进
{ 
	/*设置比较值,左右轮子各自前转*/
	push_val_left =Left_Speed_Ratio;    
	push_val_right =Right_Speed_Ratio; 
	Left_moto_Go(); 
	Right_moto_Go(); 
} 

void GoBack()   //小车后退
{ 
	push_val_left =Left_Speed_Ratio; 
	push_val_right =Right_Speed_Ratio; 
} 



void TurnLeft()   //小车左转
{ 

	push_val_right =10;		//把右轮子速度降低
	Right_moto_Go(); 
	Left_moto_Stop();
} 

 void TurnRight() //小车右转
{ 

	push_val_left =10;		//把左轮子速度降低
	Right_moto_Stop();
	Left_moto_Go();
} 

void Stop()  //小车停
{ 
	Left_moto_Stop();
	Right_moto_Stop();
} 

void Rotate() 	//小车原地转圈
{ 
	push_val_left =Left_Speed_Ratio; 
	push_val_right =Right_Speed_Ratio; 
	Left_moto_Back();
	Right_moto_Go();
}
