#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H
void Uart_init();
void Uart_send(unsigned char a);
void Uart_str(unsigned char *s);
void Timer0Init();
#endif