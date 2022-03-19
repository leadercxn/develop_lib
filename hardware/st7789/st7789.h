/*
  *@Description: 
  *@Author: cxt
  *@Date: 2022-02-17 20:53:38
 * @LastEditTime: 2022-02-18 23:31:15
 * @LastEditors: cxt
  *@Reference: 
 */
#ifndef __ST7789_H
#define __ST7789_H

#include "stdbool.h"
#include "board_config.h"
#include "mid_tftlcd.h"

#define CMD_WRAM    0x2C
#define CMD_SETX    0x2A
#define CMD_SETY    0x2B

// #define LED_EN
// #define LED_PWM


int st7789_send_byte(lcd_drv_t *p_dev, uint8_t data);
int st7789_write_cmd(lcd_drv_t *p_dev, uint8_t cmd);
int st7789_write_data(lcd_drv_t *p_dev, uint8_t data);
int st7789_write_burst_data(lcd_drv_t *p_dev, uint8_t *buff, uint16_t count);
int st7789_init(driver_info_t *p_drv);


#endif
