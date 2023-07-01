#ifndef _HCSR_H_
#define _HCSR_H_

sbit Trig= P1^4; //产生脉冲引脚
sbit Echo= P1^5; //回波引脚
void  StartModule(void);
void Conut(void);

#endif