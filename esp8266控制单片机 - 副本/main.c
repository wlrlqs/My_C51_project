
#include "reg51.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int
	
sbit SW1=P3^2;//P1^0;    //S1����
sbit LED1=P2^6;//P1^1;
sbit LED2=P2^7;//P1^2;
sbit LED3=P2^2;//P1^3;
sbit LED4=P2^3;//P1^4;
 
//ESP8266  EN��vcc�Ž� vcc 3.3 GND �ӵأ������51����
//ESP8266  TX ��P3-^0, RX ��P3^1
 

 
bit flag=0;
 
uchar SendBuf[9]="OK ";
uchar RecBuf[15];
uchar RecNum=0;
 
void delay_10us(uint us); //��ʱ
void delay(uint n);   //��ʱ
void UART_Init();   
void UART_SendByte(uchar dat);
void ESP8266_SendCmd(uchar *pbuf);
void ESP8266_SendData(uchar *pbuf); 
void ESP8266_ModeInit(void);    //WIFIģ���ʼ��
void UART_Irq( );    // �����ź�

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
	SCON=0X50;				//���ڣ�������ʽ1
	TMOD=0X20;				//��ʱ����������ʽ2
	TH1=0xFD;	    			//�����ʣ�9600
	TL1=0xFD;
	ES=0;				  	//��ֹ�����ж�
	EA=1;				 	//ʹ�����ж�
	TR1=1;					//����������
}
 
void UART_SendByte(uchar dat)
{
	ES=0; 			    		//��ֹ�����ж�
	SBUF=dat;       			//���ڷ���
	while(TI==0);   			//�ȴ����ͽ���
	TI=0;           			//���ͱ�־λ����
	ES=1; 					//ʹ�ܴ����ж�
}
 
void ESP8266_SendCmd(uchar *pbuf)
{
	while(*pbuf!='\0') 				//�����ո�����ѭ��	
	{
			UART_SendByte(*pbuf);
			delay_10us(5);
			pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\r');				//�س�
	delay_10us(5);
	UART_SendByte('\n');				//����
	delay(1000);
}
 
void ESP8266_SendData(uchar *pbuf)    
{
	uchar i=0;
	ESP8266_SendCmd("AT+CIPSEND=0,9");  //�������ݣ�AT+CIPSEND=<id>,<length>
	for(i=0;i<=8;i++)
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
}
 
void ESP8266_ModeInit(void)    //WIFIģ���ʼ��
{
	//AT+CWSAP_DEF="TCP_Server","12345678",5,4
	ESP8266_SendCmd("AT+CWMODE=2");    //����·����ģʽ 1:Station,��2:AP��3:Station+AP
	ESP8266_SendCmd("AT+CWSAP=\"LQS����������Ͱ\",\"12345678\",5,4");  //����WIFI�ȵ����Ƽ�����
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.2\"");   //����AP��IP��ַ
	ESP8266_SendCmd("AT+RST");                  //��������wifiģ��
	ESP8266_SendCmd("AT+CIPMUX=1");	        //����������ģʽ
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	   //����TCP/IP �˿�Ϊ8080
	/*ESP8266_SendCmd("AT+CWMODE=1");
	ESP8266_SendCmd("AT+RST");
	delay(1000);
	ESP8266_SendCmd("AT+CWLAP");
	Delay1ms(500);
	ESP8266_SendCmd("AT+CWJAP=\"TP-LINK_8F4E\",\"\"");
	Delay1ms(800);
	ESP8266_SendCmd("AT+CIPMUX=0");
	Delay1ms(800);
	ESP8266_SendCmd("AT+CIPSTART=��TCP��,��api.seniverse.com��,80");
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
	/* while(SW1);   */    				//�ȴ�S1������
	{LED1=LED2=LED3=LED4=1;}
	 UART_Init();      				//���ڳ�ʼ��
	 ESP8266_ModeInit();
	
	 ES=1;             			//�������ж�
	
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
 
void UART_Irq( ) interrupt 4   // �����ź�
{
	if(RI)
	{
		RI=0;
		RecBuf[RecNum]=SBUF;   //���յ��������ݣ�+IPD,0<id>,1<���ݳ���>:F<���յ�����>
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