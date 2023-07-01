#include <reg52.h>
#include <stdio.h>
#include "usartinit.h"
#define Usart_Data_length 5
sbit LED0 = P1^0;

extern unsigned char getData;
unsigned char Usart_Data[Usart_Data_length];
unsigned int Usart_Data_count,Usart_flag = 0,Usart_error = 0;

void Timer1Init(void)		//9����@12.000MHz
{
	//AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x20;		//���ö�ʱ��ֵ
	TH1 = 0xD1;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

/****************************************************
�� �� ����uart_init()
��		�ܣ������ж�1��ʼ��
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ������0��ʼ������
��		ע��
*****************************************************/
void uart_init(){
   PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xF3;		//�趨��ʱ��ֵ
	TH1 = 0xF3;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
 
    REN = 1; //����������޷���������
    ES = 1;  //���������ж�ʹ��
    EA = 1;
    TI = 0;
    RI = 0;       //�����жϿ���,���ò�ѯ��ʱ���ô��ж�   
} 

/****************************************************
�� �� ����uart_send_byte()
��		�ܣ�����һ���ֽڵ����ݣ��β�d��Ϊ����������
��ڲ�����unsigned char d
�� �� ֵ����
Ӧ�÷�Χ�������շ�
��		ע��
*****************************************************/
void uart_send_byte(unsigned char d){
	ES = 0;
	SBUF = d;
	while(!TI);
	TI = 0;   
	ES = 1;
}

/****************************************************
�� �� ����uart_send_string()
��		�ܣ�����һ���ַ���
��ڲ�����unsigned char *str
�� �� ֵ����
Ӧ�÷�Χ�������շ�
��		ע��
*****************************************************/
void uart_send_string(unsigned char *str){
 	while(*str){
	 	uart_send_byte(*str++);	
	}
}

/****************************************************
�� �� ����uart(void) interrupt 4
��		�ܣ������շ��ж�
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ�������շ�
��		ע��
*****************************************************/
void uart(void) interrupt 4 {		 //���ڷ����ж�
    if(RI){    //�յ�����		
				getData = SBUF;
				if(Usart_flag && Usart_Data_count < Usart_Data_length - 1){//ѭ������
					Usart_Data_count ++;  
					Usart_Data[Usart_Data_count] = getData;
				}
				if(Usart_flag && Usart_Data_count == Usart_Data_length - 1){//�������flag��0
					Usart_flag = 0;
					if((unsigned int)(Usart_Data[Usart_Data_length - 1]) != Usart_Data_length){//У��
						Usart_error = 1;
					}else{
						Usart_error = 0;
						uart_send_string(Usart_Data);
					}
				}
				if(getData == 0x5A){//�յ�֡ͷ��ʼ
					Usart_flag = 1;
					Usart_Data_count = 0;  
					Usart_Data[Usart_Data_count] = getData;
				}
        RI=0;   //���ж�����
    }else{        //������һ�ֽ�����
       TI=0;
		}
}