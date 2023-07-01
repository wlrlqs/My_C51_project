#include "reg52.h"
sbit led0 = P2^0;
void timer0Init(){ //1ms
	
	TMOD= 0x01;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值 //11001101
	TH0 = 0xD4;		//设置定时初值 //1101010011001101 65536
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时

}
int sum = 0;
int main(){
	timer0Init();
	while(1){
		
	}
}

void timer0() interrupt 1
{
		sum++;
		TL0 = 0xCD;		//设置定时初值 //11001101
		TH0 = 0xD4;
	if(sum == 1000){
		sum = 0;
		led0 = ~led0;
	}
}