#include <reg52.h>
#include "timer2.h"

#include "stepper_motor_5L4P.h"
#include "timer1.h"


sbit LED_RUN = P3^5;     //����ָʾ��
sbit BEEP = P3^0;        //������
sbit ir_fill = P2^7;     //����Ͱ��������⴫����
sbit ir_hand = P2^6;     //���ּ��
sbit ir_led = P1^0;      //����Ͱ״ָ̬ʾ��
sbit key_hand = P1^1;   //�ֶ��򿪹ر�����Ͱ����

unsigned char flag_ir_hand_check = 0;            //�������־λ
unsigned char flag_trush_state_open_cloase = 0;   //����Ͱ״̬
unsigned char flag_m_open_close = 0;   //�ֶ��򿪻��߹ر�
unsigned char flag_fill = 0;
unsigned char key_flag = 0;
unsigned char jpg_flag = 2;

void DelayMs(unsigned int i);
void SerialInit();
void SendByte(unsigned char sbyte);
void SendString(unsigned char *pstr);
void welcome_ui();
void open_ui();
void opened_ui();
void close_ui();
void closed_ui();
void jpg_choose();
void delay(unsigned int i){  //��ʱ����
	while(i--);
}

void trash_open(){           //������Ͱ
	stepper_init(ROT1_SEC2008*3, POSITIVE );
}

void trash_close(){         //�ر�����Ͱ
	stepper_init(ROT1_SEC2008*3, INVERTION ); 
}

unsigned char time_conuter = 0;  //����Ͱ�ر�ʱ���׼
void time_conter(){           //��ʱ���жϺ���
	time_conuter++;
	if(time_conuter > 200) time_conuter = 100;
	LED_RUN = ~LED_RUN;
	if(flag_fill == 1){
		BEEP = !BEEP;
	}else{
		BEEP = 1;
	}
	
}


void main(){
	SerialInit();
	delay(50000);
	SendString("DIR(1);\r\n");
	delay(8000);
	SendString("DIR(1);\r\n");
	delay(8000);
	SendString("CLR(0);\r\n");
	DelayMs(2000);
	welcome_ui();
	timer2_start( 1000, time_conter);  //��ʱ����ʼ��
	BEEP = 0;   //��������ʼ��
	ir_led = 0;  //����Ͱ״̬�Ƴ�ʼ��
	//delay(50000);  //��ʱ
	//delay(50000);
	//delay(50000);
	delay(50000);
	while(1){
			//trash_open();
		ir_fill = 1;
//		if(ir_fill == 0){    //����ͰͰ�����
//			delay(500);
//			if(ir_fill == 0){
//				flag_fill = 1;
//				flag_ir_hand_check = 0;
//			}
//		}else{
//			flag_fill = 0;
//			ir_led = 0;
//		}
//		if(flag_fill ==1)
//		{
//			//open_ui();
//			//trash_open();
//			//opened_ui();
//			while(flag_fill ==1)
//			{
//				BEEP = !BEEP;
//				LED_RUN = ~LED_RUN;
//				DelayMs(500);
//				LED_RUN = ~LED_RUN;
//				if(ir_fill == 0){    //����ͰͰ�����
//			delay(500);
//			if(ir_fill == 0){
//				flag_fill = 1;
//				flag_ir_hand_check = 0;
//			}
//		}else{
//			flag_fill = 0;
//			ir_led = 0;
//		}
//			}
//			BEEP = 1;
//			DelayMs(1000);
//			//close_ui();
//			//trash_close();	
//			//closed_ui();
//			DelayMs(500);
//			//welcome_ui();
//		
//		}
//		else
//		{
//			BEEP = 1;
//		}
		ir_hand = 1;
		//flag_ir_hand_check=0;
		if(ir_hand == 0){  //���ּ��
			delay(500);
			if(ir_hand == 0){
				flag_ir_hand_check=1;
				//while(ir_hand == 0);
				time_conuter = 0;
			}
		}else{
			if(key_flag==0){
				flag_ir_hand_check=0;
			}
		}
		if(ir_fill == 0){  //Ͱ�����
			delay(500);
			if(ir_fill == 0){
				flag_ir_hand_check=1;
				//while(ir_hand == 0);
				time_conuter = 0;
			}
		}
				//�ֶ��򿪹ر�����Ͱ���
		key_hand = 1;
		if(key_hand == 0){
			delay(500);
			if(key_hand == 0){
				flag_ir_hand_check=1;
				flag_m_open_close = 1;
				time_conuter = 0;
				key_flag = ~key_flag;
				while(key_hand == 0);
			}
		}
		//����Ͱ���ж�
		if( (flag_fill == 0 && flag_trush_state_open_cloase==0 && flag_ir_hand_check == 1) || (flag_fill == 1 && flag_trush_state_open_cloase==0 && flag_m_open_close != 0) ){
			flag_trush_state_open_cloase=1;
			trash_open();
			open_ui();
			delay(8000);
			opened_ui();
			delay(8000);
			ir_led = 0;
			DelayMs(800);
			welcome_ui();
			flag_m_open_close = 0;
		}
		
		//����Ͱ�رտ���
		if(flag_trush_state_open_cloase==1&&flag_ir_hand_check==0)
		{
			DelayMs(2000);
			trash_close();
			close_ui();
			delay(8000);
			closed_ui();
			DelayMs(1000);
			welcome_ui();
			ir_led = 1;
			flag_trush_state_open_cloase=0;
		}
		ir_led = 0;
		

		
	}
	
}
void jpg_choose(){
	switch(jpg_flag){
		case 0:
			SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 1:
			SendString("FSIMG(2138112,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 2:
			SendString("FSIMG(2179072,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 3:
			SendString("FSIMG(2220032,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 4:
			SendString("FSIMG(2260992,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 5:
			SendString("FSIMG(2301952,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 6:
			SendString("FSIMG(2342912,0,0,160,128,0);\r\n");
		jpg_flag ++;
		break;
		case 7:
			SendString("FSIMG(2383872,0,0,160,128,0);\r\n");
		jpg_flag = 0;
		break;
	}
}
void welcome_ui()//��ʾ��ӭui����
{
	SendString("CLR(0);\r\n");
	delay(8000); 
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000); 
//	SendString("QRCODE(8,8,https://qm.qq.com/cgi-bin/qm/qr?k=Ns796uc8EysoG11g1VVrvomLXvHEUSsf&noverify=0#,100,1);\r\n");
//	delay(40000); 
//	SendString("DC16(12,64,'QQ��Ƭ',1);\r\n");
	delay(4000); 
	SendString("DC16(76,10,'��ӭʹ��',1);\r\n");
	delay(3000); 
	SendString("DC16(68,26,'��������Ͱ',1);\r\n");
//	delay(3000); 
//	SendString("DC16(12,80,'����:2100300317',1);\r\n");
//	delay(3000); 
//	SendString("DC16(12,96,'�������꣨shen��',1);\r\n");
	delay(3000); 
	
}
void open_ui()//��ʾ����ui
{
	SendString("CLR(0);\r\n");
	delay(8000);  
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000);  
	SendString("BOX(9,105,149,115,1);\r\n");//�������߿�
	delay(800);   
	SendString("DC16(9,88,'����Ͱ�Ǵ���',1);\r\n");
	delay(800);   
	DelayMs(100);
	SendString("BOX(16,28,35,66,1);\r\n");//����Ͱ�򿪵�1��
	delay(800);  
	SendString("BOXF(9,105,25,115,1);\r\n");//������������1
	delay(800); 
    SendString("DC16(9,88,'����Ͱ�Ǵ���*',1);\r\n");
	delay(800);  	
	DelayMs(100);
	delay(800); 
	SendString("PL(51,28,51,66,1);\r\n");//����Ͱ�򿪵�2��������
	delay(800); 
	SendString("PL(70,66,70,28,1);\r\n");//����Ͱ�򿪵�2��������
	delay(800); 
	SendString("PL(70,66,51,66,1);\r\n");//����Ͱ�򿪵�2��׺���
	delay(800);     
    SendString("PL(51,28,68,23,1);\r\n");   //����Ͱ�򿪵�2�鶥����
	delay(800); 
	SendString("BOXF(9,105,45,115,1);\r\n"); //������������2
	delay(800); 
	SendString("DC16(9,88,'����Ͱ�Ǵ���*',1);\r\n");
	delay(800); 	
	DelayMs(100);
	delay(800); 
	SendString("PL(86,28,86,66,1);\r\n");//����Ͱ�򿪵�3��������
	delay(800); 
	SendString("PL(105,66,105,28,1);\r\n");//����Ͱ�򿪵�3��������
	delay(800);  
	SendString("PL(86,66,105,66,1);\r\n");//����Ͱ�򿪵�3��׺���
	delay(800);   
    SendString("PL(100,17,86,28,1);\r\n");//����Ͱ�򿪵�3�鶥����
	delay(800);  
	SendString("BOXF(9,105,75,115,1);\r\n"); //������������3
	delay(800);  
	SendString("DC16(9,88,'����Ͱ�Ǵ���**',1);\r\n");
	delay(800);  	
	DelayMs(100);  
	delay(800); 	
    SendString("PL(122,28,122,66,1);\r\n");//����Ͱ�򿪵�4��������
	delay(800); 
	SendString("PL(141,28,141,66,1);\r\n");//����Ͱ�򿪵�4��������
	delay(800);  
	SendString("PL(122,66,141,66,1);\r\n");//����Ͱ�򿪵�4��׺���
	delay(800); 
    SendString("PL(128,11,122,28,1);\r\n"); //����Ͱ�򿪵�4�鶥����
	delay(800);  
	SendString("BOXF(9,105,149,115,1);\r\n"); //������������5
	delay(800);  	
	SendString("DC16(9,88,'����Ͱ�Ǵ���***',1);\r\n");
	delay(800); 
	DelayMs(600);	
	
}
void opened_ui()
{
	SendString("CLR(0);\r\n");
	delay(40000); 
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000);
	SendString("DC16(9,88,'����Ͱ���ѳɹ���*',1);\r\n");
	delay(800);
	SendString("BOXF(9,105,149,115,1);\r\n"); //������������5
}

void close_ui()
{
	SendString("CLR(0);\r\n");
	delay(8000); 
	//SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	jpg_choose();
	delay(40000); 
	SendString("BOX(9,105,149,115,1);\r\n");//�������߿�
	delay(800); 
	SendString("DC16(9,88,'����Ͱ�ǹر���',1);\r\n");
	delay(800); 
	DelayMs(100);
	SendString("PL(16,28,16,66,1);\r\n");//����Ͱ�رյ�1��������
	delay(800);
	SendString("PL(35,66,35,28,1);\r\n");//����Ͱ�رյ�1��������
	delay(800);
	SendString("PL(16,66,35,66,1);\r\n");//����Ͱ�رյ�1��׺���
	delay(800);   
    SendString("PL(24,11,16,28,1);\r\n"); //����Ͱ�رյ�1�鶥����
	delay(800); 
	SendString("BOXF(9,105,25,115,1);\r\n");//������������1
	delay(800);
    SendString("DC16(9,88,'����Ͱ�ǹر���*',1);\r\n");
	delay(800);	
	DelayMs(100);
	SendString("PL(51,28,51,66,1);\r\n");//����Ͱ�رյ�2��������
	delay(800);
	SendString("PL(70,66,70,28,1);\r\n");//����Ͱ�رյ�2��������
	delay(800);
	SendString("PL(70,66,51,66,1);\r\n");//����Ͱ�رյ�2��׺���
	delay(800);   
    SendString("PL(66,17,51,28,1);\r\n");   //����Ͱ�رյ�2�鶥����
	delay(800);
	SendString("BOXF(9,105,45,115,1);\r\n"); //������������2
	delay(800);
	SendString("DC16(9,88,'����Ͱ�ǹر���*',1);\r\n");
	delay(800);	
	DelayMs(100);
	SendString("PL(86,28,86,66,1);\r\n");//����Ͱ�򿪵�3��������
	delay(800);
	SendString("PL(105,66,105,28,1);\r\n");//����Ͱ�򿪵�3��������
	delay(800);
	SendString("PL(86,66,105,66,1);\r\n");//����Ͱ�򿪵�3��׺���
	delay(800);   
    SendString("PL(100,23,86,28,1);\r\n");//����Ͱ�򿪵�3�鶥����
	delay(800);
	SendString("BOXF(9,105,75,115,1);\r\n"); //������������3
	delay(800);
	SendString("DC16(9,88,'����Ͱ�ǹر���**',1);\r\n");
	delay(800);	
	DelayMs(100);   
    SendString("BOX(122,28,141,66,1);\r\n");//����Ͱ�ر�4��
	delay(800);
	SendString("BOXF(9,105,149,115,1);\r\n"); //������������5
	delay(800);	
	SendString("DC16(9,88,'����Ͱ�ǹر���***',1);\r\n");
	delay(800);
	DelayMs(600);
}
void closed_ui()
{
	SendString("CLR(0);\r\n");
	delay(20000); 
	SendString("FSIMG(2097152,0,0,160,128,0);\r\n");
	delay(20000);
	SendString("DC16(9,88,'����Ͱ���ѳɹ��ر�*',1);\r\n");
	delay(800);
	SendString("BOXF(9,105,149,115,1);\r\n"); //������������5
	DelayMs(600);
}
/*
//��Ƭ��ʱ������:11.0592MHz   ��ʱ��1��Ϊ�����ʷ�����
void SerialInit(){
 TMOD=0x20;   //����T1������ʽΪ��ʽ2
 TH1=0xfd; //����ʱ����λװ��ֵ
 TL1=0xfd; //����ʱ����λװ��ֵ
 TR1=1;  //����ʱ��
             //���������ò�����
 SM0=0;  //���ô���ͨѶ��ʽΪ��ʽ1
 SM1=1;  
 REN=1;  //�����Ƿ�������ݵĿ���

 EA=1; //���жϴ�,���ò�ѯ��ʱ���ô��ж�
 ES=1; //�����жϿ���,���ò�ѯ��ʱ���ô��ж�
}
*/
//��Ƭ��ʱ������:11.0592MHz	   ��ʱ��T2��Ϊ�����ʷ�����
void SerialInit(){
    PCON &= 0x7F; //�����ʲ�����
    SCON = 0x50;  //8λ����,�ɱ䲨����
    //AUXR |= 0x40; //��ʱ��1ʱ��ΪFosc,��1T
    //AUXR &= 0xFE; //����1ѡ��ʱ��1Ϊ�����ʷ�����
    TMOD &= 0x0F; //�����ʱ��1ģʽλ
    TMOD |= 0x20; //�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
    TL1 = 0xFD;   //�趨��ʱ��ֵ
    TH1 = 0xFD;   //�趨��ʱ����װֵ
    ET1 = 0;      //��ֹ��ʱ��1�ж�
    TR1 = 1;      //������ʱ��1
 
    REN = 1; //����������޷���������
    ES = 1;  //���������ж�ʹ��
    EA = 1;
    TI = 0;
    RI = 0;       //�����жϿ���,���ò�ѯ��ʱ���ô��ж�

}


//�����жϺ���:
void SerialPortInte(void) interrupt 4 //���ô����жϷ��շ�����
{
	 unsigned char rbyte;
	 if(RI){     //RI=1,�ж�Ϊ���ڽ��յ������ݣ�RIҪ���㣬
         RI=0;
		 rbyte=SBUF; 
		 if(rbyte==0x0A){ 	  
		    SendString("");
		 }else if(rbyte==0x0D){
		 	SendString("");
		 }else{
		 	SendByte(rbyte); 
		 }
		 	
     }

}

//���ڷ���һ���ֽ�:
void SendByte(unsigned char sbyte)
{ 
     SBUF=sbyte; //��������
     while(!TI); //�ȴ��������
     TI=0; //���㷢�ͱ�־λ
}
 
//���ڷ���һ���ַ���:
void SendString(unsigned char *pstr) //����ָ��
{
     while(*pstr!='\0') //�ַ����Ƿ���
     {
         SendByte(*pstr);//�����ַ�������
         pstr++; //ָ����һ���ַ�
     }
}

void DelayMs(unsigned int i)  //��ʱi ms
{
    unsigned int j;
    while(i--)
    {
        for(j = 0; j < 125; j++);
    }
}

