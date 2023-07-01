#include "head.h"
#define uint unsigned int 
#define uchar unsigned char
void recive(char *da,char temp);
/*本代码实现串口的收发功能，PC发送什么单片机就接收什么，
然后单片机又把接收的发出去，本次编写了在发送单个字符串
函数上添加了字符串函数，方便以后发送字符串调用*/
int displaymode = 0;//显示模式 0 1
int TimeFlag = 0;//倒计时标志位 0未开始倒计时 -1倒计时结束 1开始倒计时
int reset = 1;//重置标志位 reset = 0时初始化da[]为00000并且开放读取串口数据
int stop = 0;//倒计时暂停标志位
int s = 0;//sum 中断计数次数
int i = 1;//数组读取串口下标
int Timeleast_s = -1;//倒计时剩余秒数
char da[5] = "00000";//接收串口数据数组，对应数码管四位数据
void main()
{
	Uart_init(); //串口初始化
	Timer0Init();
	while(1){
		if(displaymode == 1){ //显示模式1
			if(TimeFlag == -1){ //判断是否倒计时结束
				displayover();      //显示结束模式，显示66666666
		}else{
			displayleast(Timeleast_s);//显示剩余倒计时时间  分--秒
		}
		}
		if(displaymode == 0){//显示模式0
			displayinit(da);//初始化输入显示模式 显示da[5]数组中前四位
		}
		if(reset == 1){//da[5]数组重置
			i = 1;
			recive(da,'0');
			recive(da,'0');
			recive(da,'0');
			recive(da,'0');
			i = 1;
			reset = 0;
			TimeFlag = 0;
		}
	}
	
}

void recive(unsigned char da[5],unsigned char temp){ //数组接收函数
	if(i < 5){//从1接到5 接收到i = 5时停止接收
		da[i] = temp; 
		i++;
	}
}
void Uart_1ms() interrupt 1
{
	if(P3_1 == 0){ //当按下k1
		if(stop == 1){	//如果先前按下了k2使得计时暂停
			stop = 0;			//则恢复计时标志位
			while(P3_1 == 0){}//等待按键抬起，防误识别
				return ;   //中断服务结束
		}
		if(TimeFlag == -1)//判断是否是计时结束状态
		{
			displaymode = 0;
			reset = 1;//重置数组da[5]
			while(P3_1 == 0){}//等待按键抬起，防误识别
				return ;//中断服务结束
		}else{
			displaymode = 1;//如果处于显示模式1
		TimeFlag = 1;//进入倒计时模式
		Timeleast_s = (swap(da[1])*10 + swap(da[2]))*60 + swap(da[3])*10 + swap(da[4]);//设置剩余倒计时时间，单位S
		}
	}
	if(P3_0 == 0){//按下k2
		if(displaymode == 0){//若处于计时结束模式
			reset = 1;//重置数组
		}
		if(TimeFlag == 1){//若处于计时模式
			stop = 1;//计时暂停标志位赋1
		}
	}
	s++;//计数
	if(s == 14){ //中断14次为1秒
		if(TimeFlag == 1 && Timeleast_s > 0 && stop == 0){ //判断处于倒计时模式且剩余时间大于0 暂停计时标志位不为1
			Timeleast_s--;//剩余时间减1秒
			}
			if(Timeleast_s == 0){//当倒计时剩余时间为0秒
				TimeFlag = -1;//倒计时模式设为倒计时完成模式
				Timeleast_s = -1;	//令剩余时间为负数，防止误显示
	}
		s = 0;
	}
}
//串口发送一个字符函数
void Uart_send(unsigned char a)
{
	SBUF = a;        //SBUF是一个寄存器，将字符a寄存在SBUF中
	recive(da,a); //将接收到的a发向a
	while(!TI);     //等待发送完毕
	TI = 0;         //软件复位
}
 