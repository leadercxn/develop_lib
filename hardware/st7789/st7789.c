#include "st7789.h"

int st7789_send_byte(lcd_drv_t *p_dev, uint8_t data)
{
    IS_NULL(p_dev);
    #ifndef LCD_DEBUG
    p_dev->db_port->BSRR = (data & 0xFF);
    p_dev->db_port->BRR  = (~(data) & 0xFF);

    #else
    GPIOB->BSRR &= ~(0x0f << 12);
    GPIOB->BRR  &= ~(0x0f << 12);
    GPIOC->BSRR &= ~(0x0f << 6);
    GPIOC->BRR  &= ~(0x0f << 6);

    uint8_t tmp1  = (data & 0x0f);
    uint8_t tmp2  = (~(data & 0x0f) & 0x0f);
    GPIOB->BSRR  |= ((tmp1) << 12);
    GPIOB->BRR   |= ((tmp2) << 12);

    tmp1 = ((data & 0xf0) >> 4);
    tmp2 = ((~(data & 0xf0) & 0xf0) >> 4);
    GPIOC->BSRR  |= ((tmp1) << 6);
    GPIOC->BRR   |= ((tmp2) << 6);
    #endif
	
	return 0;
}

int st7789_write_cmd(lcd_drv_t *p_dev, uint8_t cmd)
{
    IS_NULL(p_dev);

    gpio_output_set(&p_dev->cs_pin, 0);
    gpio_output_set(&p_dev->rs_pin, 0);             // rs = 0, send cmd
    gpio_output_set(&p_dev->wr_pin, 0);
    st7789_send_byte(p_dev, cmd);
    gpio_output_set(&p_dev->wr_pin, 1);
    gpio_output_set(&p_dev->cs_pin, 1);
	
	return 0;
}

int st7789_write_data(lcd_drv_t *p_dev, uint8_t data)
{
    IS_NULL(p_dev);

    gpio_output_set(&p_dev->cs_pin, 0);
    gpio_output_set(&p_dev->rs_pin, 1);             // rs = 1, send data
    gpio_output_set(&p_dev->wr_pin, 0);
    st7789_send_byte(p_dev, data);
    gpio_output_set(&p_dev->wr_pin, 1);
    gpio_output_set(&p_dev->cs_pin, 1);
	
	return 0;
}

int st7789_write_burst_data(lcd_drv_t *p_dev, uint8_t *buff, uint16_t count)
{
    IS_NULL(p_dev);

    gpio_output_set(&p_dev->cs_pin, 0);
    gpio_output_set(&p_dev->rs_pin, 1);             // rs = 1, send data

    for (uint16_t i = 0; i < count; i++)
    {
        gpio_output_set(&p_dev->wr_pin, 0);
        st7789_send_byte(p_dev, buff[i]);
        gpio_output_set(&p_dev->wr_pin, 1);
    }
    gpio_output_set(&p_dev->wr_pin, 1);
    gpio_output_set(&p_dev->cs_pin, 1);
	
	return 0;
}

int st7789_init(driver_info_t *p_drv)
{
    IS_NULL(p_drv);
    lcd_drv_t *p_dev = p_drv->dev;

    #ifdef  LCD_DEBUG
    gpio_config(&p_dev->db0);
    gpio_config(&p_dev->db1);
    gpio_config(&p_dev->db2);
    gpio_config(&p_dev->db3);
    gpio_config(&p_dev->db4);
    gpio_config(&p_dev->db5);
    gpio_config(&p_dev->db6);
    gpio_config(&p_dev->db7);

    gpio_output_set(&p_dev->db0, 0);
    gpio_output_set(&p_dev->db1, 0);
    gpio_output_set(&p_dev->db2, 0);
    gpio_output_set(&p_dev->db3, 0);
    gpio_output_set(&p_dev->db4, 0);
    gpio_output_set(&p_dev->db5, 0);
    gpio_output_set(&p_dev->db6, 0);
    gpio_output_set(&p_dev->db7, 0);
    #else

    for (uint8_t i = 0; i < 8; i++)
    {
        gpio_config(&p_dev->db[i]);
        gpio_output_set(&p_dev->db[i], 0);
    }
    #endif

    gpio_config(&p_dev->cs_pin);
    gpio_config(&p_dev->rst_pin);
    gpio_config(&p_dev->rs_pin);
    gpio_config(&p_dev->wr_pin);
    gpio_config(&p_dev->rd_pin);

    gpio_output_set(&p_dev->cs_pin, 1);
    gpio_output_set(&p_dev->rst_pin, 1);
    gpio_output_set(&p_dev->rs_pin, 1);
    gpio_output_set(&p_dev->wr_pin, 1);
    gpio_output_set(&p_dev->rd_pin, 1);


    st7789_write_cmd(p_dev, 0x36);      // 显示扫描方向
    st7789_write_data(p_dev, 0xA0);     // 00:从左到右，从上到下
                                        // A0:从下到上，从左到右
                                        // 60:从上到下，从右到左

    st7789_write_cmd(p_dev, 0x3A);    // color format
    st7789_write_data(p_dev, 0x05);    // 16bit/pixel

    st7789_write_cmd(p_dev, 0xB2);    // Porch Setting  
    st7789_write_data(p_dev, 0x0C);   
    st7789_write_data(p_dev, 0x0C);   
    st7789_write_data(p_dev, 0x00);   
    st7789_write_data(p_dev, 0x33);   
    st7789_write_data(p_dev, 0x33);   

    st7789_write_cmd(p_dev, 0xB7);    // Gate Control
    st7789_write_data(p_dev, 0x35);   

    st7789_write_cmd(p_dev, 0xBB);    // VCOM  
    st7789_write_data(p_dev, 0x1C);   

    st7789_write_cmd(p_dev, 0xC0);    // LCM control   
    st7789_write_data(p_dev, 0x2C);   

    st7789_write_cmd(p_dev, 0xC2);    // VDV and VRH Command Enable 
    st7789_write_data(p_dev, 0x01);   

    st7789_write_cmd(p_dev, 0xC3);    //  VRH Set  
    st7789_write_data(p_dev, 0x0B);       

    st7789_write_cmd(p_dev, 0xC4);    // VDV Set
    st7789_write_data(p_dev, 0x20);   

    st7789_write_cmd(p_dev, 0xC6);    // Frame Rate    
    st7789_write_data(p_dev, 0x0F);   

    st7789_write_cmd(p_dev, 0xD0);    // Power Control 1
    st7789_write_data(p_dev, 0xA4);   
    st7789_write_data(p_dev, 0xA1);   

    st7789_write_cmd(p_dev, 0xE0);    // Positive Voltage     
    st7789_write_data(p_dev, 0xD0);   
    st7789_write_data(p_dev, 0x06);   
    st7789_write_data(p_dev, 0x0B);   
    st7789_write_data(p_dev, 0x0A);   
    st7789_write_data(p_dev, 0x09);   
    st7789_write_data(p_dev, 0x06);   
    st7789_write_data(p_dev, 0x2F);   
    st7789_write_data(p_dev, 0x44);   
    st7789_write_data(p_dev, 0x45);   
    st7789_write_data(p_dev, 0x18);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x27);   
    st7789_write_data(p_dev, 0x2D);   

    st7789_write_cmd(p_dev, 0xE1);    // Negative Voltage    
    st7789_write_data(p_dev, 0xD0);   
    st7789_write_data(p_dev, 0x06);   
    st7789_write_data(p_dev, 0x0B);   
    st7789_write_data(p_dev, 0x0A);   
    st7789_write_data(p_dev, 0x09);   
    st7789_write_data(p_dev, 0x05);   
    st7789_write_data(p_dev, 0x2E);   
    st7789_write_data(p_dev, 0x43);   
    st7789_write_data(p_dev, 0x45);   
    st7789_write_data(p_dev, 0x18);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x14);   
    st7789_write_data(p_dev, 0x27);   
    st7789_write_data(p_dev, 0x2D);   

    st7789_write_cmd(p_dev, 0x21);    // Display Inversion On
    st7789_write_cmd(p_dev, 0x11);    // Sleep Out
    delay_ms(5);
    st7789_write_cmd(p_dev, 0x29);    // Display On

    LCD_Param_Setting(p_drv);
    LCD_Clear(p_drv);
	
	return 0;
}




