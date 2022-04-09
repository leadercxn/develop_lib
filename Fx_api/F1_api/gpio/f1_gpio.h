#ifndef __F1_GPIO_H
#define __F1_GPIO_H

#ifdef HK32F103
#include "stm32f10x.h"
#endif

#include "stdbool.h"


void conf_gpio_output(uint32_t ahbperiph, GPIO_TypeDef *gpio_port, uint32_t gpio_pin);

void set_gpio_value(GPIO_TypeDef *gpio_port , uint32_t gpio_pin ,uint8_t value);

void conf_gpio_input(uint32_t ahbperiph, GPIO_TypeDef *gpio_port, uint32_t gpio_pin);

void get_gpio_value(GPIO_TypeDef *gpio_port , uint32_t gpio_pin ,uint8_t *p_value);

void conf_whole_gpios_output(uint32_t ahbperiph, GPIO_TypeDef *gpio_port, uint16_t gpio_pins);

void set_halt_gpios_value(GPIO_TypeDef *gpio_port ,uint8_t value, bool lsb);

void conf_whole_gpios_input(uint32_t ahbperiph, GPIO_TypeDef *gpio_port, uint16_t gpio_pins);

void get_halt_gpios_value(GPIO_TypeDef *gpio_port ,bool lsb, uint8_t *p_value);

void output_shift_to_input(GPIO_TypeDef *gpio_port, uint16_t gpio_pin);

void input_shift_to_output(GPIO_TypeDef *gpio_port, uint16_t gpio_pin);

void conf_gpio_af(uint32_t ahbperiph, GPIO_TypeDef *gpio_port, uint16_t gpio_pins);

void conf_whole_gpios_af(uint32_t ahbperiph, GPIO_TypeDef *gpio_port, uint16_t gpio_pins);

#endif
