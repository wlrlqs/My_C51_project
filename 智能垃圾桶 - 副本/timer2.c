#include "timer2.h"

unsigned int timer2_interrupt_counter_V = 0;

void timer2_IR() interrupt 5{
    static unsigned int timer2_interrupt_counter = 0; 
		TF2 = 0;
    timer2_interrupt_counter++;
    if( timer2_interrupt_counter >= timer2_interrupt_counter_V ) {  
        timer2_interrupt_counter = 0;
				TIMER_CallBackTable[1]();
    }
	/*   static u16 timer1_interrupt_counter = 0;
	  TR1 = 0; 
    TH1 = 0xFC;
    TL1 = 0x6b;
	  TR1 = 1; 
    timer1_interrupt_counter++;
    if( timer1_interrupt_counter >= timer1_interrupt_counter_V ) {  
        timer1_interrupt_counter = 0;
				tIMER1_CallBackTable[1]();*/
}

void timer2_start(int ms, TIMER_CallBackType AppCBFun){
	timer2_interrupt_counter_V = ms;
	TIMER_CallBackTable[1] = AppCBFun;
	
	T2MOD = 0;		 
	T2CON = 0;		 

	TL2 = 0x66;		//设置定时初值
	TH2 = 0xFC;		//设置定时初值
	RCAP2L = 0x66;		//设置定时重载值
	RCAP2H = 0xFC;		//设置定时重载值
	TR2 = 1;		 

  EA = 1;
	ET2 = 1;
  TR2 = 1; 
	PT2 = 1;
	TF2 = 0;
	/*	timer1_interrupt_counter_V = ms;
	tIMER1_CallBackTable[1] = AppCBFun;
	
	TMOD &= 0x0f;		 
	TMOD |= 0x10;		 
  TH1 = 0xFC;
  TL1 = 0x6b;
  EA = 1;   
  ET1 = 1; 
  TR1 = 1; 
	
  if(timer1_interrupt_counter_V < 1 || timer1_interrupt_counter_V > 60000 ){ TR1 = 0;}*/
}

