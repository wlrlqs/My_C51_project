#ifndef STEPPER_MOTOR_5P4L_H
#define STEPPER_MOTOR_5P4L_H

//USE TIMER2

#define STEPPER_PIN P2

#include <reg52.h>
#include "timer2.h"

#define ROT1_SEC2008 1
#define POSITIVE     0
#define INVERTION    1

void stepper_init(unsigned char rot_per_sec, unsigned char flag_pos_inv_tmp );

#endif