#include "mid_tftlcd.h"
#include "FONT.H"



/**
 * @brief  LCD参数设置
 * 
 * @param[i] p_drv          驱动设备结构体 
 * 
 * @retval 
 */
int LCD_Param_Setting(driver_info_t *p_drv)
{
    p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setxcmd);
    p_drv->write_data(p_drv->dev, 0);
    p_drv->write_data(p_drv->dev, 0);
    p_drv->write_data(p_drv->dev, ((p_drv->lcd_param->width - 1) >> 8));
    p_drv->write_data(p_drv->dev, ((p_drv->lcd_param->width - 1) & 0xff));

    p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setycmd);
    p_drv->write_data(p_drv->dev, 0);
    p_drv->write_data(p_drv->dev, 0);
    p_drv->write_data(p_drv->dev, ((p_drv->lcd_param->height - 1) >> 8));
    p_drv->write_data(p_drv->dev, ((p_drv->lcd_param->height - 1) & 0xff));

    return 0;
}

/**
 * @brief  设置光标坐标
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (xpos, ypos)   光标坐标
 * 
 * @retval null
 */
int LCD_SetCursor(driver_info_t *p_drv, uint16_t xpos, uint16_t ypos)
{
    p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setxcmd);
    p_drv->write_data(p_drv->dev, (xpos >> 8));
    p_drv->write_data(p_drv->dev, (xpos & 0xff));
    p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setycmd);
    p_drv->write_data(p_drv->dev, (ypos >> 8));
    p_drv->write_data(p_drv->dev, (ypos & 0xff));

    return 0;
}

/**
 * @brief   写命令到GRAM，使能更改画笔颜色，每次只改一个像素
 *          若更改多个像素颜色，只需要发送一次命令，不能发送多次，
 *          因为每发送一次，都会重新定位到光标位置
 * 
 * @param[i] p_drv          驱动设备结构体 
 * 
 * @retval null
 */
int LCD_WriteRAM_Prepare(driver_info_t *p_drv)
{
    p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->wramcmd);
    
    return 0;
}

/**
 * @brief   更改画笔颜色，紧随LCD_WriteRAM_Prepare使用
 * 
 * @param[i] p_drv          驱动设备结构体 
 * 
 * @retval null
 */
int LCD_WriteRAM(driver_info_t *p_drv, uint16_t color)
{
    p_drv->write_data(p_drv->dev, (color >> 8));
    p_drv->write_data(p_drv->dev, (color & 0xff));

    return 0;
}


/**
 * @brief  画点函数
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (x, y)         点坐标
 * @param[i] color          目标点颜色
 * 
 * @retval no reutrn
 */
int LCD_DrawPoint(driver_info_t *p_drv, uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SetCursor(p_drv, x, y);
    LCD_WriteRAM_Prepare(p_drv);
    LCD_WriteRAM(p_drv, color);

    return 0;
}

/**
 * @brief  清屏
 * 
 * @param[i] p_drv          驱动设备结构体 
 * 
 * @retval no reutrn
 */
int LCD_Clear(driver_info_t *p_drv)
{
    lcd_dev_t *lcddev = p_drv->lcd_param;
    uint8_t buf[640];       // TODO: 此处固定大小并不妥，可以通过开辟内存的方式分配空间

    LCD_SetCursor(p_drv, 0x00, 0x00);
    LCD_WriteRAM_Prepare(p_drv);

    for (uint16_t y = 0; y < lcddev->height; y++)
    {
        for (uint16_t x = 0; x < lcddev->width*2; x += 2)
        {
            buf[x] = (p_drv->background_color >> 8);
            buf[x+1] = (p_drv->background_color & 0xff);
        }
        p_drv->write_burst_data(p_drv->dev, buf, lcddev->width * 2);
    }

    return 0;
}

/**
 * @brief           显示字符
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (x, y)         字符坐标
 * @param[i] num            待显示字符
 * @param[i] size           字体大小，暂只支持12/16/24
 * @param[i] mode           是否叠加(1:是， 0:否)
 * 
 * @retval no return
 */
int LCD_ShowChar(driver_info_t *p_drv, uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{  							  
    uint8_t temp, t1, t;
	uint16_t y0 = y;

    //得到字体一个字符对应点阵集所占的字节数	
	uint8_t csize = (size/8 + ((size % 8) ? 1:0))*(size / 2);	
    lcd_dev_t *lcddev = p_drv->lcd_param;
    
    //得到偏移后的值(ASCII字库是从空格开始取模，所以-' '就是对应字符的字库)
    num = num - ' ';       
    
	for (t = 0; t < csize; t++)
	{   
		if (size == 12)
        {
            temp = asc2_1206[num][t]; 	 	//调用1206字体
        }
		else if (size == 16)
        {
            temp = asc2_1608[num][t];	        //调用1608字体
        }
		else if (size == 24)
        {
            temp = asc2_2412[num][t];	        //调用2412字体
        }
		else 
        {
            return -1;                            //没有的字库
        }

		for (t1 = 0; t1 < 8; t1++)
		{			    
			if (temp & 0x80)
            {
                LCD_DrawPoint(p_drv, x, y, p_drv->point_color);
            }
			else if (mode == 0)
            {
                LCD_DrawPoint(p_drv, x, y, p_drv->background_color);
            }

			temp <<= 1;
			y++;
			if (y >= lcddev->height)
            {
                return -1;		//超区域
            }

			if ((y - y0) == size)
			{
				y = y0;
				x++;
				if (x >= lcddev->width)
                {
                    return -1;	//超区域
                }
				break;
			}
		}  	 
	} 
    return 0; 	    	   	 	  
} 

/**
 * @brief  字符串显示
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (x, y)         字符串起始坐标
 * @param[i] width          显示窗口宽度
 * @param[i] height         显示窗口高度
 * @param[i] size           字体大小
 * @param[i] *p             字符串起始地址
 * 
 * @retval no return
 */  
int LCD_ShowString(driver_info_t *p_drv, uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0 = x;
	width += x;
	height += y;

    while ((*p <= '~') && (*p>=' '))    //判断是不是非法字符!
    {       
        if (x >= width)
        {
            x = x0;
            y += size;
        }
        if (y >= height)
        {
            break;                      //退出
        }
        LCD_ShowChar(p_drv, x, y, *p, size, 1);
        x += size / 2;
        p++;
    }  
    return 0;
}

uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result = 1;	 
	while (n--)
    {
        result*=m; 
    }   
	return result;
}	

/**
 * @brief  显示数字
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (x, y)         数字起始坐标
 * @param[i] num            待显数字
 * @param[i] len            数字位数
 * @param[i] size           数值
 * 
 * @retval no return
 */
int LCD_ShowNum(driver_info_t *p_drv, uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow = 0;		

	for (t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len-t-1)) % 10;
		if ((enshow == 0) && (t < (len - 1)))
		{
			if (temp == 0)
			{
				LCD_ShowChar(p_drv, (x + (size / 2) * t), y, ' ', size, 0);
				continue;
			}
            else 
            {
                enshow=1; 
            }
		}
        LCD_ShowChar(p_drv, (x + (size/2) * t), y, (temp + '0'), size, 0); 
	}

    return 0;
} 

/**
 * @brief  填充指定区域指定颜色
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (sx,sy)        矩形坐标1
 * @param[i] (ex,ey)        矩形坐标2
 * @param[i] color          颜色
 * 
 * @retval no return
 */
int LCD_Fill(driver_info_t *p_drv, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{          
	uint16_t i;
	uint16_t xlen=0;
    uint8_t buf[320*2];

    for (i = 0; i < (p_drv->lcd_param->width)*2; i += 2)
    {
        buf[i] = (color >> 8);
        buf[i+1] = (color & 0xff);
    }

    xlen = ex - sx + 1;	 
    for (i = sy; i <= ey; i++)
    {
        LCD_SetCursor(p_drv, sx, i);      				//设置光标位置 
        LCD_WriteRAM_Prepare(p_drv);     			//开始写入GRAM	
        p_drv->write_burst_data(p_drv->dev, buf, xlen*2); 
    }	 

    return 0;
} 

/**
 * @brief  画线粗为1像素的线
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (x1, y1)       起点坐标
 * @param[i] (x2, y2)       终点坐标
 * @param[i] color          颜色
 * 
 * @retval no return
 */
int LCD_DrawUnitLine(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1;                          //计算坐标增量 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 

	if (delta_x > 0)
    {
        incx=1;                                 //设置单步方向 
    }
	else if (delta_x == 0)
    {
        incx = 0;//垂直线 
    }
	else 
    {
        incx = -1;
        delta_x = -delta_x;
    } 

	if(delta_y > 0)
    {
        incy = 1; 
    }
	else if (delta_y == 0)
    {
        incy = 0;                     //水平线 
    }
	else
    {
        incy = -1;
        delta_y = -delta_y;
    } 

	if (delta_x > delta_y)
    {
        distance = delta_x;             //选取基本增量坐标轴 
    }
	else 
    {
        distance = delta_y; 
    }

	for (t = 0; t <= distance + 1; t++)             //画线输出 
	{  
		LCD_DrawPoint(p_drv, uRow, uCol, color);//画点 
		xerr += delta_x; 
		yerr += delta_y; 

		if (xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if (yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  

    return 0;
} 

/**
 * @brief  画粗线
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (x1, y1)       端点1
 * @param[i] (x2, y2)       端点2
 * @param[i] width          宽度
 * @param[i] color          颜色
 * 
 * @retval [description] 
 */
int LCD_DrawLine(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t width, uint16_t color)
{
    uint16_t delta_x = x2-x1;
    uint16_t delta_y = y2-y1;

    for (uint8_t i = 0; i < width; i++)
    {
        if (delta_y == 0)
        {
            LCD_DrawUnitLine(p_drv, x1, y1+i, x2, y2+i, color);
        }
        else if (delta_x == 0)
        {
            LCD_DrawUnitLine(p_drv, x1+i, y1, x2+i, y2, color);
        }
        else
        {
            // TODO: 斜线增粗有点缺陷
            LCD_DrawUnitLine(p_drv, x1, y1+i, x2, y2+i, color);
        }
    }

    return 0;
}

/**
 * @brief  画矩形
 * 
 * @param[i] p_drv          驱动设备结构体 
 * @param[i] (x1,y1)        矩形坐标1
 * @param[i] (x2,y2)        矩形对角坐标2
 * @param[i] width          宽度
 * @param[i] color          边框颜色
 * 
 * @retval no return
 */
int LCD_DrawRectangle(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t width, uint16_t color)
{
    if (((x1 + width) > (x2 - width)) || (y1 + width) > (y2 - width))
    {
        return -1;
    }

    for (uint8_t i = 0; i < width; i++)
    {
        LCD_DrawLine(p_drv, x1, y1+i, x2, y1+i, 1, color);
        LCD_DrawLine(p_drv, x1+i, y1, x1+i, y2, 1, color);
        LCD_DrawLine(p_drv, x1, y2-i, x2, y2-i, 1, color);
        LCD_DrawLine(p_drv, x2-i, y1, x2-i, y2, 1, color);
    }

    return 0;
}

/**
 * @brief  按百分比块填充
 * 
 * @param[i] p_drv              驱动设备结构体 
 * @param[i] (x1, y1)(x2, y2)   块的对角坐标
 * @param[i] color              填充块颜色
 * @param[i] rate               填充百分比
 * @param[i] coord_x_end        保留上次的横轴终止坐标
 * 
 * @retval [description] 
 */
int LCD_FillRectanglePercent(driver_info_t *p_drv, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t rate, uint16_t *coord_x_end)
{
    uint16_t x_end = x1 + (x2-x1)*rate/100;

    *coord_x_end = (*coord_x_end > x1) ? *coord_x_end : x1;

    if (x_end == RECT_X_START)
    {
        x_end += RECT_LINE_WIDTH;
    }

    if (x_end == RECT_X_END)
    {
        x_end -= RECT_LINE_WIDTH;
    }

    rate = ((rate > 100) ? 100 : rate);

    // (x_new x_old)对这个区间进行填充/清屏操作
    if (x_end < *coord_x_end)
    {
        LCD_Fill(p_drv, x_end, y1+RECT_LINE_WIDTH, *coord_x_end, y2-RECT_LINE_WIDTH, BLACK);
    }
    else
    {
        LCD_Fill(p_drv, *coord_x_end, y1+RECT_LINE_WIDTH, x_end, y2-RECT_LINE_WIDTH, color);
    }

    *coord_x_end = x_end;
    return 0;
}

/**
 * @brief  画直角三角形， 仅限直角在右下方种类
 * 
 * @param[i] p_drv      驱动设备结构体 
 * @param[i] (x, y)     底角坐标
 * @param[i] base       底边长度
 * @param[i] height     高
 * @param[i] width     边的宽度
 * @param[i] color      颜色
 * 
 * @retval [description] 
 */
int LCD_DrawTriangle(driver_info_t *p_drv, uint16_t x, uint16_t y, uint16_t base, uint16_t height, uint8_t width, uint16_t color)
{
    // 三个坐标(x, y), (x+base, y), (x+base, y-height)
    lcd_dev_t *lcddev = p_drv->lcd_param;
    if ((x + base > lcddev->width) || (y-height < 0) || (x > lcddev->width) || (y > lcddev->height))
    {
        return -1;
    }

    for (uint8_t i = 0; i < width; i++)
    {
        LCD_DrawLine(p_drv, x, y+i, (x+base), y+i, 1, color);
        LCD_DrawLine(p_drv, (x+base-i), y, (x+base-i), (y-height), 1, color);
        LCD_DrawLine(p_drv, x+i, y+i, (x+base), (y-height+i), 1, color);
    }
    LCD_DrawLine(p_drv, x, y, (x+base), (y-height), width, color);

    return 0;
}

/**
 * @brief  按百分比填充三角形
 * 
 * @param[i] p_drv      驱动设备结构体 
 * @param[i] (x, y)     底角坐标
 * @param[i] base       底边长度
 * @param[i] height     高
 * @param[i] rate       填充百分比
 * @param[i] color      颜色
 * 
 * @retval [description] 
 */
int LCD_FillTriangle(driver_info_t *p_drv,  uint16_t x, uint16_t y, uint16_t base, uint16_t height, uint8_t rate, uint16_t color)
{
    // 斜边的表达式
    int16_t k = -1 * height * 10 / base;
    uint32_t b = y - k * x /10;
    uint16_t x_end = base * rate / 100;
    
    for (uint16_t i = 0; i < x_end; i++)
    {
        LCD_DrawLine(p_drv, x+i, y, x+i, (k*(x+i)/10+b), 1, color);
    }

    return 0;
}

/**
 * @brief  显示bmp图片
 * 
 * @param[i] p_drv      驱动设备结构体
 * @param[i] (x, y)     bmp的左上角坐标
 * @param[i] size       bmp数组大小
 * @param[i] height     bmp图片高度
 * @param[i] *bmp       bmp数据
 * @param[i] color      bmp图片显示颜色
 * 
 * @retval int 
 */
int LCD_ShowBMP(driver_info_t *p_drv,  uint16_t x, uint16_t y, uint16_t size, uint16_t height, const unsigned char *bmp, uint16_t color)
{
    uint16_t y0 = y;
    uint16_t i, k = 0;
    uint8_t tmp;
    uint16_t tmp_color = p_drv->point_color;
    lcd_dev_t *lcddev = p_drv->lcd_param;
    p_drv->point_color = color;

    for (i = 0; i < size; i++)
    {
        tmp = bmp[i];

        for (k = 0; k < 8; k++)
        {
            if (tmp & 0x80)
            {
                LCD_DrawPoint(p_drv, x, y, p_drv->point_color);
            }
            
            tmp <<= 1;
            y++;
            if (y > lcddev->height)
            {
                return -1;
            }

            if ((y - y0) >= height)
            {
                x++;
                y = y0;
                if (x > lcddev->width)
                {
                    return -1;
                }
                break;
            }
        }
    }
    p_drv->point_color = tmp_color;

    return 0;
}
