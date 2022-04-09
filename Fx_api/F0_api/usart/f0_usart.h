#ifndef __F0_USART_H
#define __F0_USART_H

#include "stdbool.h"

#ifdef FT32
#include "ft32f0xx.h"
#endif 

#ifdef HK32F0
#include "hk32f0xx.h"
#endif

/**
 * @enum f0_uart_id_t
 * @brief ID supported by UART.
 */
enum f0_uart_id_t
{
    F0_UART_1 = 0, ///< USART1.
    F0_UART_2,     ///< USART2.
    F0_UART_3,     ///< UART 3.
    F0_UART_4,     ///< UART 4.
    F0_UART_5,     ///< UART 5.
    F0_UART_6,     ///< UART 6.
    F0_UART_7,     ///< UART 7.
    F0_UART_8,     ///< UART 8.
    F0_UART_MAX
};

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

    uint16_t tx_pin_source;
    uint16_t rx_pin_source;

    uint16_t tx_af;
    uint16_t rx_af;

    uint8_t uart_id;
} f0_uart_info_t;

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
} f0_uart_config_t;

void f0_uart_init(f0_uart_info_t uart_info, f0_uart_config_t const *p_config);

void f0_uart_put(uint8_t uart_id, char ch);

uint16_t f0_uart_get(uint8_t uart_id);

#endif
