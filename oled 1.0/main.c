/************************************************************************************
*  Copyright (c), 2013, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: OLED12864.c
* Project  : OLED
* Processor: STC89C52
* Compiler : Keil C51 Compiler
* 
* Author : 小林
* Version: 1.00
* Date   : 2013.8.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description: 128*64点整OLED模块功能演示程序的主函数，仅适用heltec.taobao.com所售产品
*
* Others: none;
*
* Function List:
*
* 1. void delay(unsigned int z) -- 主函数中用于调整显示效果的延时函数,STC89C52 12MHZ z=1时大约延时1ms,其他频率需要自己计算
* 2. void main(void) -- 主函数
*
* History: none;
*注意说明：CS脚需要接地
*************************************************************************************/


//#include "reg52.h"
#include "intrins.h"
#include "codetab.h"
#include "LQ12864.h"

void main(void)
{
	unsigned char i;

	OLED_Init(); //OLED初始化
	while(1)
	{
//		OLED_Fill(0xff); //屏全亮
//		delay(2000);
//		OLED_Fill(0x00); //屏全灭
//		delay(200);

		OLED_P8x16Str(47,0,"LQS");
		OLED_P16x16Ch(72,0,0);
		OLED_P16x16Ch(24,2,1);
		OLED_P16x16Ch(40,2,2);
		OLED_P16x16Ch(57,2,3);
		OLED_P16x16Ch(74,2,4);
		OLED_P16x16Ch(91,2,5);
		OLED_P8x16Str(47,4,"(*3*)");
		OLED_P16x16Ch(40,6,6);
		OLED_P16x16Ch(57,6,7);
		OLED_P16x16Ch(74,6,8);
		OLED_P8x16Str(91,6,"!");
		
		delay(3000);
		
		OLED_CLS();//清屏
		delay(100);

		Draw_BMP(0,0,128,8,BMP3);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)

		delay(3000);
		OLED_CLS();//清屏
		delay(100);

	}
}
