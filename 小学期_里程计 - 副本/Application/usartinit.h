#ifndef __USARTINIT_H__
#define __USARTINIT_H__

void uart_init();
void uart_send_byte(unsigned char d);
void uart_send_string(unsigned char *str);
#endif