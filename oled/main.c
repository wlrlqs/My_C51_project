#include <reg51.h>
#define uchar unsigned char 	//�궨�壬Ϊ������
#define uint unsigned int
#define DIGI P0 				//�궨�壬��P0�ڶ���Ϊ����ܶ�ѡ�ӿ�
#define SELECT P2 				//�궨�壬��P2����Ϊ�����ѡ���

unsigned char  digivalue[]={0x07,0x06,0x4F,0x3F,0x3F,0x4F,0x3F,0x3F};
//��ʾ���������飬����Ϊ0��1��..��7
unsigned char select[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //ѡ����������飬����ѡ��1��2��..��8

void delay() //�ӳٺ������������������ļ��ʱ��
{ 
	uchar ii=200; //�������������˸���������Ｔ��
	while(ii--);
}

int main() //������
{
	uchar i=0;
	while(1)
	{ 
		for(i=0;i<8;i++) 		//8�������������ʾ
		{
			SELECT=select[i]; 	//ѡ���i�������
			DIGI=0x90;
			DIGI=digivalue[i]; 	//��ʾi
			delay();
		} 
	}
	return 0;
}
