/*蓝牙控制小车*/
#include <REG52.H>

#include "HR_SC.H"
#include "RED.H"
#include "CHELUN.H"
#include "xj.h"
#include "hcsr.h"


unsigned int HC_SR04_time=0;
extern unsigned char Left_Speed_Ratio;  	//电机转速设定
extern unsigned char Right_Speed_Ratio;
extern unsigned char pwm_val_left;
extern unsigned char pwm_val_right;
extern unsigned int flag1;
extern char M_sensor;
extern unsigned int flag1;
void Timer2(void);
 

void Add_Speed_Ratio()	//调速函数
{
		Left_Speed_Ratio++;
		Right_Speed_Ratio++;
}
void Remine_Speed_Ratio()	//调速函数
{
		Left_Speed_Ratio--;
		Right_Speed_Ratio--;
}


int main(void)
{
	Init_UART();
	Init_Timer0();
	Timer2();
	while(1)
		{			
			if(Echo==1)
				{
				TH2=0XFC;	
	      TL2=0X66;
				TR2=1;			    //开启计数	
				while(Echo);			//当RX为1计数并等待	
				TR2=0;				//关闭计数	
			  Conut();			//计算		
			}
				
				if(M_sensor==1)
				{
					GoStright();

				}
				else
				{
					stop();
				}

	}
}
void UART_ser() interrupt 4
{
	HR_SC_INPUT();
}
void Timer0_ser () interrupt 1
{
	TH0 = 0XFC;//(65535-1000)/256;
	TL0 = 0X18;//(65535-1000)%256;

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
	

//		switch (flag)	//蓝牙发送按键测试
//		{
//		case 'G':P2 = 0XFE;break;//前进
//		case 'K':P2 = 0XFD;break;//后退
//		case 'H':P2 = 0XFB;break;//左转
//		case 'J':P2 = 0XF7;break;//右转
//		case 'I':P2 = 0XEF;break;//停止

void Timer2()		
{
	//T2MOD = 0;	//初始化模式寄存器
	T2CON = 0;	//初始化控制寄存器
	TL2 = 0;	//设置定时初值
	TH2 = 0;	//设置定时初值
	ET2=1;
	EA=1;
	PT2=0;
    TR2 = 0;		//定时器2开始计时
}
//中断服务函数模板
void Timer2_Routine() interrupt 5
{
	
    /*TF2 = 0;           //中断标志复位
    RCAP2L  = 0x66;	   //设置低位定时初值
    RCAP2H = 0xFC	;  //设置高位定时初值*/
		flag1=1;
    
}
