#ifndef TIMER2_H
#define TIMER2_H

#include <reg52.h>


sfr T2MOD = 0xC9;

typedef void (*TIMER_CallBackType)(void);

static TIMER_CallBackType TIMER_CallBackTable[3];

void timer2_start(int ms, TIMER_CallBackType AppCBFun);
void timer2_stop();

#endif
