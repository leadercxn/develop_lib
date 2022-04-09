#ifndef __F1_USART_H
#define __F1_USART_H

#include "stdbool.h"

#ifdef HK32F103
#include "stm32f10x.h"
#endif

/**
 * @enum f1_uart_id_t
 * @brief ID supported by UART.
 */
typedef enum 
{
    F1_UART_1 = 0, ///< UART 1.
    F1_UART_2,     ///< UART 2.
    F1_UART_3,     ///< UART 3.
    F1_UART_4,     ///< UART 4.
    F1_UART_5,     ///< UART 5.
    F1_UART_MAX
}f1_uart_id_t;

typedef struct
{
    USART_TypeDef *uart;

    GPIO_TypeDef *tx_port;

    GPIO_TypeDef *rx_port;

    uint32_t clk;
    uint32_t tx_clk;
    uint32_t rx_clk;

    uint16_t tx_pin;
    uint16_t rx_pin;

    f1_uart_id_t uart_id;
} f1_uart_info_t;

/**
 * @brief Structure for UART configuration.
 * 
 */
typedef struct
{
    // void *p_context; ///< Context passed to interrupt handler.
    uint32_t baudrate; ///< Baudrate configuration.
    uint32_t databits; ///< Data bits for one character configuration.
    uint32_t stopbit;  ///< Stopbit configuration.
    uint32_t parity;   ///< Parity configuration.
    uint32_t hwfc;     ///< Flow control configuration.
    uint32_t mode;
    uint8_t interrupt_priority; ///< Interrupt priority.
} f1_uart_config_t;

void f1_uart_init(f1_uart_info_t uart_info, f1_uart_config_t const *p_config);

void f1_uart_put(uint8_t uart_id, char ch);

uint16_t f1_uart_get(uint8_t uart_id);

#endif
