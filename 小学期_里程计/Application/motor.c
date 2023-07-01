/********************
���߷�ʽ:
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
#define MotorData P2                    //����������ƽӿڶ���
#define LED P1
sbit motor_scan = P2^7;
#define weight 2//�ڵ�����
float timer = 0;
//�����ⲿ����
extern float w_speed,w_distance,ws_distance;
extern unsigned char led_speed[4];
extern int speed_choose;
//�ڲ���������
unsigned int speed_delay = 0,motor_task_loop = 0,scan_last = 0,cw_ccw_flag = 0;
float speed_count_delay = 0;
uchar phasecw[4] ={0xF1,0xF2,0xF4,0xF8};//��ת �����ͨ���� D-C-B-A
uchar phaseccw[4]={0xF8,0xF4,0xF2,0xF1};//��ת �����ͨ���� A-B-C-D

/****************************************************
�� �� ����Delay_xms
��		�ܣ������ʱ
��ڲ�����uint x
�� �� ֵ����
Ӧ�÷�Χ����ʱ
��		ע��
*****************************************************/
void Delay_xms(uint x){
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}

/****************************************************
�� �� ����MotorCW
��		�ܣ��������˳ʱ��ת��
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ���������
��		ע��
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
�� �� ����MotorCCW
��		�ܣ����������ʱ��ת��
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ���������
��		ע��
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
�� �� ����MotorStop
��		�ܣ��������ֹͣת��
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ���������
��		ע��
*****************************************************/
void MotorStop(void){
 MotorData=MotorData=(MotorData & 0xF0);
}
/****************************************************
�� �� ����loops_count
��		�ܣ��������loop����
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ������led��ʾ�����λ״̬��ȡ1 / 13��Ƶ
��		ע��
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
�� �� ����motor_speed_fbd
��		�ܣ�����ٶȼ��
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ������ٶȼ��
��		ע��
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
�� �� ����motor_speed_choose
��		�ܣ�����ٶ�ѡ��
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ������ٶȿ���
��		ע��
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
�� �� ����motor_control
��		�ܣ��������������
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ�������������
��		ע��
*****************************************************/
void motor_control(void){
	if(!cw_ccw_flag){
		MotorCW();   //˳ʱ��ת��
	}else {
		MotorCCW();   //��ʱ��ת��
	} 
}