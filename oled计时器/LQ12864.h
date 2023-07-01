/************************************************************************************
*  Copyright (c), 2013, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: LQ12864.h
* Project  : OLED
* Processor: STC89C52
* Compiler : Keil C51 Compiler
* 
* Author : Ð¡ÁÖ
* Version: 1.00
* Date   : 2013.8.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64µãÕûOLEDÄ£¿éÇý¶¯ÎÄ¼þ£¬½öÊÊÓÃheltec.taobao.comËùÊÛ²úÆ·
*
* Others: none;
*
* Function List:
*
* 1. void delay(unsigned int z) -- ÑÓÊ±º¯Êý,ºÁÃë
* 2. void IIC_Start() -- ¿ªÆôI2C×ÜÏß
* 3. void IIC_Stop() -- ¹Ø±ÕI2C×ÜÏß
* 4. void Write_IIC_Byte(unsigned char IIC_Byte) -- Í¨¹ýI2C×ÜÏßÐ´Ò»¸öbyteµÄÊý¾Ý
* 5. void OLED_WrDat(unsigned char dat) -- ÏòOLEDÆÁÐ´Êý¾Ý
* 6. void OLED_WrCmd(unsigned char cmd) -- ÏòOLEDÆÁÐ´ÃüÁî
* 7. void OLED_Set_Pos(unsigned char x, unsigned char y) -- ÉèÖÃÏÔÊ¾×ø±ê
* 8. void OLED_Fill(unsigned char bmp_dat) -- È«ÆÁÏÔÊ¾(ÏÔÊ¾BMPÍ¼Æ¬Ê±²Å»áÓÃµ½´Ë¹¦ÄÜ)
* 9. void OLED_CLS(void) -- ¸´Î»/ÇåÆÁ
* 10. void OLED_Init(void) -- OLEDÆÁ³õÊ¼»¯³ÌÐò£¬´Ëº¯ÊýÓ¦ÔÚ²Ù×÷ÆÁÄ»Ö®Ç°×îÏÈµ÷ÓÃ
* 11. void OLED_P6x8Str(unsigned char x, y,unsigned char ch[]) -- 6x8µãÕû£¬ÓÃÓÚÏÔÊ¾ASCIIÂëµÄ×îÐ¡ÕóÁÐ£¬²»Ì«ÇåÎú
* 12. void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]) -- 8x16µãÕû£¬ÓÃÓÚÏÔÊ¾ASCIIÂë£¬·Ç³£ÇåÎú
* 13.void OLED_P16x16Ch(unsigned char x, y, N) -- 16x16µãÕû£¬ÓÃÓÚÏÔÊ¾ºº×ÖµÄ×îÐ¡ÕóÁÐ£¬¿ÉÉèÖÃ¸÷ÖÖ×ÖÌå¡¢¼Ó´Ö¡¢ÇãÐ±¡¢ÏÂ»®ÏßµÈ
* 14.void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- ½«128x64ÏñËØµÄBMPÎ»Í¼ÔÚÈ¡×ÖÈí¼þÖÐËã³ö×Ö±í£¬È»ºó¸´ÖÆµ½codetabÖÐ£¬´Ëº¯Êýµ÷ÓÃ¼´¿É
*
* History: none;
*
*************************************************************************************/

#include "reg52.h"

// ------------------------------------------------------------
// IO¿ÚÄ£ÄâI2CÍ¨ÐÅ
// SCL½ÓP1^3
// SDA½ÓP1^2
// ------------------------------------------------------------
sbit SCL=P3^7; //´®ÐÐÊ±ÖÓ
sbit SDA=P3^6; //´®ÐÐÊý¾Ý

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
/*********************OLEDÇý¶¯³ÌÐòÓÃµÄÑÓÊ±³ÌÐò************************************/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
   SCL = high;		
   SDA = high;
   SDA = low;
   SCL = low;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
   SCL = low;
   SDA = low;
   SCL = high;
   SDA = high;
}

/**********************************************
// Í¨¹ýI2C×ÜÏßÐ´Ò»¸ö×Ö½Ú
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(IIC_Byte & 0x80)
			SDA=high;
		else
			SDA=low;
		SCL=high;
		SCL=low;
		IIC_Byte<<=1;
	}
	SDA=1;
	SCL=1;
	SCL=0;
}

/*********************OLEDÐ´Êý¾Ý************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	Write_IIC_Byte(0x40);			//write data
	Write_IIC_Byte(IIC_Data);
	IIC_Stop();
}
/*********************OLEDÐ´ÃüÁî************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	Write_IIC_Byte(0x00);			//write command
	Write_IIC_Byte(IIC_Command);
	IIC_Stop();
}
/*********************OLED ÉèÖÃ×ø±ê************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLEDÈ«ÆÁ************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}
/*********************OLED¸´Î»************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}
/*********************OLED³õÊ¼»¯************************************/
void OLED_Init(void)
{
	delay(500);//³õÊ¼»¯Ö®Ç°µÄÑÓÊ±ºÜÖØÒª£¡
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0×óÓÒ·´ÖÃ 0xa1Õý³£
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0ÉÏÏÂ·´ÖÃ 0xc8Õý³£
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //³õÊ¼ÇåÆÁ
	OLED_Set_Pos(0,0);
} 
/***************¹¦ÄÜÃèÊö£ºÏÔÊ¾6*8Ò»×é±ê×¼ASCII×Ö·û´®	ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7****************/
void OLED_P6x8Str(unsigned char x, y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
/*******************¹¦ÄÜÃèÊö£ºÏÔÊ¾8*16Ò»×é±ê×¼ASCII×Ö·û´®	 ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7****************/
void OLED_P8x16Str(unsigned char x, y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************¹¦ÄÜÃèÊö£ºÏÔÊ¾16*16µãÕó  ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7****************************/
void OLED_P16x16Ch(unsigned char x, y, N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
/***********¹¦ÄÜÃèÊö£ºÏÔÊ¾ÏÔÊ¾BMPÍ¼Æ¬128¡Á64ÆðÊ¼µã×ø±ê(x,y),xµÄ·¶Î§0¡«127£¬yÎªÒ³µÄ·¶Î§0¡«7*****************/
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}
