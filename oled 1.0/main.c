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
* Author : С��
* Version: 1.00
* Date   : 2013.8.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description: 128*64����OLEDģ�鹦����ʾ�������������������heltec.taobao.com���۲�Ʒ
*
* Others: none;
*
* Function List:
*
* 1. void delay(unsigned int z) -- �����������ڵ�����ʾЧ������ʱ����,STC89C52 12MHZ z=1ʱ��Լ��ʱ1ms,����Ƶ����Ҫ�Լ�����
* 2. void main(void) -- ������
*
* History: none;
*ע��˵����CS����Ҫ�ӵ�
*************************************************************************************/


//#include "reg52.h"
#include "intrins.h"
#include "codetab.h"
#include "LQ12864.h"

void main(void)
{
	unsigned char i;

	OLED_Init(); //OLED��ʼ��
	while(1)
	{
//		OLED_Fill(0xff); //��ȫ��
//		delay(2000);
//		OLED_Fill(0x00); //��ȫ��
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
		
		OLED_CLS();//����
		delay(100);

		Draw_BMP(0,0,128,8,BMP3);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)

		delay(3000);
		OLED_CLS();//����
		delay(100);

	}
}
