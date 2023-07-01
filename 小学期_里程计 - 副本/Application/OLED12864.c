#include "codetab.h"
#include "LQ12864.h"
#include "OLED12864.h"
#include <rtx51tny.h>
#include <stdio.h>
#include <string.h>
#define LED P1
sbit LED0 = P1^0;
sbit LED1 = P1^1;
sbit LED2 = P1^2;
sbit LED3 = P1^3;
sbit LED4 = P1^4;
sbit LED5 = P1^5;
sbit LED6 = P1^6;
sbit LED7 = P1^7;
int oled_i = 0,led_i = 0,line_i = 0;
extern float w_speed,w_distance,ws_distance;
extern int speed_choose;
unsigned char speed_ch[6],distance_ch[6];
extern unsigned int Usart_flag;
unsigned int display_mode = 6,display_last_mode = 0,distance_mode = 0;
unsigned char led_speed[4] = {0x7F,0x3F,0x1F,0x0F};
void usart_led(void){
	if(Usart_flag){
			led_i = 30;
		}else{
			led_i = 0;
		}
		while(led_i){
			os_wait2(K_TMO,50);
			LED0 = !LED0;
			led_i --;
		}
}
void float_to_char(float num, unsigned char str[6]){
	int whole_part;    // 取整数部分 
	float decimal_part;    // 取小数部分
	whole_part = (int)num;    // 取整数部分 
	decimal_part = (num - whole_part) * 100;    // 取小数部分
	str[0] = (unsigned char)(whole_part / 10) + '0';
	str[1] = (unsigned char)(whole_part % 10) + '0';
	str[2] = '.';
	str[3] = (unsigned char)((int)decimal_part / 10) + '0';
	str[4] = (unsigned char)((int)decimal_part % 10) + '0';
	str[5] = '\0';
}
void float_1_to_char(float num, unsigned char str[6]){
	int whole_part;    // 取整数部分 
	float decimal_part;    // 取小数部分
	whole_part = (int)num;    // 取整数部分 
	decimal_part = (num - whole_part) * 100;    // 取小数部分
	str[0] = (unsigned char)(whole_part / 100) + '0';
	str[1] = (unsigned char)((whole_part / 10) % 10) + '0';
	str[2] = (unsigned char)(whole_part %  10) + '0';
	str[3] = '.';
	str[4] = (unsigned char)((int)decimal_part / 10) + '0';
	str[5] = '\0';
}
int ii = 0;
void led(void){
	switch (display_mode){
		case 1:
			LED = 0xAA;
			os_wait2(K_TMO,200);
			os_wait2(K_TMO,100);
			LED = 0x55;
			os_wait2(K_TMO,200);
			os_wait2(K_TMO,100);
			if(speed_choose == 2 || speed_choose == 3){
				P2 = P2 | 0xF0;
			}
			break;
		
		case 2:
			break;
			
		case 3:
			LED = P2 & led_speed[speed_choose];
			break;
			
		case 4:
			LED = 0x00;
			os_wait2(K_TMO,200);
			os_wait2(K_TMO,200);
			LED = 0xFF;
			os_wait2(K_TMO,200);
			os_wait2(K_TMO,200);
			break;
		
		case 5:
			
			break;
			
		case 6:
			LED = 0xFE;
			for(ii = 0; ii < 8;ii++){
				LED = (LED >> (8 - 1) | (LED << 1));//(x >> (32 - n) | (x << n))循环左移//(x << (32 - n) | (x >> n))右移
				os_wait2(K_TMO,75);
			}
			break;
		
		default :
			break;
	}
	
}
void oled(void){
	if(w_distance >= 1000 && !distance_mode){
			distance_mode = 1;
			OLED_P8x16Str(112,4,"  ");
			OLED_P6x8Str(112,5,"KM");
		}
		if(!distance_mode){
			ws_distance = w_distance;
		}else{
			ws_distance = w_distance / 1000;
		}
	switch (display_mode){
		case 1:
			display_last_mode = display_mode;
			float_to_char(w_speed,speed_ch);
			float_1_to_char(ws_distance,distance_ch);
			OLED_P6x8Str(7,0,"Mil billing system");
			OLED_P6x8Str(1 ,1,"--------------------");
			OLED_P8x16Str(0,2,"Speed:");
			OLED_P8x16Str(48,2,speed_ch);
			OLED_P8x16Str(88,2,"M/s");
			OLED_P8x16Str(0,4,"Distance:");
			OLED_P8x16Str(72,4,distance_ch);
			if(!distance_mode){
				OLED_P8x16Str(112,4,"M");
			}else{
				if(!oled_i){
					OLED_P8x16Str(112,4,"  ");
					oled_i = 1;
				}
				OLED_P6x8Str(112,5,"KM");
			}
			OLED_P8x16Str(0,6,"Stop_");
			OLED_P6x8Str(40,6,"Push K2 Start");
			OLED_P6x8Str(40,7,"Push K3 Clear");
			os_wait2(K_TMO,25);
			OLED_P6x8Str(40,6,"             ");
			OLED_P6x8Str(40,6,"Push K2 Start");
			OLED_P6x8Str(40,7,"             ");
			OLED_P6x8Str(40,7,"Push K3 Clear");
			break;
		
		case 2:
			display_last_mode = display_mode;
			OLED_P8x16Str(0,6,"Running");
			OLED_P6x8Str(56,6,"K2 SW Speed");
			OLED_P6x8Str(56,7,"            ");
			display_mode = 3;
			break;
			
		case 3:
			oled_i = 0;
			display_last_mode = display_mode;
			w_speed += 0.2;
		  w_distance += 100.2;
			float_to_char(w_speed,speed_ch);
			float_1_to_char(ws_distance,distance_ch);
			OLED_P8x16Str(48,2,speed_ch);
			OLED_P8x16Str(72,4,distance_ch);
			OLED_P6x8Str(56 + line_i * 6,7,"_");
			line_i ++;
			if(line_i == 12){
				line_i = 0;
				OLED_P6x8Str(56,7,"            ");
				OLED_P6x8Str(56,7,"Push K3 Stop");
			}
			break;
			
		case 4:
			display_last_mode = display_mode;
			OLED_P8x16Str(32,1,"Distance");
			OLED_P8x16Str(28,3,"Set zero!");
			OLED_P8x16Str(0,5,"Push K2 continue");
			OLED_P8x16Str(0,5,"                ");
			OLED_P8x16Str(0,5,"Push K2 continue");
			w_speed = 0;
			w_distance = 0;
			ws_distance = 0;
			distance_mode = 0;
			break;
		
		case 5:
			OLED_CLS();//清屏
			os_wait2(K_TMO,120);
			switch (display_last_mode){
				case 4:
					display_mode = 6;
					break;
				case 1:
					display_mode = 4;
					break;
				case 6:
					display_mode = 1;
					break;
				default :
					break;
			}
			break;
			
		case 6:
			display_last_mode = display_mode;
			OLED_P8x16Str(8,0,"Welcome To Use");
			OLED_P6x8Str (7,2,"Mil billing system");
			OLED_P8x16Str(0,3,"GUET----LQS");
//			OLED_P6x8Str (88,3,"21003");
//			OLED_P6x8Str (88,4,"00317");
			OLED_P8x16Str(8,5,"EVOLUTION-TEAM");
			OLED_P6x8Str(10,7,"Push K2 To Continue");
			OLED_P6x8Str(10,7,"                   ");
			OLED_P6x8Str(10,7,"Push K2 To Continue");
			break;
		
		default :
			break;
	}
	
//		for(oled_i=0; oled_i<8; oled_i++)//通过点整显示汉字 -- i表示字表数组的位置
//		{
////			OLED_P16x16Ch(i*16,0,i);
//		 	OLED_P16x16Ch(oled_i*16,2,oled_i+8);
//		 	OLED_P16x16Ch(oled_i*16,4,oled_i+16);
//		 	OLED_P16x16Ch(oled_i*16,6,oled_i+24);
//		}
}