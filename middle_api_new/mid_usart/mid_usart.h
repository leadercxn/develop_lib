#ifndef __MID_USART_H
#define __MID_USART_H

#ifdef F0
#include "f0_usart.h"
#elif defined F1
#include "f1_usart.h"
#endif

typedef struct
{
#ifdef F0
    f0_uart_info_t *uart_info;
    f0_uart_config_t *uart_config;
#elif defined F1
    f1_uart_info_t *uart_info;
    f1_uart_config_t *uart_config;
#endif
}usart_object_t;

int usart_init(usart_object_t *p_usart_object);

int usart_put_char(usart_object_t *p_usart_object, char ch);

int usart_put_string(usart_object_t *p_usart_object, const char *s);

uint16_t usart_get_char(usart_object_t *p_usart_object);


#endif
