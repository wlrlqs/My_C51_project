///************************************************************************************
//*  Copyright (c), 2013, HelTec Automatic Technology co.,LTD.
//*            All rights reserved.
//*
//* Http:    www.heltec.cn
//* Email:   cn.heltec@gmail.com
//* WebShop: heltec.taobao.com
//*
//* File name: OLED12864.c
//* Project  : OLED
//* Processor: STC89C52
//* Compiler : Keil C51 Compiler
//* 
//* Author : С��
//* Version: 1.00
//* Date   : 2013.8.8
//* Email  : hello14blog@gmail.com
//* Modification: none
//* 
//* Description: 128*64����OLEDģ�鹦����ʾ�������������������heltec.taobao.com���۲�Ʒ
//*
//* Others: none;
//*
//* Function List:
//*
//* 1. void delay(unsigned int z) -- �����������ڵ�����ʾЧ������ʱ����,STC89C52 12MHZ z=1ʱ��Լ��ʱ1ms,����Ƶ����Ҫ�Լ�����
//* 2. void main(void) -- ������
//*
//* History: none;
//*ע��˵����CS����Ҫ�ӵ�
//*************************************************************************************/

//#include<reg52.h>
//#include "intrins.h"
//#include "codetab.h"
//#include "LQ12864.h"

//void main(void)
//{
//	unsigned char i;

//	OLED_Init(); //OLED��ʼ��
//	while(1)
//	{
////		OLED_Fill(0xff); //��ȫ��
////		delay(2000);
////		OLED_Fill(0x00); //��ȫ��
////		delay(200);
//		OLED_P16x16Ch(24,0,1);
//		OLED_P16x16Ch(40,0,2);
//		OLED_P16x16Ch(57,0,3);
//		OLED_P16x16Ch(74,0,4);
//		OLED_P16x16Ch(91,0,5);
//		for(i=0; i<8; i++)//ͨ��������ʾ���� -- i��ʾ�ֱ������λ��
//		{
////			OLED_P16x16Ch(i*16,0,i);
//		 	OLED_P16x16Ch(i*16,2,i+8);
//		 	OLED_P16x16Ch(i*16,4,i+16);
//		 	OLED_P16x16Ch(i*16,6,i+24);
//		}
//		delay(4000);
//		OLED_CLS();//����

//		OLED_P8x16Str(0,0,"HelTec");//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
//		OLED_P8x16Str(0,2,"OLED Display");
//		OLED_P8x16Str(0,4,"www.heltec.cn");
//		OLED_P6x8Str(0,6,"cn.heltec@gmail.com");
//		OLED_P6x8Str(0,7,"heltec.taobao.com");
//		delay(4000);
//		OLED_CLS();

//		Draw_BMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ��ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
//		delay(8000);
//		Draw_BMP(0,0,128,8,BMP2);
//		delay(8000);
//	}
//}
#include "RTX51TNY.h"
#include "reg52.h"
sbit led0=P1^0;
sbit led1=P1^1;
/***************************************
* �� �� ��       : task_create
* ��������     : ����0
* ��    ��       : ��
* ��    ��      : ��
****************************************/
void task_create(void) _task_ 0
{
 os_create_task(1);
 os_create_task(2);
 os_delete_task(0);
 while(1)
 {
  ;
 }  
}
/*****************************************
* �� �� ��       : task_1
* ��������     : ����1
* ��    ��       : ��
* ��    ��      : ��
******************************************/
void task_1(void) _task_ 1
{
 while(1)
 {
  led0=!led0;
  os_send_signal(2);
  os_wait2(K_IVL,250);
  os_wait2(K_IVL,250);
 }  
}

/*******************************************
* �� �� ��       : task_2
* ��������     : ����2
* ��    ��       : ��
* ��    ��      : ��
*******************************************/
void task_2(void) _task_ 2
{
 unsigned char sig;
 while(1)
 {
  

  sig=os_wait2(K_SIG|K_IVL,10);
  os_reset_interval(10);
  if(sig==SIG_EVENT)
  {
   led1=0;
  }else
  {
   led1=1;
  }
  os_wait2(K_IVL,100);
 }  
}