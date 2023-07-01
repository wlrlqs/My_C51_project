#include <regx52.h>
#include <stdio.h>
#include <stdlib.h>
sbit LSA=P2^2;	//74HC138译码器端口
sbit LSB=P2^3;
sbit LSC=P2^4;
unsigned char out[18]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00,0x40};
void delay1(unsigned int	ms)
{
	unsigned char i;
	while(ms--)
		for(i=0;i<1;i++);
}
void display(int j,int d)
{
		switch(j)	 //位选，选择点亮的数码管，
		{
			case(7):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(6):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(5):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(4):
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(3):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(2):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(1):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(0):
				LSA=1;LSB=1;LSC=1; break;//显示第7位	
		}
		P0=out[d];//发送段码
		delay1(10); //间隔一段时间扫描	
		P0=0x00;//消隐
}
int swap(unsigned char da){
int k;
switch(da){
			case('0'):
				k = 0;break;
			case('1'):
				k = 1;break;
			case('2'):
				k = 2;break;
			case('3'):
				k = 3;break;
			case('4'):
				k = 4;break;
			case('5'):
				k = 5;break;
			case('6'):
				k = 6;break;
			case('7'):
				k = 7;break;
			case('8'):
				k = 8;break;
			case('9'):
				k = 9;break;	
		}
	return k;
}
void displayinit(unsigned char da[5]){
	unsigned int k;
	display(0,10);
	display(1,8);
	display(2,8);
	k = swap(da[1]);
	display(3,k);
	k =	swap(da[2]);
	display(4,k);
	display(5,17);
	k = swap(da[3]);
	display(6,k);
	k = swap(da[4]);
	display(7,k);
}
void displayleast(int Timeleast_s){
	int D4 = (Timeleast_s%60)%10;
	int D3 = (Timeleast_s%60 - D4)/10;
	int D2 = (Timeleast_s/60)%10;
	int D1 = Timeleast_s/600;
	
	if(D1 < 0){
		D1 = 0;
	}
	if(D2 < 0){
		D2 = 0;
	}
	if(D3 < 0){
		D3 = 0;
	}
	if(D4 < 0){
		D4 = 0;
	}
	display(0,11);
	display(1,8);
	display(2,8);
	display(3,D1);
	display(4,D2);
	display(5,17);
	display(6,D3);
	display(7,D4);
}
void displayover(){
	display(0,6);
	display(1,6);
	display(2,6);
	display(3,6);
	display(4,6);
	display(5,6);
	display(6,6);
	display(7,6);
}