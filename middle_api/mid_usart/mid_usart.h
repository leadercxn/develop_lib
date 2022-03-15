#ifndef __MID_USART_H
#define __MID_USART_H

#ifdef FT32
#include "ft_usart.h"
#endif

typedef struct
{
    uint8_t id;

    ft_uart_info_t *uart_info;

    ft_uart_config_t *uart_config;
}usart_object_t;

int usart_init(usart_object_t *p_usart_object);

int usart_put_char(usart_object_t *p_usart_object, char ch);

int usart_put_string(usart_object_t *p_usart_object, const char *s);

uint16_t usart_get_char(usart_object_t *p_usart_object);


#endif
