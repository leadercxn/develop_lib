#ifndef __NT35510_H
#define __NT35510_H

#include "board_config.h"
#include "mid_tftlcd.h"


int nt35510_write_cmd(lcd_drv_t *p_dev, uint16_t cmd);
int nt35510_write_data(lcd_drv_t *p_dev, uint16_t data);
int nt35510_write_burst_data(lcd_drv_t *p_dev, uint16_t *buff, uint16_t count);
int nt35510_write_reg(lcd_drv_t *p_dev, uint16_t cmd, uint16_t data);
int nt35510_read_reg(lcd_drv_t *p_dev, uint16_t cmd);
int nt35510_read_data(lcd_drv_t *p_dev);
int nt35510_init(driver_info_t *p_drv);



#endif 

