#include <REGX52.H>
sbit LED0 = P1^0;
/**
  * @brief 串口初始化
  * @param  无
  * @retval 无
  */
void Uart_Init()	//4800bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xC7;		//设定定时初值
	TH1 = 0xFE;		//设定定时初值
	ET1 = 1;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	
	//接收数据完成后就触发中断
	ES=1;
	EA=1;
}

/**
  * @brief 串口发送一位字节数据
  * @param  Byte 要发送的字节数据
  * @retval 无
  */
void Uart_SendByte(unsigned char Byte)
{
	SBUF=Byte;   //向缓存器中写入内容
	while(TI==0);   //发送中断标志位 如果检测到了寄存器的TI位 如果为0 就表示数据未发送完成  反复执行本条语句检测TI位
	//如果TI为1 就表示SUBF的数据已经发送完成了  马上执行下条语句将TI位清零
	TI=0;
}
void main()
{
	Uart_Init();
	
	while(1)
	{
		Uart_SendByte('a');
	}
}
// 串口中断函数模板
void Uart_Routine() interrupt 4
{
	if(RI==1)   //如果接收到数据 接收完成后 产生中断 如果是接收中断  
		//发送数据也会产生中断
	{
		LED0 = ~LED0;  //从SUBF缓存器中读取数据
		//Uart_SendByte(SBUF);
		RI=0;
	}
}


