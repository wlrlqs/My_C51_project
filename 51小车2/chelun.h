#ifndef _CHELUN_H_
#define _CHELUN_H_				//预编译,防止头文件重复定义

void Delay1000ms();//1000ms延时函数
//关于轮子电机
void Left_moto_Go(void);
void Left_moto_Back(void);
void Left_moto_Stop(void);			//左轮子 前进 后退 停止函数
void Right_moto_Go(void);
void Right_moto_Back(void);
void Right_moto_Stop(void);			//右轮子 前进 后退 停止函数

void pwm_out_left_moto(void);   //左边电机调速函数
void pwm_out_right_moto(void);  //右边电机调速函数

void Rotate(); 									//小车原地转圈
void Stop();  									//小车停
void TurnRight(); 							//小车右转
void TurnLeft();   							//小车左转
void GoBack();  								//小车后退
void GoStright();   					  //小车前进
#endif