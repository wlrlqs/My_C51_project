#ifndef TIMER1_H
#define TIMER1_H

#include <reg52.h>

typedef void (*TIMER1_CallBackType)(void);

void timer1_start(int ms, TIMER1_CallBackType AppCBFun);


#endif
