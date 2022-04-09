#ifndef _MID_TFTLCD_H
#define _MID_TFTLCD_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "mid_gpio.h"

// 外设位宽设置
#define CONFIG_FSMC_BUSWIDTH_8B
// #define CONFIG_FSMC_BUSWIDTH_16B

// LCD控制方式
// #define CONFIG_CTRL_GPIO
#define CONFIG_CTRL_FSMC


// FSMC相关设置
// 此处只针对F1系列而言，其FSMC控制4个BANK，其中BANK1是控制SRAM，BANK1有4个扇区，每个扇区64M
#define FSMC_BASE_ADDR          0x60000000
#define BANK1_SECTOR1_OFFSET    0x00000000
#define BANK1_SECTOR2_OFFSET    0x04000000
#define BANK1_SECTOR3_OFFSET    0x08000000
#define BANK1_SECTOR4_OFFSET    0x0C000000

// 此处是针对8b的外设RAM，当RS接到对应的Ax，需要设置的offset值
#define BANK_8B_A0_OFFSET       0x0000
#define BANK_8B_A1_OFFSET       0x0001
#define BANK_8B_A2_OFFSET       0x0003
#define BANK_8B_A3_OFFSET       0x0007
#define BANK_8B_A4_OFFSET       0x000F
#define BANK_8B_A5_OFFSET       0x001F
#define BANK_8B_A6_OFFSET       0x003F
#define BANK_8B_A7_OFFSET       0x007F
#define BANK_8B_A8_OFFSET       0x00FF
#define BANK_8B_A9_OFFSET       0x01FF
#define BANK_8B_A10_OFFSET      0x03FF
#define BANK_8B_A11_OFFSET      0x07FF
#define BANK_8B_A12_OFFSET      0x0FFF
#define BANK_8B_A13_OFFSET      0x1FFF
#define BANK_8B_A14_OFFSET      0x3FFF
#define BANK_8B_A15_OFFSET      0x7FFF
#define BANK_8B_A16_OFFSET      0xFFFF

// 此处是针对16b的外设RAM，当RS接到对应的Ax，需要设置的offset值
#define BANK_16B_A0_OFFSET       0x0000
#define BANK_16B_A1_OFFSET       0x0002
#define BANK_16B_A2_OFFSET       0x0006
#define BANK_16B_A3_OFFSET       0x000E
#define BANK_16B_A4_OFFSET       0x001E
#define BANK_16B_A5_OFFSET       0x003E
#define BANK_16B_A6_OFFSET       0x007E
#define BANK_16B_A7_OFFSET       0x00FE
#define BANK_16B_A8_OFFSET       0x01FE
#define BANK_16B_A9_OFFSET       0x03FE
#define BANK_16B_A10_OFFSET      0x07FE
#define BANK_16B_A11_OFFSET      0x0FFE
#define BANK_16B_A12_OFFSET      0x1FFE
#define BANK_16B_A13_OFFSET      0x3FFE
#define BANK_16B_A14_OFFSET      0x7FFE
#define BANK_16B_A15_OFFSET      0xFFFE
#define BANK_16B_A16_OFFSET      0x1FFFE

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	


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
    uint16_t    dir;            // 横屏(1)/竖屏(0)
	uint16_t    wramcmd;		//开始写gram指令
	uint16_t    setxcmd;		//设置x坐标指令
	uint16_t    setycmd;		//设置y坐标指令 
}lcd_dev_t;


// LCD 地址结构体，只对FSMC有用
typedef struct
{
	uint8_t lcd_cmd;
	uint8_t lcd_data;
} LCD_Type_8b_t;

typedef struct
{
	uint16_t lcd_cmd;
	uint16_t lcd_data;
} LCD_Type_16b_t;

#define LCD_BASE        ((uint32_t)(0x60000000 | 0x00FFFF))
#define LCD             ((LCD_Type_8b_t *) LCD_BASE)


// gpio驱动方式
typedef struct
{
    gpio_object_t   cs_pin;
    gpio_object_t   rst_pin;
    gpio_object_t   rs_pin;
    gpio_object_t   wr_pin;
    gpio_object_t   rd_pin;

    #ifndef LCD_DEBUG
    gpio_object_t   db[8];
    GPIO_TypeDef *  db_port;         // 设计中需要使用同一个端口(Px0~Px8)
    #else
    gpio_object_t   gpio_port1;
    gpio_object_t   gpio_port2;
    #endif

}ctrl_type_gpio_t;                  // GPIO控制

// fsmc驱动方式
// TODO: 不同的MCU的FSMC分布不一样，若更换平台，需要对此处进行扩充
typedef struct 
{
    gpio_object_t   rst_pin;
    gpio_object_t   back_light;
    #ifdef HK32F103
    gpio_object_t   fsmc_pin1;      // FSMC port D
    gpio_object_t   fsmc_pin2;      // FSMC port E
    #endif
}ctrl_type_fsmc_t;                  // FSMC控制


// LCD Driver config
typedef struct 
{
    #ifdef F0
    ctrl_type_gpio_t ctrl_gpio;

    #elif defined F1
    ctrl_type_fsmc_t ctrl_fsmc;
    uint32_t    lcd_base_addr;
    #ifdef CONFIG_FSMC_BUSWIDTH_16B
    LCD_Type_16b_t *lcd_addr;
    #elif defined CONFIG_FSMC_BUSWIDTH_8B
    LCD_Type_8b_t *lcd_addr;
    #endif

    #endif
}lcd_drv_t;


typedef struct driver_info
{
    lcd_drv_t *dev;
    lcd_dev_t *lcd_param;
    int (*init)(struct driver_info *p_drv);
    int (*write_cmd)(lcd_drv_t *dev, uint16_t cmd);
    int (*write_data)(lcd_drv_t *dev, uint16_t data);
    int (*write_reg)(lcd_drv_t *dev, uint16_t cmd, uint16_t data);
    int (*write_burst_data)(lcd_drv_t *dev, uint8_t* buff, uint16_t count);
    int (*read_data)(lcd_drv_t *dev);
    int (*read_reg)(lcd_drv_t *dev, uint16_t cmd);
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
int LCD_Scan_Dir(driver_info_t *p_drv);
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

