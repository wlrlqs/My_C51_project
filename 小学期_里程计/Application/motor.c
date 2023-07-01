/********************
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
sbit motor_scan = P2^7;
#define weight 2//遮挡长度
float timer = 0;
//引用外部变量
extern float w_speed,w_distance,ws_distance;
extern unsigned char led_speed[4];
extern int speed_choose;
//内部变量定义
unsigned int speed_delay = 0,motor_task_loop = 0,scan_last = 0,cw_ccw_flag = 0;
float speed_count_delay = 0;
uchar phasecw[4] ={0xF1,0xF2,0xF4,0xF8};//正转 电机导通相序 D-C-B-A
uchar phaseccw[4]={0xF8,0xF4,0xF2,0xF1};//反转 电机导通相序 A-B-C-D

/****************************************************
函 数 名：Delay_xms
功		能：软件延时
入口参数：uint x
返 回 值：无
应用范围：延时
备		注：
*****************************************************/
void Delay_xms(uint x){
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}

/****************************************************
函 数 名：MotorCW
功		能：步进电机顺时针转动
入口参数：无
返 回 值：无
应用范围：电机控制
备		注：
*****************************************************/
void MotorCW(void){
 uchar i;
 for(i=0;i<4;i++){
	 loops_count();
	 MotorData=phasecw[i];
	 os_wait2(K_TMO,speed_delay);
  }
}

/****************************************************
函 数 名：MotorCCW
功		能：步进电机逆时针转动
入口参数：无
返 回 值：无
应用范围：电机控制
备		注：
*****************************************************/
void MotorCCW(void){
 uchar i;
 for(i=0;i<4;i++){
	 loops_count();
   MotorData=phaseccw[i];
	 os_wait2(K_TMO,speed_delay);
  }
}

/****************************************************
函 数 名：MotorStop
功		能：步进电机停止转动
入口参数：无
返 回 值：无
应用范围：电机控制
备		注：
*****************************************************/
void MotorStop(void){
 MotorData=MotorData=(MotorData & 0xF0);
}
/****************************************************
函 数 名：loops_count
功		能：电机控制loop计算
入口参数：无
返 回 值：无
应用范围：用于led显示电机相位状态，取1 / 13分频
备		注：
*****************************************************/
void loops_count(void){
	motor_speed_fbd();
	motor_task_loop++;
	if(motor_task_loop == 1000){
		motor_task_loop = 0; 
	}
	 if(motor_task_loop % 13 == 0){
		LED = P2 & led_speed[speed_choose];
	 }
}

/****************************************************
函 数 名：motor_speed_fbd
功		能：电机速度检测
入口参数：无
返 回 值：无
应用范围：电机速度检测
备		注：
*****************************************************/

void motor_speed_fbd(void){//timer = 9ms * 0.65 = 0.00675s
	w_distance += w_speed * speed_count_delay;
	if(motor_scan){
		timer += speed_count_delay;
		scan_last = 1;
	}else if(!motor_scan && scan_last){
		w_speed = weight / timer;
		scan_last = 0;
		timer = 0;
	}
}

/****************************************************
函 数 名：motor_speed_choose
功		能：电机速度选择
入口参数：无
返 回 值：无
应用范围：电机速度控制
备		注：
*****************************************************/
void motor_speed_choose(void){
	switch (speed_choose){
				case 0:
					speed_delay = 8;
					speed_count_delay = 0.009;
					//w_speed = 4.16;
					break;
				case 1:
					speed_delay = 4;
					speed_count_delay = 0.0054;
					//w_speed = 12.5;
					break;
				case 2:
					speed_delay = 2;
					speed_count_delay = 0.00324;
					//w_speed = 19.44;
					break;
				case 3:
					speed_delay = 1;
					speed_count_delay = 0.00310;
					//w_speed = 33.33;
					break;
	}
}

/****************************************************
函 数 名：motor_control
功		能：电机控制主函数
入口参数：无
返 回 值：无
应用范围：电机控制任务
备		注：
*****************************************************/
void motor_control(void){
	if(!cw_ccw_flag){
		MotorCW();   //顺时针转动
	}else {
		MotorCCW();   //逆时针转动
	} 
}