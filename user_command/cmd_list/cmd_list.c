#include "board_config.h"

static gpio_object_t m_gpio_led = 
{
	.gpio_port_periph_clk 	= RCC_APB2Periph_GPIOB,
	.p_gpio_port			= GPIOB,
	.gpio_pin				= GPIO_Pin_0,
	.gpio_dir				= GPIO_DIR_OUTPUR,
};

static usart_object_t m_usart_obj; 

int led_control(int argc, char **argv)
{
	m_usart_obj = get_usart_object();
    if (argc != 2) {
        static const char *msg = "led control command. pls enter param.\n\r";
        usart_put_string(&m_usart_obj, msg);
        return 0;
    }
    if (ntlibc_strcmp(argv[1], "on") == 0) {

        gpio_output_set(&m_gpio_led, 0);
        return 0;
    }

    if (ntlibc_strcmp(argv[1], "off") == 0) {

        gpio_output_set(&m_gpio_led, 1);
        return 0;
    }
	return -1;
}
