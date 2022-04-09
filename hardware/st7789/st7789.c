#include "st7789.h"

int st7789_send_byte(lcd_drv_t *p_dev, uint8_t data)
{
    IS_NULL(p_dev);
#if defined (CONFIG_CTRL_GPIO)
    #ifndef LCD_DEBUG
    p_dev->ctrl_gpio.db_port->BSRR = (data & 0xFF);
    p_dev->ctrl_gpio.db_port->BRR  = (~(data) & 0xFF);

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
#endif
	
	return 0;
}

int st7789_write_cmd(lcd_drv_t *p_dev, uint16_t cmd)
{
    IS_NULL(p_dev);

    #if defined (CONFIG_CTRL_GPIO)
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 0);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 0);             // rs = 0, send cmd
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 0);
    st7789_send_byte(p_dev, (uint8_t)cmd);
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);

    #elif defined (CONFIG_CTRL_FSMC)
    p_dev->lcd_addr->lcd_cmd = (uint8_t)cmd;
    #endif
	
	return 0;
}

int st7789_write_data(lcd_drv_t *p_dev, uint16_t data)
{
    IS_NULL(p_dev);

    #if defined (CONFIG_CTRL_GPIO)
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 0);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 1);             // rs = 1, send data
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 0);
    st7789_send_byte(p_dev, (uint8_t)data);
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);
	
    #elif defined (CONFIG_CTRL_FSMC)
    p_dev->lcd_addr->lcd_data = (uint8_t)data;
    #endif

	return 0;
}

uint8_t st7789_read_data(lcd_drv_t *p_dev)
{
    uint8_t data;
    #if defined (CONFIG_CTRL_FSMC)
    data = p_dev->lcd_addr->lcd_data;

    #elif defined (CONFIG_CTRL_GPIO)

    #endif
    return data;
}


int st7789_write_burst_data(lcd_drv_t *p_dev, uint8_t *buff, uint16_t count)
{
    IS_NULL(p_dev);

	#if defined (CONFIG_CTRL_GPIO)
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 0);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 1);             // rs = 1, send data

    for (uint16_t i = 0; i < count; i++)
    {
        gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 0);
        st7789_send_byte(p_dev, buff[i]);
        gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    }
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);
	#endif
	
	return 0;
}

int st7789_init(driver_info_t *p_drv)
{
    // 1. 初始化管脚
    // 1.1 GPIO控制方式

    IS_NULL(p_drv);
    lcd_drv_t *p_dev = p_drv->dev;

#if defined (CONFIG_CTRL_GPIO)
    #ifdef  LCD_DEBUG
    gpio_config(&p_dev->ctrl_gpio.gpio_port1);
    gpio_config(&p_dev->ctrl_gpio.gpio_port2);

    gpio_output_set(&p_dev->ctrl_gpio.gpio_port1, 0);
    gpio_output_set(&p_dev->ctrl_gpio.gpio_port2, 0);
    #else

    for (uint8_t i = 0; i < 8; i++)
    {
        gpio_config(&p_dev->ctrl_gpio.db[i]);
        gpio_output_set(&p_dev->ctrl_gpio.db[i], 0);
    }
    #endif

    gpio_config(&p_dev->ctrl_gpio.cs_pin);
    gpio_config(&p_dev->ctrl_gpio.rst_pin);
    gpio_config(&p_dev->ctrl_gpio.rs_pin);
    gpio_config(&p_dev->ctrl_gpio.wr_pin);
    gpio_config(&p_dev->ctrl_gpio.rd_pin);

    gpio_output_set(&p_dev->ctrl_gpio.cs_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.rst_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.rs_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.wr_pin, 1);
    gpio_output_set(&p_dev->ctrl_gpio.rd_pin, 1);

#elif defined (CONFIG_CTRL_FSMC)

    // 1.2 FSMC控制方式
    gpio_config(&p_dev->ctrl_fsmc.rst_pin);
    gpio_config(&p_dev->ctrl_fsmc.back_light);
    gpio_config(&p_dev->ctrl_fsmc.fsmc_pin1);
    gpio_config(&p_dev->ctrl_fsmc.fsmc_pin2);

    fsmc_conig(0, BUS_WIDTH_8b);

    p_dev->lcd_base_addr = ((uint32_t)((FSMC_BASE_ADDR + BANK1_SECTOR1_OFFSET) | BANK_8B_A16_OFFSET));
    p_dev->lcd_addr = ((LCD_Type_8b_t *)(p_dev->lcd_base_addr));


	gpio_output_set(&p_dev->ctrl_fsmc.rst_pin, 0);
    delay_ms(1000);
    gpio_output_set(&p_dev->ctrl_fsmc.rst_pin, 1);
#endif

    delay_ms(10);                       // 此延时不能忽略，因为FSMC速度快，芯片复位之后立马发送命令，可能会导致初始化不完全

    // read id
    st7789_write_cmd(p_dev, 0XDA);				   
    p_drv->lcd_param->id =st7789_read_data(p_dev);	
    p_drv->lcd_param->id =st7789_read_data(p_dev);
    
    if (p_drv->lcd_param->id == 0x85)
    {
        st7789_write_cmd(p_dev, 0XDB);				   
        p_drv->lcd_param->id =st7789_read_data(p_dev);	
        p_drv->lcd_param->id =st7789_read_data(p_dev);

        if (p_drv->lcd_param->id == 0x85)
        {
            st7789_write_cmd(p_dev, 0XDC);				   
            p_drv->lcd_param->id =st7789_read_data(p_dev);	
            p_drv->lcd_param->id =st7789_read_data(p_dev);

            if (p_drv->lcd_param->id == 0x52)
            {
                p_drv->lcd_param->id = 0x7789;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }




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




