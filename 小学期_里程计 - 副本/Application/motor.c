/*******************************************************
接线方式:
IN1 ---- P23
IN2 ---- P22
IN3 ---- P21
IN4 ---- P20
*********************/
#include <reg52.h>
#include "motor.h"
#include <rtx51tny.h>
#define uchar unsigned char
#define uint  unsigned int
#define MotorData P2                    //步进电机控制接口定义
#define LED P1
sbit M_1 = P2^3;
sbit M_2 = P2^2;
sbit M_3 = P2^1;
sbit M_4 = P2^0;
extern unsigned char led_speed[4];
unsigned int speed_delay = 0,motor_task_loop = 0;
extern int speed_choose;
uchar phasecw[4] ={0xF8,0xF4,0xF2,0xF1};//正转 电机导通相序 D-C-B-A
uchar phaseccw[4]={0xF1,0xF2,0xF4,0xF8};//反转 电机导通相序 A-B-C-D
//ms延时函数
void Delay_xms(uint x){
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
//顺时针转动
void MotorCW(void){
 uchar i;
 for(i=0;i<4;i++){
	 loops_count();
	 MotorData=phasecw[i];
	 os_wait2(K_TMO,speed_delay);
  }
}
//逆时针转动
void MotorCCW(void){
 uchar i;
 for(i=0;i<4;i++){
	 loops_count();
   MotorData=phaseccw[i];
	 os_wait2(K_TMO,speed_delay);
  }
}
//停止转动
void MotorStop(void){
 MotorData=MotorData=(MotorData & 0xF0);
}
void loops_count(void){
	motor_task_loop++;
	if(motor_task_loop == 1000){
		motor_task_loop = 0;
	}
	 if(motor_task_loop % 31 == 0){
		LED = P2 & led_speed[speed_choose];
	 }
}
void motor_speed_choose(void){
	switch (speed_choose){
				case 0:
					speed_delay = 8;
					break;
				case 1:
					speed_delay = 4;
					break;
				case 2:
					speed_delay = 2;
					break;
				case 3:
					speed_delay = 1;
					break;
	}
}
//主函数
void motor_control(void){
  MotorCW();   //顺时针转动 
}