#include "timer1.h"

int timer1_interrupt_counter_V = 0;

typedef unsigned int u16;
typedef unsigned char u8;

static TIMER1_CallBackType tIMER1_CallBackTable[3];


void timer1_IR() interrupt 3{
    static u16 timer1_interrupt_counter = 0;
	  TR1 = 0; 
    TH1 = 0xFC;
    TL1 = 0x6b;
	  TR1 = 1; 
    timer1_interrupt_counter++;
    if( timer1_interrupt_counter >= timer1_interrupt_counter_V ) {  
        timer1_interrupt_counter = 0;
				tIMER1_CallBackTable[1]();
    }
}

void timer1_start(int ms, TIMER1_CallBackType AppCBFun){
	timer1_interrupt_counter_V = ms;
	tIMER1_CallBackTable[1] = AppCBFun;
	
	TMOD &= 0x0f;		 
	TMOD |= 0x10;		 
  TH1 = 0xFC;
  TL1 = 0x6b;
  EA = 1;   
  ET1 = 1; 
  TR1 = 1; 
	
  if(timer1_interrupt_counter_V < 1 || timer1_interrupt_counter_V > 60000 ){ TR1 = 0;}
}


