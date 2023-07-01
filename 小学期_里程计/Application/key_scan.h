#ifndef __KEY_SCAN_H__
#define __KEY_SCAN_H__
sbit key = P3^2;
sbit key2 = P2^4;
sbit key3 = P2^5;
sbit key4 = P2^6;
sbit key5 = P2^7;
extern unsigned int display_mode,display_last_mode,cw_ccw_flag;
extern int speed_choose;
void single_key_scan(void);
#endif