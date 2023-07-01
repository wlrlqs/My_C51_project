#ifndef __MAIN_H__
#define __MAIN_H__
#include "usartinit.h"
#include "intrins.h"
#include "OLED12864.h"
#define LED P1
sbit LED0 = P1^0;
sbit LED1 = P1^1;
sbit LED2 = P1^2;
sbit LED3 = P1^3;
sbit LED4 = P1^4;
sbit LED5 = P1^5;
sbit LED6 = P1^6;
sbit LED7 = P1^7;
sbit key = P3^2;
sbit key2 = P2^4;
sbit key3 = P2^5;
sbit key4 = P2^6;
sbit key5 = P2^7;
bit key_flag;
extern void OLED_Init(void);
unsigned char getData;
float w_speed,w_distance,ws_distance;
int speed_choose;
extern unsigned int Usart_flag,speed_delay;
extern unsigned int display_mode,display_last_mode;
extern unsigned char led_speed[4];
#endif