#include "f1_usart.h"
#include "util.h"
#include "stdbool.h"

#ifdef FT32
#include "ft32f0xx.h"
#endif 

#ifdef HK32F0
#include "hk32f0xx.h"
#endif


static USART_TypeDef *m_usart_array[F1_UART_MAX] = 
{USART1, USART2, USART3, UART4, UART5};

/**
  * @brief  Configures COM port.
  * @param  uart_id: Specifies the COM port to be configured.
  *          This parameter can be one of following parameters:    
  *            @arg COM1
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *         contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void f1_uart_init(f1_uart_info_t uart_info, f1_uart_config_t const *p_config)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_InitTypeDef USART_InitStruct;

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(uart_info.tx_clk | uart_info.rx_clk, ENABLE);

    /* Enable USART clock 不同串口不同的外设时钟 */
    if (F1_UART_1 == uart_info.uart_id)
    {
        RCC_APB2PeriphClockCmd(uart_info.clk, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(uart_info.clk, ENABLE);
    }

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = uart_info.tx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(uart_info.tx_port, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = uart_info.rx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(uart_info.rx_port, &GPIO_InitStructure);

    /* USART configuration */
    USART_InitStruct.USART_BaudRate = p_config->baudrate;
    USART_InitStruct.USART_WordLength = p_config->databits;
    USART_InitStruct.USART_StopBits = p_config->stopbit;
    USART_InitStruct.USART_Parity = p_config->parity;
    USART_InitStruct.USART_Mode = p_config->mode;
    USART_InitStruct.USART_HardwareFlowControl = p_config->hwfc;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    USART_Init(uart_info.uart, &USART_InitStruct);
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
    /* Enable USART */
    USART_Cmd(uart_info.uart, ENABLE);
}

void f1_uart_put(uint8_t uart_id, char ch)
{
    /* Loop until transmit data register is empty */
    while (USART_GetFlagStatus(m_usart_array[uart_id], USART_FLAG_TXE) == RESET)
    {
    }

    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(m_usart_array[uart_id], (uint8_t)ch);
}

uint16_t f1_uart_get(uint8_t uart_id)
{
    uint16_t data;

    /* Loop until receive data register is not empty */
    while (USART_GetFlagStatus(m_usart_array[uart_id], USART_FLAG_RXNE) == RESET)
    {
    }

    /* Place your implementation of fgetc here */
    /* e.g. write a character to the USART */
    data = USART_ReceiveData(m_usart_array[uart_id]);
    return data;
}

/**
 * @brief  串口1中断服务程序
 * 
 * @retval [description] 
 */
void USART1_IRQHandler(void)                
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // Res =USART_ReceiveData(USART1);	//读取接收到的数据
    }
}
