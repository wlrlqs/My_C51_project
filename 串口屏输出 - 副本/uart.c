#include "reg52.h"
/**
  * @brief  串口初始化，//9600bps@11.0592MHz
  * @param  无
  * @retval 无
  */
void UartInit(void)		//9600bps@11.0592MHz
{
	 PCON &= 0x7F;      //波特率不倍速 SMOD=0
     SCON = 0x50;       //方式1,8位数据,可变波特率,接收允许
     T2CON  = 0x34;   
     RCAP2H = 0xFF;    
     RCAP2L = 0xDC;  
     TH2    = 0xFF;  
     TL2    = 0xDC;
	   
	 EA=1; //总中断打开,采用查询法时不用打开中断 
     ES = 1;          //串口中断开关,采用查询法时不用打开中断
}



/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
