#include <rtx51tny.h>
#include <reg52.h>
#include <stdio.h>
#include "key_scan.h"

/****************************************************
�� �� ����single_key_scan
��		�ܣ�������ɨ��
��ڲ�������
�� �� ֵ����
Ӧ�÷�Χ������ɨ������
��		ע��
*****************************************************/
void single_key_scan(void){
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
							if(display_mode == 3){
								cw_ccw_flag = ~cw_ccw_flag;
							}
							while(key4==0);//�ȴ������ɿ�����ֹ����ִ��
            }
    }
//		if(key5==0){    //�����ţ��������Ϊ�͵�ƽ�������if
//            os_wait2(K_TMO,10); //��ʱ10ms����
//            if(key5==0){   //�ٴ��жϰ����Ƿ��£���ֹ���ţ���ǿ�ȶ�
//							while(key5==0);//�ȴ������ɿ�����ֹ����ִ��
//            }
//    }
}