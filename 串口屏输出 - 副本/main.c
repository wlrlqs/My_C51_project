#include <reg52.h>


void DelayMs(unsigned int i);
void SerialInit();
void SendByte(unsigned char sbyte);
void SendString(unsigned char *pstr);


void main(void)
{
		SerialInit();
	    SendString("CLR(1);\r\n");
		while(1);	  //注:必需要无限循环
}


/*
//单片机时钟周期:11.0592MHz   以时钟1作为波特率发生器
void SerialInit(){
 TMOD=0x20;   //设置T1工作方式为方式2
 TH1=0xfd; //给定时器高位装初值
 TL1=0xfd; //给定时器低位装初值
 TR1=1;  //开定时器
             //以上是设置波特率
 SM0=0;  //设置串口通讯方式为方式1
 SM1=1;  
 REN=1;  //串口是否接收数据的开关

 EA=1; //总中断打开,采用查询法时不用打开中断
 ES=1; //串口中断开关,采用查询法时不用打开中断
}
*/
//单片机时钟周期:11.0592MHz	   以时钟T2作为波特率发生器
void SerialInit(){
 	 PCON &= 0x7F;		//波特率不倍速
	SCON = 0x40;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;			//设定定时初值
	TH1 = 0xFD;			//设定定时器重装值		
	TR1 = 1;			//启动定时器1
	ET1 = 0;        	//禁止定时器1中断

}


//串口中断函数:
void SerialPortInte(void) interrupt 4 //采用串口中断法收发数据
{
	 unsigned char rbyte;
	 if(RI){     //RI=1,判定为串口接收到了数据，RI要清零，
         RI=0;
		 rbyte=SBUF; 
		 if(rbyte==0x0A){ 	  
		    SendString("换行");
		 }else if(rbyte==0x0D){
		 	SendString("回车");
		 }else{
		 	SendByte(rbyte); 
		 }
		 	
     }

}

//串口发送一个字节:
void SendByte(unsigned char sbyte)
{ 
     SBUF=sbyte; //发送数据
     while(!TI); //等待发送完成
     TI=0; //清零发送标志位
}
 
//串口发送一个字符串:
void SendString(unsigned char *pstr) //定义指针
{
     while(*pstr!='\0') //字符串是否发完
     {
         SendByte(*pstr);//发送字符串数据
         pstr++; //指向下一个字符
     }
}

void DelayMs(unsigned int i)  //延时i ms
{
    unsigned int j;
    while(i--)
    {
        for(j = 0; j < 125; j++);
    }
}