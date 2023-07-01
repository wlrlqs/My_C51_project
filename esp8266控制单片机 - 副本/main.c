
#include "reg51.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int
	
sbit SW1=P3^2;//P1^0;    //S1按键
sbit LED1=P2^6;//P1^1;
sbit LED2=P2^7;//P1^2;
sbit LED3=P2^2;//P1^3;
sbit LED4=P2^3;//P1^4;
 
//ESP8266  EN、vcc脚接 vcc 3.3 GND 接地，必须和51共地
//ESP8266  TX 接P3-^0, RX 接P3^1
 

 
bit flag=0;
 
uchar SendBuf[9]="OK ";
uchar RecBuf[15];
uchar RecNum=0;
 
void delay_10us(uint us); //延时
void delay(uint n);   //延时
void UART_Init();   
void UART_SendByte(uchar dat);
void ESP8266_SendCmd(uchar *pbuf);
void ESP8266_SendData(uchar *pbuf); 
void ESP8266_ModeInit(void);    //WIFI模块初始化
void UART_Irq( );    // 接收信号

void Delay1ms(int k)		//@11.0592MHz
{
	for(k;k>=0;k--)
	{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
	}
}
void delay_10us(uint us)
{
	while(us--);
}
 
void delay(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<100;j++);
}
 
void UART_Init()
{
	SCON=0X50;				//串口：工作方式1
	TMOD=0X20;				//定时器：工作方式2
	TH1=0xFD;	    			//波特率：9600
	TL1=0xFD;
	ES=0;				  	//禁止串口中断
	EA=1;				 	//使能总中断
	TR1=1;					//启动计数器
}
 
void UART_SendByte(uchar dat)
{
	ES=0; 			    		//禁止串口中断
	SBUF=dat;       			//串口发送
	while(TI==0);   			//等待发送结束
	TI=0;           			//发送标志位清零
	ES=1; 					//使能串口中断
}
 
void ESP8266_SendCmd(uchar *pbuf)
{
	while(*pbuf!='\0') 				//遇到空格跳出循环	
	{
			UART_SendByte(*pbuf);
			delay_10us(5);
			pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\r');				//回车
	delay_10us(5);
	UART_SendByte('\n');				//换行
	delay(1000);
}
 
void ESP8266_SendData(uchar *pbuf)    
{
	uchar i=0;
	ESP8266_SendCmd("AT+CIPSEND=0,9");  //发送数据：AT+CIPSEND=<id>,<length>
	for(i=0;i<=8;i++)
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
}
 
void ESP8266_ModeInit(void)    //WIFI模块初始化
{
	//AT+CWSAP_DEF="TCP_Server","12345678",5,4
	ESP8266_SendCmd("AT+CWMODE=2");    //设置路由器模式 1:Station,，2:AP，3:Station+AP
	ESP8266_SendCmd("AT+CWSAP=\"LQS的智能垃圾桶\",\"12345678\",5,4");  //设置WIFI热点名称及密码
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.2\"");   //设置AP的IP地址
	ESP8266_SendCmd("AT+RST");                  //重新启动wifi模块
	ESP8266_SendCmd("AT+CIPMUX=1");	        //开启多连接模式
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	   //启动TCP/IP 端口为8080
	/*ESP8266_SendCmd("AT+CWMODE=1");
	ESP8266_SendCmd("AT+RST");
	delay(1000);
	ESP8266_SendCmd("AT+CWLAP");
	Delay1ms(500);
	ESP8266_SendCmd("AT+CWJAP=\"TP-LINK_8F4E\",\"\"");
	Delay1ms(800);
	ESP8266_SendCmd("AT+CIPMUX=0");
	Delay1ms(800);
	ESP8266_SendCmd("AT+CIPSTART=“TCP”,“api.seniverse.com”,80");
	Delay1ms(200);
	ESP8266_SendCmd("AT+CIPMODE=1");
	Delay1ms(200);
	ESP8266_SendCmd("AT+CIPSEND");
	Delay1ms(200);
	ESP8266_SendCmd("GET https://api.seniverse.com/v3/weather/now.json?key=SaXVUHdckOVA6899e&location=beijing&language=zh-Hans&unit=c");*/
	
}
 
void main()
{	
	Delay1ms(1000);
	 P2=0xff;
	/* while(SW1);   */    				//等待S1键按下
	{LED1=LED2=LED3=LED4=1;}
	 UART_Init();      				//串口初始化
	 ESP8266_ModeInit();
	
	 ES=1;             			//允许串口中断
	
	 while(1)
	 {
		 if(flag==1)
		 {
			 flag = 0;
			 ESP8266_SendData(SendBuf); 
		 }
		 delay(10);
	 }		
}
 
void UART_Irq( ) interrupt 4   // 接收信号
{
	if(RI)
	{
		RI=0;
		RecBuf[RecNum]=SBUF;   //接收到网络数据：+IPD,0<id>,1<数据长度>:F<接收的数据>
		if(RecBuf[0]=='+')
			RecNum++;
		else 
			RecNum=0;
		if(RecNum==10)
		{
			RecNum=0;
			if(RecBuf[0]=='+'&&RecBuf[1]=='I'&&RecBuf[2]=='P'&&RecBuf[3]=='D')
			{
				switch(RecBuf[9])
				{
					case '1': LED1=0;break; 					
					/*case '2': LED2=0;break;
					case '3': LED3=0;break;
					case '4': LED4=0;break;*/
					default:LED1=1;//P2 = 0xff;				
				}
				flag = 1;
 			}
		}				
	}
}