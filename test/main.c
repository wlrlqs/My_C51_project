#include <reg52.h>
void UartInit()		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;			//设定定时初值
	TH1 = 0xFD;			//设定定时器重装值		
	TR1 = 1;			//启动定时器1
	ET1 = 0;        	//禁止定时器1中断
	EA=1;				//开放总中断 
	ES=1;				//开放串口中断
}
void Uartsend(unsigned char byte)//发送
{
	SBUF=byte;//把数据写入发送缓冲区SBUF
	//数据发送完成的标志是TI=1；所以等待数据传送完
	while(TI==0);
	TI=0;//软件清零
}
void UART_ISR() interrupt 4//串口中断函数
{
	if(RI==1)//接收中断
	{
		Uartsend(SBUF);//把接收的数据发送到计算机
		RI=0;//软件清零
	}
}
int main()
{
	UartInit();
	while(1)
	{
		
	}
}