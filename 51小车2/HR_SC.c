#include <REG52.H>
#include "HR_SC.H"
unsigned char flag;
/*蓝牙控制小车前进后腿左转右转通过按键实现*/
void Init_UART()
{
	TMOD &= 0X00;
	TMOD |= 0X21;	//开启定时器1,采用工作方式2,8位自动重装载
								//开启定时器0,采用工作方式1
	SCON = 0X50;	//串口通信采取工作方式1,REN位打开
	PCON = 0X80;	//波特率加倍
	TH1 = 0XFA;
	TL1 = 0XFA;//9600 蓝牙配置好了也是这个波特率
	ET1 = 0;
	ES = 1;
	EA = 1;		//开启定时器中断
	TR1 = 1;
}

void Init_Timer0()
{
	TH0 = 0XFC;//(65535-1000)/256;
	TL0 = 0X18;(65535-1000)%256;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}
void SendByte(unsigned char temp)	//串口发送数据
{
	SBUF = temp;
	while(TI == 0);
	TI = 0;
}
void HR_SC_INPUT()
{
	flag = SBUF;		//接收手机发来的数据
	while(RI ==0);	//等待接收完成
	RI = 0;
  SendByte(flag);
}
