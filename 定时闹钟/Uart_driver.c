#include "head.h"
//串口初始化函数
void Uart_init()
{
	
	REN = 1;			//允许接收
	EA = 1;				//允许中断
	ES = 1; 			//串行中断允许
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
    
}
void Timer0Init(void)		//10毫秒@11.0592MHz
{
	//1. 配置定时器0工作模式位16位计时
	TMOD |= 0x01;
	//2. 给初值，定一个10ms出来
	TL0=0x00;
	TH0=0xDC;
	//3. 开始计时，定时器"数数"
	TR0 = 1;
	TF0 = 0;
	//4. 打开定时器0中断
	ET0 = 1;
	//5. 打开总中断EA
	EA = 1;
}
//串口发送字符串函数   
void Uart_str(unsigned char *s)
{
	while(*s != '\0')  //判断字符串是否发送完毕
	{
		Uart_send(*s);
		s++;              //字符地址+1
	}
}
 //串口中断服务函数
void Uart() interrupt 4
{
	unsigned char temp;       //用于存放读出的寄存器SBUF中的值
	if(RI)               //检测串口接收中断标志，接收时为1
	{		
		temp = SBUF;             //将SBUF寄存器的值存在temp中
		Uart_send(temp);        //发送temp
		RI = 0;                //清中断标志
	}
}

