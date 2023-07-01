
#include "stepper_motor_5L4P.h"

unsigned char code F_Rotation[4]={0xf1,0xf2,0xf4,0xf8};  
unsigned char code B_Rotation[4]={0xf8,0xf4,0xf2,0xf1};  


int stepper_c  = 0;

unsigned int stepper_c_conter = 0;
unsigned int MAX_STEP = 450;

unsigned char flag_pos_inv = 0;

void stepper_run(){
	if( flag_pos_inv == POSITIVE && stepper_c_conter < MAX_STEP){
		STEPPER_PIN |= 0x0f;
		STEPPER_PIN &= F_Rotation[stepper_c++]; 
		if(stepper_c >= 4){
			stepper_c = 0;
		}
		stepper_c_conter++;
	}else if( stepper_c_conter < MAX_STEP ) {
		STEPPER_PIN |= 0x0f;
		STEPPER_PIN &= B_Rotation[stepper_c++]; 
		if(stepper_c >= 4){
			stepper_c = 0;
		}
		stepper_c_conter++;
	}
	if( stepper_c_conter >= MAX_STEP ) {
		 TR2 = 0; 
	}
}


void stepper_init(unsigned char rot_per_sec, unsigned char flag_pos_inv_tmp ){
	flag_pos_inv = flag_pos_inv_tmp;
	timer2_start(rot_per_sec, stepper_run);
	stepper_c_conter = 0;
}


