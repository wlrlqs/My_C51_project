#ifndef __OLED12864_H__
#define __OLED12864_H__
sbit LED0 = P1^0;
sbit LED1 = P1^1;
sbit LED2 = P1^2;
sbit LED3 = P1^3;
sbit LED4 = P1^4;
sbit LED5 = P1^5;
sbit LED6 = P1^6;
sbit LED7 = P1^7;
void oled(void);
void led(void);
void usart_led(void);
#endif