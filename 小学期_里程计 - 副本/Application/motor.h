#ifndef __MOTOR_H__
#define __MOTOR_H__
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
#endif