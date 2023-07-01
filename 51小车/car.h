#ifndef __car_H_
#define __car_H_

#include <REGX52.H>
#include <intrins.h>

sbit Left_moto_pwm=P1^6 ;
sbit Right_moto_pwm=P1^7;
sbit p34=P1^1;
sbit p35=P1^2; 
sbit p36=P1^3;
sbit p37=P1^4;
sbit Trig= P2^1; //产生脉冲引脚
sbit Echo= P2^0; //回波引脚
sbit LED=P0^0;
sbit BOTTOM_L_led=P3^6;
sbit BOTTOM_R_led=P3^7;
sbit R_led=P3^3;
sbit L_led=P3^4;
sbit R_sensor=P3^3;
sbit L_sensor=P3^2;
void Left_moto_go();
void Left_moto_back();
void Left_moto_stp();
void Right_moto_go();
void Right_moto_back(); 
void Right_moto_stp(); 
void delay(unsigned int k);
int delay1s(int i);
int delay1ms(int i);
void pwm_out_left_moto(void);
void pwm_out_right_moto(void);
void run(void);
void back(void);
void left(void);
void right(void);
void stop(void);
void rotate(void);
void  StartModule();
void Timer1Init();
void Timer0Init();
void Conut(void);
void steer(void);
void follow_the_trail(void);
void ConfigUART(unsigned int baud);

#endif