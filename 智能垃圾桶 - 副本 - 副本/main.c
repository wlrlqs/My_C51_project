#include <reg52.h>
#include "timer2.h"

#include "stepper_motor_5L4P.h"
#include "timer1.h"


sbit LED_RUN = P3^5;     //运行指示灯
sbit BEEP = P3^0;        //蜂鸣器
sbit ir_fill = P2^7;     //垃圾桶满不满检测传感器
sbit ir_hand = P2^6;     //人手检测
sbit ir_led = P1^0;      //垃圾桶状态指示灯
sbit key_hand = P1^1;   //手动打开关闭垃圾桶按键

unsigned char flag_ir_hand_check = 0;            //红外检测标志位
unsigned char flag_trush_state_open_cloase = 0;   //垃圾桶状态
unsigned char flag_m_open_close = 0;   //手动打开或者关闭
unsigned char flag_fill = 0;
unsigned char key_flag = 0;
unsigned char jpg_flag = 2;

void DelayMs(unsigned int i);
void SerialInit();
void SendByte(unsigned char sbyte);
void SendString(unsigned char *pstr);
void welcome_ui();
void open_ui();
void opened_ui();
void close_ui();
void closed_ui();
void jpg_choose();
void delay(unsigned int i){  //延时函数
	while(i--);
}

void trash_open(){           //打开垃圾桶
	stepper_init(ROT1_SEC2008*3, POSITIVE );
}

void trash_close(){         //关闭垃圾桶
	stepper_init(ROT1_SEC2008*3, INVERTION ); 
}

unsigned char time_conuter = 0;  //垃圾桶关闭时间基准
void time_conter(){           //定时器中断函数
	time_conuter++;
	if(time_conuter > 200) time_conuter = 100;
	LED_RUN = ~LED_RUN;
	if(flag_fill == 1){
		BEEP = !BEEP;
	}else{
		BEEP = 1;
	}
	
}


void main(){
	SerialInit();
	delay(50000);
	SendString("DIR(1);\r\n");
	delay(8000);
	SendString("DIR(1);\r\n");
	delay(8000);
	SendString("CLR(0);\r\n");
	DelayMs(2000);
	welcome_ui();
	timer2_start( 1000, time_conter);  //定时器初始化
	BEEP = 0;   //蜂鸣器初始化
	ir_led = 0;  //垃圾桶状态灯初始化
	//delay(50000);  //延时
	//delay(50000);
	//delay(50000);
	delay(50000);
	while(1){
			//trash_open();
		ir_fill = 1;
//		if(ir_fill == 0){    //垃圾桶桶满检测
//			delay(500);
//			if(ir_fill == 0){
//				flag_fill = 1;
//				flag_ir_hand_check = 0;
//			}
//		}else{
//			flag_fill = 0;
//			ir_led = 0;
//		}
//		if(flag_fill ==1)
//		{
//			//open_ui();
//			//trash_open();
//			//opened_ui();
//			while(flag_fill ==1)
//			{
//				BEEP = !BEEP;
//				LED_RUN = ~LED_RUN;
//				DelayMs(500);
//				LED_RUN = ~LED_RUN;
//				if(ir_fill == 0){    //垃圾桶桶满检测
//			delay(500);
//			if(ir_fill == 0){
//				flag_fill = 1;
//				flag_ir_hand_check = 0;
//			}
//		}else{
//			flag_fill = 0;
//			ir_led = 0;
//		}
//			}
//			BEEP = 1;
//			DelayMs(1000);
//			//close_ui();
//			//trash_close();	
//			//closed_ui();
//			DelayMs(500);
//			//welcome_ui();
//		
//		}
//		else
//		{
//			BEEP = 1;
//		}
		ir_hand = 1;
		//flag_ir_hand_check=0;
		if(ir_hand == 0){  //人手检测
			delay(500);
			if(ir_hand == 0){
				flag_ir_hand_check=1;
				//while(ir_hand == 0);
				time_conuter = 0;
			}
		}else{
			if(key_flag==0){
				flag_ir_hand_check=0;
			}
		}
		if(ir_fill == 0){  //桶满检测
			delay(500);
			if(ir_fill == 0){
				flag_ir_hand_check=1;
				//while(ir_hand == 0);
				time_conuter = 0;
			}
		}
				//手动打开关闭垃圾桶检测
		key_hand = 1;
		if(key_hand == 0){
			delay(500);
			if(key_hand == 0){
				flag_ir_hand_check=1;
				flag_m_open_close = 1;
				time_conuter = 0;
				key_flag = ~key_flag;
				while(key_hand == 0);
			}
		}
		//垃圾桶打开判断
		if( (flag_fill == 0 && flag_trush_state_open_cloase==0 && flag_ir_hand_check == 1) || (flag_fill == 1 && flag_trush_state_open_cloase==0 && flag_m_open_close != 0) ){
			flag_trush_state_open_cloase=1;
			trash_open();
			open_ui();
			delay(8000);
			opened_ui();
			delay(8000);
			ir_led = 0;
			DelayMs(800);
			welcome_ui();
			flag_m_open_close = 0;
		}
		
		//垃圾桶关闭控制
		if(flag_trush_state_open_cloase==1&&flag_ir_hand_check==0)
		{
			DelayMs(2000);
			trash_close();
			close_ui();
			delay(8000);
			closed_ui();
			DelayMs(1000);
			welcome_ui();
			ir_led = 1;
			flag_trush_state_open_cloase=0;
		}
		ir_led = 0;
		

		
	}
	
}
void jpg_choose(){
	switch(jpg_flag){
		case 0:
			SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 1:
			SendString("FSIMG(2138112,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 2:
			SendString("FSIMG(2179072,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 3:
			SendString("FSIMG(2220032,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 4:
			SendString("FSIMG(2260992,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 5:
			SendString("FSIMG(2301952,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 6:
			SendString("FSIMG(2342912,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 7:
			SendString("FSIMG(2383872,0,0,160,128,0);\r\n");
		jpg_flag = 0;
		break;
	}
}
void welcome_ui()//显示欢迎ui界面
{
	SendString("CLR(0);\r\n");
	delay(8000); 
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000); 
//	SendString("QRCODE(8,8,https://qm.qq.com/cgi-bin/qm/qr?k=Ns796uc8EysoG11g1VVrvomLXvHEUSsf&noverify=0#,100,1);\r\n");
//	delay(40000); 
//	SendString("DC16(12,64,'QQ名片',1);\r\n");
	delay(4000); 
	SendString("DC16(76,10,'欢迎使用',1);\r\n");
	delay(3000); 
	SendString("DC16(68,26,'智能垃圾桶',1);\r\n");
//	delay(3000); 
//	SendString("DC16(12,80,'制作:2100300317',1);\r\n");
//	delay(3000); 
//	SendString("DC16(12,96,'李期王申（shen）',1);\r\n");
	delay(3000); 
	
}
void open_ui()//显示开盖ui
{
	SendString("CLR(0);\r\n");
	delay(8000);  
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000);  
	SendString("BOX(9,105,149,115,1);\r\n");//进度条边框
	delay(800);   
	SendString("DC16(9,88,'垃圾桶盖打开中',1);\r\n");
	delay(800);   
	DelayMs(100);
	SendString("BOX(16,28,35,66,1);\r\n");//垃圾桶打开第1块
	delay(800);  
	SendString("BOXF(9,105,25,115,1);\r\n");//进度条填充进度1
	delay(800); 
    SendString("DC16(9,88,'垃圾桶盖打开中*',1);\r\n");
	delay(800);  	
	DelayMs(100);
	delay(800); 
	SendString("PL(51,28,51,66,1);\r\n");//垃圾桶打开第2块左竖线
	delay(800); 
	SendString("PL(70,66,70,28,1);\r\n");//垃圾桶打开第2块右竖线
	delay(800); 
	SendString("PL(70,66,51,66,1);\r\n");//垃圾桶打开第2块底横线
	delay(800);     
    SendString("PL(51,28,68,23,1);\r\n");   //垃圾桶打开第2块顶盖线
	delay(800); 
	SendString("BOXF(9,105,45,115,1);\r\n"); //进度条填充进度2
	delay(800); 
	SendString("DC16(9,88,'垃圾桶盖打开中*',1);\r\n");
	delay(800); 	
	DelayMs(100);
	delay(800); 
	SendString("PL(86,28,86,66,1);\r\n");//垃圾桶打开第3块左竖线
	delay(800); 
	SendString("PL(105,66,105,28,1);\r\n");//垃圾桶打开第3块右竖线
	delay(800);  
	SendString("PL(86,66,105,66,1);\r\n");//垃圾桶打开第3块底横线
	delay(800);   
    SendString("PL(100,17,86,28,1);\r\n");//垃圾桶打开第3块顶盖线
	delay(800);  
	SendString("BOXF(9,105,75,115,1);\r\n"); //进度条填充进度3
	delay(800);  
	SendString("DC16(9,88,'垃圾桶盖打开中**',1);\r\n");
	delay(800);  	
	DelayMs(100);  
	delay(800); 	
    SendString("PL(122,28,122,66,1);\r\n");//垃圾桶打开第4块左竖线
	delay(800); 
	SendString("PL(141,28,141,66,1);\r\n");//垃圾桶打开第4块右竖线
	delay(800);  
	SendString("PL(122,66,141,66,1);\r\n");//垃圾桶打开第4块底横线
	delay(800); 
    SendString("PL(128,11,122,28,1);\r\n"); //垃圾桶打开第4块顶盖线
	delay(800);  
	SendString("BOXF(9,105,149,115,1);\r\n"); //进度条填充进度5
	delay(800);  	
	SendString("DC16(9,88,'垃圾桶盖打开中***',1);\r\n");
	delay(800); 
	DelayMs(600);	
	
}
void opened_ui()
{
	SendString("CLR(0);\r\n");
	delay(40000); 
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000);
	SendString("DC16(9,88,'垃圾桶盖已成功打开*',1);\r\n");
	delay(800);
	SendString("BOXF(9,105,149,115,1);\r\n"); //进度条填充进度5
}

void close_ui()
{
	SendString("CLR(0);\r\n");
	delay(8000); 
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000); 
	SendString("BOX(9,105,149,115,1);\r\n");//进度条边框
	delay(800); 
	SendString("DC16(9,88,'垃圾桶盖关闭中',1);\r\n");
	delay(800); 
	DelayMs(100);
	SendString("PL(16,28,16,66,1);\r\n");//垃圾桶关闭第1块左竖线
	delay(800);
	SendString("PL(35,66,35,28,1);\r\n");//垃圾桶关闭第1块右竖线
	delay(800);
	SendString("PL(16,66,35,66,1);\r\n");//垃圾桶关闭第1块底横线
	delay(800);   
    SendString("PL(24,11,16,28,1);\r\n"); //垃圾桶关闭第1块顶盖线
	delay(800); 
	SendString("BOXF(9,105,25,115,1);\r\n");//进度条填充进度1
	delay(800);
    SendString("DC16(9,88,'垃圾桶盖关闭中*',1);\r\n");
	delay(800);	
	DelayMs(100);
	SendString("PL(51,28,51,66,1);\r\n");//垃圾桶关闭第2块左竖线
	delay(800);
	SendString("PL(70,66,70,28,1);\r\n");//垃圾桶关闭第2块右竖线
	delay(800);
	SendString("PL(70,66,51,66,1);\r\n");//垃圾桶关闭第2块底横线
	delay(800);   
    SendString("PL(66,17,51,28,1);\r\n");   //垃圾桶关闭第2块顶盖线
	delay(800);
	SendString("BOXF(9,105,45,115,1);\r\n"); //进度条填充进度2
	delay(800);
	SendString("DC16(9,88,'垃圾桶盖关闭中*',1);\r\n");
	delay(800);	
	DelayMs(100);
	SendString("PL(86,28,86,66,1);\r\n");//垃圾桶打开第3块左竖线
	delay(800);
	SendString("PL(105,66,105,28,1);\r\n");//垃圾桶打开第3块右竖线
	delay(800);
	SendString("PL(86,66,105,66,1);\r\n");//垃圾桶打开第3块底横线
	delay(800);   
    SendString("PL(100,23,86,28,1);\r\n");//垃圾桶打开第3块顶盖线
	delay(800);
	SendString("BOXF(9,105,75,115,1);\r\n"); //进度条填充进度3
	delay(800);
	SendString("DC16(9,88,'垃圾桶盖关闭中**',1);\r\n");
	delay(800);	
	DelayMs(100);   
    SendString("BOX(122,28,141,66,1);\r\n");//垃圾桶关闭4块
	delay(800);
	SendString("BOXF(9,105,149,115,1);\r\n"); //进度条填充进度5
	delay(800);	
	SendString("DC16(9,88,'垃圾桶盖关闭中***',1);\r\n");
	delay(800);
	DelayMs(600);
}
void closed_ui()
{
	SendString("CLR(0);\r\n");
	delay(20000); 
	SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	delay(20000);
	SendString("DC16(9,88,'垃圾桶盖已成功关闭*',1);\r\n");
	delay(800);
	SendString("BOXF(9,105,149,115,1);\r\n"); //进度条填充进度5
	DelayMs(600);
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
    PCON &= 0x7F; //波特率不倍速
    SCON = 0x50;  //8位数据,可变波特率
    //AUXR |= 0x40; //定时器1时钟为Fosc,即1T
    //AUXR &= 0xFE; //串口1选择定时器1为波特率发生器
    TMOD &= 0x0F; //清除定时器1模式位
    TMOD |= 0x20; //设定定时器1为8位自动重装方式
    TL1 = 0xFD;   //设定定时初值
    TH1 = 0xFD;   //设定定时器重装值
    ET1 = 0;      //禁止定时器1中断
    TR1 = 1;      //启动定时器1
 
    REN = 1; //不开启这个无法接受数据
    ES = 1;  //开启串口中断使能
    EA = 1;
    TI = 0;
    RI = 0;       //串口中断开关,采用查询法时不用打开中断

}


//串口中断函数:
void SerialPortInte(void) interrupt 4 //采用串口中断法收发数据
{
	 unsigned char rbyte;
	 if(RI){     //RI=1,判定为串口接收到了数据，RI要清零，
         RI=0;
		 rbyte=SBUF; 
		 if(rbyte==0x0A){ 	  
		    SendString("");
		 }else if(rbyte==0x0D){
		 	SendString("");
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

