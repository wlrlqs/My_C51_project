#include <rtx51tny.h>
#include <reg52.h>
#include <stdio.h>
#include "main.h"
#include "motor.h"
void State_checking(void) _task_ 0 {
	EA = 0;//��ʼ��
	EX0 = 1; 
	IT0 = 1;
	uart_init();
	OLED_Init();
	EA = 1;
	LED += 0xFE;
	os_create_task (1);//��������
	//os_create_task (2);
	os_create_task (3);
   while (1){
		 oled();
   }
}
void LED_CTRL (void) _task_ 1 {
    while(1){
			if(display_mode == 3){
				motor_control();
			}else{
				led();
			}
    }
}
 
void USART_LED_CTRL (void) _task_ 2 {
    while (1){
			if(display_mode == 3){
				motor_control();
			}else{
				os_wait2(K_TMO,50);
			}
    }
}
 
void MODE_CTRL (void) _task_ 3 {
	while(1){
		os_wait2(K_TMO,250);//��������Ƶ��
		motor_speed_choose();
		if(key2==0){    //�����ţ��������Ϊ�͵�ƽ�������if
        
            os_wait2(K_TMO,10); //��ʱ10ms����
            if(key2==0){   //�ٴ��жϰ����Ƿ��£���ֹ���ţ���ǿ�ȶ�
							if(display_mode == 1){
								display_mode = 2;
							}else if(display_mode == 4){
								display_mode = 5;
							}else if(display_mode == 6){
								display_mode = 5;
							}else if(display_mode == 3){
								speed_choose ++;
							if(speed_choose > 3){
								speed_choose = 0;
							}
							}
							while(key2==0);//�ȴ������ɿ�����ֹ����ִ��
            }
        }
		if(key3==0){    //�����ţ��������Ϊ�͵�ƽ�������if
        
            os_wait2(K_TMO,10); //��ʱ10ms����
            if(key3==0){   //�ٴ��жϰ����Ƿ��£���ֹ���ţ���ǿ�ȶ�
							if(display_mode == 3){
								display_mode = 1;
							}else if(display_mode == 1){
								display_mode = 5;
							}
                 while(key3==0);//�ȴ������ɿ�����ֹ����ִ��
            }
    }
		if(key4==0){    //�����ţ��������Ϊ�͵�ƽ�������if
            os_wait2(K_TMO,10); //��ʱ10ms����
            if(key4==0){   //�ٴ��жϰ����Ƿ��£���ֹ���ţ���ǿ�ȶ�
							while(key4==0);//�ȴ������ɿ�����ֹ����ִ��
            }
    }
		if(key5==0){    //�����ţ��������Ϊ�͵�ƽ�������if
            os_wait2(K_TMO,10); //��ʱ10ms����
            if(key5==0){   //�ٴ��жϰ����Ƿ��£���ֹ���ţ���ǿ�ȶ�
							while(key5==0);//�ȴ������ɿ�����ֹ����ִ��
            }
    }
	}
}
 

