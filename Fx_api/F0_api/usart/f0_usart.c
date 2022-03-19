#include "f0_usart.h"
#include "util.h"
#include "stdbool.h"

#ifdef FT32
#include "ft32f0xx.h"
#endif 

#ifdef HK32F0
#include "hk32f0xx.h"
#endif

/**
  * @brief  Configures COM port.
  * @param  uart_id: Specifies the COM port to be configured.
  *          This parameter can be one of following parameters:    
  *            @arg COM1
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *         contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void f0_uart_init(f0_uart_info_t uart_info, f0_uart_config_t const *p_config)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_InitTypeDef USART_InitStruct;

    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(uart_info.tx_clk | uart_info.rx_clk, ENABLE);

    /* Enable USART clock 不同串口不同的外设时钟 */
    //RCC_APB1PeriphClockCmd(uart_info.clk, ENABLE);

    RCC_APB2PeriphClockCmd(uart_info.clk, ENABLE);

    /* Connect PXx to USARTx_Tx */
    GPIO_PinAFConfig(uart_info.tx_port, uart_info.tx_pin_source, uart_info.tx_af);

    /* Connect PXx to USARTx_Rx */
    GPIO_PinAFConfig(uart_info.rx_port, uart_info.rx_pin_source, uart_info.rx_af);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = uart_info.tx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(uart_info.tx_port, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = uart_info.rx_pin;
    GPIO_Init(uart_info.rx_port, &GPIO_InitStructure);

    /* USART configuration */
    USART_InitStruct.USART_BaudRate = p_config->baudrate;
    USART_InitStruct.USART_WordLength = p_config->databits;
    USART_InitStruct.USART_StopBits = p_config->stopbit;
    USART_InitStruct.USART_Parity = p_config->parity;
    USART_InitStruct.USART_Mode = p_config->mode;
    USART_InitStruct.USART_HardwareFlowControl = p_config->hwfc;

    USART_Init(uart_info.uart, &USART_InitStruct);

    /* Enable USART */
    USART_Cmd(uart_info.uart, ENABLE);
}

void f0_uart_put(uint8_t uart_id, char ch)
{

    if (F0_UART_1 == uart_id)
    {
        /* Loop until transmit data register is empty */
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        {
        }

        /* Place your implementation of fputc here */
        /* e.g. write a character to the USART */
        USART_SendData(USART2, (uint8_t)ch);
        return;
    }


    /* Loop until transmit data register is empty */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t)ch);
}

uint16_t f0_uart_get(uint8_t uart_id)
{
    uint16_t data;

    if (F0_UART_1 == uart_id)
    {
        /* Loop until receive data register is not empty */
        while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        {
        }

        /* Place your implementation of fgetc here */
        /* e.g. write a character to the USART */
        data = USART_ReceiveData(USART2);
        return data;
    }

    /* Loop until receive data register is not empty */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {
    }

    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    data = USART_ReceiveData(USART1);
    return data;
}

