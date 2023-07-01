#include <REGX52.H>
#include "car.h"
unsigned char Left_Speed_Ratio;  //左电机转速的设定值
unsigned char Right_Speed_Ratio; //右电机转速的设定值

extern unsigned char Left_Speed_Ratio;
extern unsigned char Right_Speed_Ratio;
extern unsigned char pwm_val_left;
extern unsigned char pwm_val_right;

void run(void)     //小车前行
{ 
	push_val_left =Left_Speed_Ratio;    
	push_val_right =Right_Speed_Ratio; 
	Left_moto_go(); 
	Right_moto_go(); 
} 

 

void back(void)   //小车后退
{ 
	push_val_left =Left_Speed_Ratio; 
	push_val_right =Right_Speed_Ratio; 
	Left_moto_back();
	Right_moto_back();
} 



void left(void)   //小车左转
{ 
	push_val_left =Left_Speed_Ratio; 
	push_val_right =Right_Speed_Ratio;
	Right_moto_go(); 
	Left_moto_stp();
} 

 void right(void) //小车右转
{ 
	push_val_left =Left_Speed_Ratio;
	push_val_right =Right_Speed_Ratio;
	Right_moto_stp();
	Left_moto_go();
} 

void stop(void)  //小车停止
{ 
	push_val_left =Left_Speed_Ratio; 
	push_val_right =Right_Speed_Ratio; 
	Left_moto_stp();
	Right_moto_stp();
} 

void rotate(void) //小车原地转圈
{ 
	push_val_left =Left_Speed_Ratio; 
	push_val_right =Right_Speed_Ratio; 
	Left_moto_back();
	Right_moto_go();
} 
