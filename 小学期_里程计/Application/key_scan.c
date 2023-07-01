#include <rtx51tny.h>
#include <reg52.h>
#include <stdio.h>
#include "key_scan.h"

/****************************************************
函 数 名：single_key_scan
功		能：单按键扫描
入口参数：无
返 回 值：无
应用范围：按键扫描任务
备		注：
*****************************************************/
void single_key_scan(void){
	if(key2==0){    //读引脚，如果引脚为低电平，则进入if
        
            os_wait2(K_TMO,10); //延时10ms消抖
            if(key2==0){   //再次判断按键是否按下，防止干扰，增强稳定
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
							while(key2==0);//等待按键松开，防止往下执行
            }
        }
		if(key3==0){    //读引脚，如果引脚为低电平，则进入if
        
            os_wait2(K_TMO,10); //延时10ms消抖
            if(key3==0){   //再次判断按键是否按下，防止干扰，增强稳定
							if(display_mode == 3){
								display_mode = 1;
							}else if(display_mode == 1){
								display_mode = 5;
							}
                 while(key3==0);//等待按键松开，防止往下执行
            }
    }
		if(key4==0){    //读引脚，如果引脚为低电平，则进入if
            os_wait2(K_TMO,10); //延时10ms消抖
            if(key4==0){   //再次判断按键是否按下，防止干扰，增强稳定
							if(display_mode == 3){
								cw_ccw_flag = ~cw_ccw_flag;
							}
							while(key4==0);//等待按键松开，防止往下执行
            }
    }
//		if(key5==0){    //读引脚，如果引脚为低电平，则进入if
//            os_wait2(K_TMO,10); //延时10ms消抖
//            if(key5==0){   //再次判断按键是否按下，防止干扰，增强稳定
//							while(key5==0);//等待按键松开，防止往下执行
//            }
//    }
}