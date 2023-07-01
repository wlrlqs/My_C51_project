#ifndef __MOTOR_H__
#define __MOTOR_H__
sbit M_1 = P2^3;
sbit M_2 = P2^2;
sbit M_3 = P2^1;
sbit M_4 = P2^0;
void Delay_xms(unsigned int x);
//顺时针转动
void MotorCW(void);
//逆时针转动
void MotorCCW(void);
//停止转动
void MotorStop(void);
//主函数
void motor_control(void);
void loops_count(void);
void motor_speed_choose(void);
void motor_speed_fbd(void);
#endif