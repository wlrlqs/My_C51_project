#include <car.h>

extern unsigned char Left_Speed_Ratio;
extern unsigned char Right_Speed_Ratio;
unsigned int time=0; 
unsigned int HC_SR04_time=0;
extern unsigned char pwm_val_left;
extern unsigned char pwm_val_right;
 bit   flag =0;
unsigned  flag1 =0;
 unsigned   flag2 =0;
 unsigned   k =0;
extern char M_sensor;  
 
int delay1s(int i)   
{
	for(i;i>=0;i--)
	{
    unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
    _nop_();  
	}
}
int delay1ms(int i)   
{
	for(i;i>=0;i--)
	{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
	}
}

void Timer0Init()
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。

	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
}

void Timer1Init()
{
	TMOD|=0X10;//选择为定时器1模式，工作方式1，仅用TR1打开启动。
	TH1=0;	
	TL1=0;	
	ET1=1;//打开定时器1中断允许
	EA=1;//打开总中断
	TR1=0;//打开定时器			
}

void timer0()interrupt 1 using 2 
{ 
TH0=0XFC;	//给定时器赋初值，定时1ms
TL0=0X18;
time++; 
pwm_val_left++; 
pwm_val_right++; 
pwm_out_left_moto(); 
pwm_out_right_moto(); 	
HC_SR04_time++;
if(HC_SR04_time>=100)   //100ms 启动一次超声波测距
{
	
	HC_SR04_time=0;
	StartModule();
}
} 
void Timer1() interrupt 3
{
	flag=1;    //若定时器1溢出则flag置1
}


void follow(void)
{
	while(1)
		{
			if(Echo==1)
				{
				TH1=0;	
	      TL1=0;
				TR1=1;			    //开启计数	
				while(Echo);			//当RX为1计数并等待	
				TR1=0;				//关闭计数	
			  Conut();			//计算		
			}
				
				if(M_sensor==0)
				{
					stop();
	
				}
				else{
					while(M_sensor==1)
					{
						run();
						if(Echo==1)
				{
				TH1=0;	
	      TL1=0;
				TR1=1;			    //开启计数	
				while(Echo);			//当RX为1计数并等待	
				TR1=0;				//关闭计数	
			  Conut();			//计算		
			}
					}
				}			
		}
					
}

void main(){

Timer0Init();
	Timer1Init();	
	Left_Speed_Ratio=4;   //设置左电机车速为最大车速的40%
	Right_Speed_Ratio=4;	//设置右电机车速为最大车速的40%
			run();
		while(1)
		{
			follow();
		}
}
	