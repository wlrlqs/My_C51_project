#ifndef __MAIN_H__
#define __MAIN_H__
#include "usartinit.h"
#include "intrins.h"
#include "OLED12864.h"
#include "motor.h"
#include "key_scan.h"
#define LED P1
extern void OLED_Init(void);
unsigned char getData;
float w_speed,w_distance,ws_distance;
int speed_choose;
extern unsigned int Usart_flag,speed_delay;
extern unsigned int display_mode,display_last_mode;
extern unsigned char led_speed[4];
#endif