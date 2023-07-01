#ifndef __MOTOR_H__
#define __MOTOR_H__
sbit M_1 = P2^3;
sbit M_2 = P2^2;
sbit M_3 = P2^1;
sbit M_4 = P2^0;
void Delay_xms(unsigned int x);
//˳ʱ��ת��
void MotorCW(void);
//��ʱ��ת��
void MotorCCW(void);
//ֹͣת��
void MotorStop(void);
//������
void motor_control(void);
void loops_count(void);
void motor_speed_choose(void);
void motor_speed_fbd(void);
#endif