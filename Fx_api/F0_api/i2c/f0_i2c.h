#ifndef __F0_I2C_H
#define __F0_I2C_H

typedef struct
{
    uint32_t    sda_port_periph_clk;
    uint32_t    scl_port_periph_clk;
    GPIO_TypeDef *p_sda_gpio_port;
    GPIO_TypeDef *p_scl_gpio_port;
    uint16_t    sda_gpio_pin;
    uint16_t    scl_gpio_pin;
} f0_virt_i2c_t;


typedef void (*i2c_transfer_timeout_handler_t) (void);

void i2c_transfer_timeout_handler_register(i2c_transfer_timeout_handler_t handler);

void i2c1_res_init(void);

void i2c1_res_deinit(void);

int i2c1_write_one_byte(uint8_t slaver_addr, uint8_t reg, uint8_t data);

int i2c1_read_one_byte(uint8_t slaver_addr, uint8_t reg, uint8_t *p_data);


void f0_virt_i2c_init(f0_virt_i2c_t *p_virt_i2c);
void f0_virt_i2c_start(f0_virt_i2c_t *p_virt_i2c);
void f0_virt_i2c_stop(f0_virt_i2c_t *p_virt_i2c);
int f0_virt_i2c_wait_ack(f0_virt_i2c_t *p_virt_i2c);
void f0_virt_i2c_set_ack(f0_virt_i2c_t *p_virt_i2c);
void f0_virt_i2c_set_nack(f0_virt_i2c_t *p_virt_i2c);
void f0_virt_i2c_send_byte(f0_virt_i2c_t *p_virt_i2c, uint8_t byte);
uint8_t f0_virt_i2c_read_byte(f0_virt_i2c_t *p_virt_i2c, bool ack);

#endif

