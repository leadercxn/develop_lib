#ifndef _MID_TFTLCD_H
#define _MID_TFTLCD_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "mid_gpio.h"
// #include "ft32f0xx.h"


#define RECT_X_START    120         // 动态显示框左边坐标
#define RECT_X_END      275         // 动态显示框右边坐标
#define RECT_LINE_WIDTH 3           // 动态显示边框宽度


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40     //棕色
#define BRRED 			 0XFC07     //棕红色
#define GRAY  			 0X8430     //灰色
#define DARKBLUE      	 0X01CF	    //深蓝色
#define LIGHTBLUE      	 0X7D7C	    //浅蓝色  
#define GRAYBLUE       	 0X5458     //灰蓝色
#define LIGHTGREEN     	 0X841F     //浅绿色
#define LIGHTGRAY        0XEF5B     //浅灰色(PANNEL)
#define LGRAY 			 0XC618     //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651     //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12     //浅棕蓝色(选择条目的反色)

typedef struct 
{
    uint16_t    width;          // LCD宽
    uint16_t    height;         // LCD长
    uint16_t    id;             // LCD ID
    uint16_t    dir;            // 横屏/竖屏
	uint16_t    wramcmd;		//开始写gram指令
	uint16_t    setxcmd;		//设置x坐标指令
	uint16_t    setycmd;		//设置y坐标指令 
}lcd_dev_t;


typedef struct 
{
    gpio_object_t   cs_pin;
    gpio_object_t   rst_pin;
    gpio_object_t   rs_pin;
    gpio_object_t   wr_pin;
    gpio_object_t   rd_pin;

    #ifdef PARALLEL_PORT
    
    #ifndef LCD_DEBUG
    gpio_object_t   db[8];
    GPIO_TypeDef *  db_port;         // 设计中需要使用同一个端口(Px0~Px8)
    #else
    gpio_object_t   db0;
    gpio_object_t   db1;
    gpio_object_t   db2;
    gpio_object_t   db3;
    gpio_object_t   db4;
    gpio_object_t   db5;
    gpio_object_t   db6;
    gpio_object_t   db7;
    #endif

    #elif   SERIAL_IIC
    gpio_object_t   iic_scl;
    gpio_object_t   iic_sda;

    #elif   SERIAL_SPI
    gpio_object_t   spi_cs;
    gpio_object_t   spi_clk;
    gpio_object_t   spi_in;
    gpio_object_t   spi_out;
    #endif

    #ifdef LED_EN
    gpio_object_t   led_en;
    #endif

    #ifdef LED_PWM
    gpio_object_t   led_pwm;
    #endif
}lcd_drv_t;



typedef struct 
{
    lcd_drv_t* dev;
    lcd_dev_t* lcd_param;
    int (*write_cmd)(lcd_drv_t *p_drv, uint8_t cmd);
    int (*write_data)(lcd_drv_t *p_drv, uint8_t cmd);
    int (*write_burst_data)(lcd_drv_t *p_drv, uint8_t* buff, uint16_t count);
    uint16_t point_color;
    uint16_t background_color;
}driver_info_t;


int LCD_Param_Setting(driver_info_t *p_drv);
int LCD_DrawPoint(driver_info_t *p_drv, uint16_t x, uint16_t y, uint16_t color);
int LCD_Clear(driver_info_t *p_drv);
int LCD_ShowChar(driver_info_t *p_drv, uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
int LCD_ShowString(driver_info_t *p_drv, uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);
int LCD_ShowNum(driver_info_t *p_drv, uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
int LCD_DrawUnitLine(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
int LCD_DrawLine(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t width, uint16_t color);
int LCD_Fill(driver_info_t *p_drv, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
int LCD_DrawRectangle(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t width, uint16_t color);
int LCD_FillRectanglePercent(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t rate, uint16_t *coord_x_end);
int LCD_DrawTriangle(driver_info_t *p_drv, uint16_t x, uint16_t y, uint16_t base, uint16_t height, uint8_t width, uint16_t color);
int LCD_FillTriangle(driver_info_t *p_drv,  uint16_t x, uint16_t y, uint16_t base, uint16_t height, uint8_t rate, uint16_t color);
int LCD_ShowBMP(driver_info_t *p_drv,  uint16_t x, uint16_t y, uint16_t size, uint16_t height, const unsigned char *bmp, uint16_t color);


#endif

