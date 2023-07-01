#include <car.h>


unsigned char pwm_val_left =0;
unsigned char push_val_left =0; 
unsigned char pwm_val_right =0;
unsigned char push_val_right=0;
unsigned char Left_Speed_Ratio;
unsigned char Right_Speed_Ratio;

bit Left_moto_stop =1;
bit Right_moto_stop =1;


void Left_moto_go()  //左电机正转
{p34=0;p35=1;} 
void Left_moto_back() //左电机反转
{p34=1;p35=0;} 
void Left_moto_stp()  //左电机停转
 {p34=1;p35=1;} 
void Right_moto_go()  //右电机正转
{p36=0;p37=1;} 
void Right_moto_back() //右电机反转
{p36=1;p37=0;}  
void Right_moto_stp()  //右电机停转
{p36=1;p37=1;} 


void pwm_out_left_moto(void)    //左电机PWM
{ 
if(Left_moto_stop) 
{ 
if(pwm_val_left<=push_val_left) 
Left_moto_pwm=1; 
else 
Left_moto_pwm=0;
if(pwm_val_left>=10)  
pwm_val_left=0; 
} 
else 
Left_moto_pwm=0; 
} 

void pwm_out_right_moto(void)    //右电机PWM
{ 
if(Right_moto_stop) 
{ 
if(pwm_val_right<=push_val_right) 
Right_moto_pwm=1; 
else 
Right_moto_pwm=0; 
if(pwm_val_right>=10) 
pwm_val_right=0; 
} 
else 
Right_moto_pwm=0; 
} 


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
Left_moto_back();
} 

 void right(void) //小车右转
{ 
push_val_left =Left_Speed_Ratio;
push_val_right =Right_Speed_Ratio;
Right_moto_back();
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
