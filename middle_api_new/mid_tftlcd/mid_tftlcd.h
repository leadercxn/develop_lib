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

// LCD parameter
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


// LCD Driver config
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


typedef struct driver_info
{
    lcd_drv_t* dev;
    lcd_dev_t* lcd_param;
    int (*init)(struct driver_info *p_drv);
    int (*write_cmd)(lcd_drv_t *dev, uint8_t cmd);
    int (*write_data)(lcd_drv_t *dev, uint8_t cmd);
    int (*write_burst_data)(lcd_drv_t *dev, uint8_t* buff, uint16_t count);
    uint16_t point_color;
    uint16_t background_color;
}driver_info_t;


// 图形相关结构体
typedef struct coordinate
{
    uint16_t x;
    uint16_t y;
}coordinate_t;

typedef struct point
{
    coordinate_t    coord;
    uint16_t        color;
}point_info_t;

typedef struct chars
{
    uint8_t         num;        // 待显字符
    uint8_t         size;       // 字体大小
    uint8_t         mode;       // 是否叠加显示
    coordinate_t    coord;      // 字符坐标
}chars_info_t;

typedef struct string
{
    char            *str;       // 待显字符串
    uint8_t         size;       // 字体大小
    uint16_t        width;      // 显示窗口宽
    uint16_t        height;     // 显示窗口高
    coordinate_t    coord;      // 字符坐标
}string_info_t;

typedef struct number
{
    uint32_t         num;         // 待显字符
    uint8_t          size;        // 字体大小
    uint8_t          len;         // 显示长度
    coordinate_t     coord;       // 字符坐标
}number_info_t;

typedef struct fill_area
{
    coordinate_t coord_s;         // 左上角坐标
    coordinate_t coord_e;         // 右下角坐标
    uint16_t     color;
    uint8_t      width;
}fill_area_info_t;

typedef fill_area_info_t line_info_t;           // 线段信息结构体
typedef fill_area_info_t rect_info_t;           // 矩形信息结构体

typedef struct rect_dynamic
{
    rect_info_t *rect;
    uint8_t     rate;           // 条形框占比(左边为起点)
    uint16_t    end_x;          // 记录上次显示的右边坐标
}rect_dynamic_info_t;

typedef struct triangle
{
    coordinate_t coord;
    uint16_t base;
    uint16_t height;
    uint8_t width;
    uint16_t color;
}triangle_info_t;               // 三角形信息

typedef struct triangle_fill
{
    triangle_info_t *triangle;
    uint8_t rate;
}triangle_fill_info_t;               // 三角形信息

typedef struct bmp_info
{
    coordinate_t coord;
    uint16_t    size;      // bmp图片数组大小
    uint16_t    height;    // bmp图片高度
    uint16_t    color;     // 图片颜色
    const unsigned char *bmp;      // bmp图片数组      
}bmp_info_t;


int LCD_Param_Setting(driver_info_t *p_drv);
int LCD_Clear(driver_info_t *p_drv);
int LCD_DrawPoint(driver_info_t *p_drv, point_info_t point);
int LCD_ShowChar(driver_info_t *p_drv, chars_info_t chars);
int LCD_ShowString(driver_info_t *p_drv, string_info_t string);
int LCD_ShowNum(driver_info_t *p_drv, number_info_t number);
int LCD_Fill(driver_info_t *p_drv, fill_area_info_t fill_area);
int LCD_DrawUnitLine(driver_info_t *p_drv, line_info_t line);
int LCD_DrawLine(driver_info_t *p_drv, line_info_t line);
int LCD_DrawRectangle(driver_info_t *p_drv, rect_info_t rect);
int LCD_FillRectanglePercent(driver_info_t *p_drv, rect_dynamic_info_t *p_rect);
int LCD_DrawTriangle(driver_info_t *p_drv, triangle_info_t triangle);
int LCD_FillTriangle(driver_info_t *p_drv, triangle_fill_info_t *tri_fill);
int LCD_ShowBMP(driver_info_t *p_drv, bmp_info_t bmp_info);


#endif

