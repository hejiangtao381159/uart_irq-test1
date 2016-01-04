#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

extern void uart2_init(u32 bound);
extern void uart2_putc(char c);
extern void uart2_puts(const char *str);

#endif


