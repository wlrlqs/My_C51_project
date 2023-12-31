#include <reg52.h>
#include <stdio.h>
#include "usartinit.h"
#define Usart_Data_length 5
sbit LED0 = P1^0;
extern unsigned char getData;
unsigned char Usart_Data[Usart_Data_length];
unsigned int Usart_Data_count,Usart_flag = 0,Usart_error = 0;

void uart_init(){
   PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
 
    REN = 1; //不开启这个无法接受数据
    ES = 1;  //开启串口中断使能
    EA = 1;
    TI = 0;
    RI = 0;       //串口中断开关,采用查询法时不用打开中断   
} 

void uart_send_byte(unsigned char d){ //发送一个字节的数据，形参d即为待发送数据。
	ES = 0;
	SBUF = d;
	while(!TI);
	TI = 0;   
	ES = 1;
}
//发送一串字符串
void uart_send_string(unsigned char *str){
 	while(*str){
	 	uart_send_byte(*str++);	
	}
}

void uart(void) interrupt 4 {		 //串口发送中断
    if(RI){    //收到数据		
				getData = SBUF;
				if(Usart_flag && Usart_Data_count < Usart_Data_length - 1){//循环接收
					Usart_Data_count ++;  
					Usart_Data[Usart_Data_count] = getData;
				}
				if(Usart_flag && Usart_Data_count == Usart_Data_length - 1){//接收完成flag置0
					Usart_flag = 0;
					if((unsigned int)(Usart_Data[Usart_Data_length - 1]) != Usart_Data_length){//校验
						Usart_error = 1;
					}else{
						Usart_error = 0;
						//uart_send_string(Usart_Data);
					}
				}
				if(getData == 0x5A){//收到帧头则开始
					Usart_flag = 1;
					Usart_Data_count = 0;  
					Usart_Data[Usart_Data_count] = getData;
				}
        RI=0;   //清中断请求
    }else{        //发送完一字节数据
       TI=0;
		}
}