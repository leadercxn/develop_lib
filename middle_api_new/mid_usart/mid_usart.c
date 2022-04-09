#include "mid_usart.h"
#include "board_config.h"

int usart_init(usart_object_t *p_usart_object)
{
    if (!p_usart_object)
    {
        return -EINVAL;
    }

    int err_code = 0;

#ifdef F0
    f0_uart_init(*(p_usart_object->uart_info), p_usart_object->uart_config);
#elif F1
    f1_uart_init(*(p_usart_object->uart_info), p_usart_object->uart_config);
#endif

    return err_code;
}

int usart_put_char(usart_object_t *p_usart_object, char ch)
{
    if (!p_usart_object)
    {
        return -EINVAL;
    }

    int err_code = 0;

#ifdef F0
    f0_uart_put(p_usart_object->uart_info->uart_id, ch);
#elif F1
    f1_uart_put(p_usart_object->uart_info->uart_id, ch);
#endif

    return err_code;
}

int usart_put_string(usart_object_t *p_usart_object, const char *s)
{
    if (!p_usart_object)
    {
        return -EINVAL;
    }

    int err_code = 0;

    while (*s != '\0')
    {
    #ifdef F0
        f0_uart_put(p_usart_object->uart_info->uart_id, *s++);
    #elif defined F1
        f1_uart_put(p_usart_object->uart_info->uart_id, *s++);
    #endif
    }

    return err_code;
}

uint16_t usart_get_char(usart_object_t *p_usart_object)
{
    if (!p_usart_object)
    {
        return -EINVAL;
    }

    int err_code = 0;
    uint16_t data;

#ifdef F0
    data = f0_uart_get(p_usart_object->uart_info->uart_id);
#elif defined F1
    data = f1_uart_get(p_usart_object->uart_info->uart_id);
#endif

    return data;
}



