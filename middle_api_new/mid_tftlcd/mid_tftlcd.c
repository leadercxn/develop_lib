#include "mid_tftlcd.h"
#include "font.h"
#include "board_config.h"


/**
 * @brief  LCD参数设置
 * 
 * @param[i] p_drv          驱动设备结构体 
 * 
 * @retval 
 */
int LCD_Param_Setting(driver_info_t *p_drv)
{
    IS_NULL(p_drv);

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
    IS_NULL(p_drv);

    if ((xpos > p_drv->lcd_param->width) || (ypos > p_drv->lcd_param->height))
    {
        return -EINVAL;
    }

    lcd_dev_t *lcd_param = p_drv->lcd_param;

    if (lcd_param->id == 0x5510)
    {
        p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setxcmd);
        p_drv->write_data(p_drv->dev, (xpos >> 8));
        p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setxcmd + 1);
        p_drv->write_data(p_drv->dev, (xpos & 0xff));
        p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setycmd);
        p_drv->write_data(p_drv->dev, (ypos >> 8));
        p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setycmd + 1);
        p_drv->write_data(p_drv->dev, (ypos & 0xff));
    }
    else
    {
        p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setxcmd);
        p_drv->write_data(p_drv->dev, (xpos >> 8));
        p_drv->write_data(p_drv->dev, (xpos & 0xff));
        p_drv->write_cmd(p_drv->dev, p_drv->lcd_param->setycmd);
        p_drv->write_data(p_drv->dev, (ypos >> 8));
        p_drv->write_data(p_drv->dev, (ypos & 0xff));
    }

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
    IS_NULL(p_drv);

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
    IS_NULL(p_drv);

    lcd_dev_t *lcd_param = p_drv->lcd_param;

    #if defined CONFIG_FSMC_BUSWIDTH_16B
        p_drv->write_data(p_drv->dev, color);
    #elif defined CONFIG_FSMC_BUSWIDTH_8B
        p_drv->write_data(p_drv->dev, (color >> 8));
        p_drv->write_data(p_drv->dev, (color & 0xff));
    #endif

    return 0;
}

/**
 * @brief   设置LCD扫描方向
 * 
 * @param[i] p_drv          驱动设备结构体 
 * 
 * @retval null
 */
int LCD_Scan_Dir(driver_info_t *p_drv)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
    uint16_t id = p_drv->lcd_param->id;
    uint16_t dir = p_drv->lcd_param->dir;
    uint16_t width = p_drv->lcd_param->width;
    uint16_t height = p_drv->lcd_param->height;
    uint16_t setxcmd = p_drv->lcd_param->setxcmd;
    uint16_t setycmd = p_drv->lcd_param->setxcmd;
    lcd_drv_t *p_dev = p_drv->dev;

    // 横屏时，对6804和1963不改变扫描方向！竖屏时1963改变方向
	if (((dir == 1) && (id != 0X6804) && (id != 0X1963))
        || ((dir == 0) && (id == 0X1963)))
	{			   
		switch (dir)//方向转换
		{
			case 0 : dir = 6; break;
			case 1 : dir = 7; break;
			case 2 : dir = 4; break;
			case 3 : dir = 5; break;
			case 4 : dir = 1; break;
			case 5 : dir = 0; break;
			case 6 : dir = 3; break;
			case 7 : dir = 2; break;	     
		}
	} 

    // 9341/6804/5310/5510/1963,特殊处理
	if (id == 0x9341 || id == 0X6804 || id == 0X5310 || id == 0X5510 || id == 0X1963)
	{
		switch (dir)
		{
            //从左到右,从上到下
			case L2R_U2D:
				regval |= (0 << 7)|(0 << 6)|(0 << 5); 
				break;

            //从左到右,从下到上
			case L2R_D2U:
				regval |= (1 << 7)|(0 << 6)|(0 << 5); 
				break;

            //从右到左,从上到下
			case R2L_U2D:
				regval |= (0 << 7)|(1 << 6)|(0 << 5); 
				break;
            
            //从右到左,从下到上
			case R2L_D2U:
				regval |= (1 << 7)|(1 << 6)|(0 << 5); 
				break;
            
            //从上到下,从左到右
			case U2D_L2R:
				regval |= (0 << 7)|(0 << 6)|(1 << 5); 
				break;
            
            //从上到下,从右到左
			case U2D_R2L:
				regval |= (0 << 7)|(1 << 6)|(1 << 5); 
				break;
            
            //从下到上,从左到右
			case D2U_L2R:
				regval |= (1 << 7)|(0 << 6)|(1 << 5); 
				break;
            
            //从下到上,从右到左
			case D2U_R2L:
				regval |= (1 << 7)|(1 << 6)|(1 << 5); 
				break;	 
		}

		if (id == 0X5510)
        {
            dirreg = 0X3600;
        }
		else
        {
            dirreg = 0X36;
        }

        if ((id !=0X5310 ) && (id != 0X5510) && (id != 0X1963))
        {
            // 5310/5510/1963不需要BGR 
            regval |= 0X08;
        }  

		if (id == 0X6804)
        {
            // 6804的BIT6和9341的反了	 
            regval |= 0x02;
        }  

		p_drv->write_reg(p_dev, dirreg, regval);

        // 1963不做坐标处理
		if (id != 0X1963)
		{
			if (regval & 0X20)
			{
                //交换X,Y
				if (width < height)
				{
					temp    = width;
					width   = height;
					height  = temp;
				}
			}
            else  
			{
                //交换X,Y
				if (width > height)
				{
					temp    = width;
					width   = height;
					height  = temp;
				}
			}  
		}

		if (id==0X5510)
		{
			p_drv->write_cmd(p_dev, setxcmd);
            p_drv->write_data(p_dev, 0); 
			p_drv->write_cmd(p_dev, setxcmd+1);
            p_drv->write_data(p_dev, 0); 
            p_drv->write_cmd(p_dev, setxcmd+2);
            p_drv->write_data(p_dev, (width - 1) >> 8);            
            p_drv->write_cmd(p_dev, setxcmd+3);
            p_drv->write_data(p_dev, (width - 1) & 0XFF);   

			p_drv->write_cmd(p_dev, setycmd);
            p_drv->write_data(p_dev, 0); 
			p_drv->write_cmd(p_dev, setycmd+1);
            p_drv->write_data(p_dev, 0); 
            p_drv->write_cmd(p_dev, setycmd+2);
            p_drv->write_data(p_dev, (height - 1) >> 8);            
            p_drv->write_cmd(p_dev, setycmd+3);
            p_drv->write_data(p_dev, (height - 1) & 0XFF);   
		}
        else
		{
			p_drv->write_cmd(p_dev, setxcmd);
            p_drv->write_data(p_dev, 0); 
            p_drv->write_data(p_dev, 0); 
            p_drv->write_data(p_dev, (width - 1) >> 8);  
            p_drv->write_data(p_dev, (width - 1) & 0XFF);   

			p_drv->write_cmd(p_dev, setycmd);
            p_drv->write_data(p_dev, 0); 
            p_drv->write_data(p_dev, 0); 
            p_drv->write_data(p_dev, (height - 1) >> 8);  
            p_drv->write_data(p_dev, (height - 1) & 0XFF);   
		}
    }
    else 
	{
		switch(dir)
		{
            //从左到右,从上到下
			case L2R_U2D:
				regval |= (1<<5)|(1<<4)|(0<<3); 
				break;

            //从左到右,从下到上
			case L2R_D2U:
				regval |= (0<<5)|(1<<4)|(0<<3); 
				break;

            //从右到左,从上到下
			case R2L_U2D:
				regval |= (1<<5)|(0<<4)|(0<<3);
				break;

            //从右到左,从下到上
			case R2L_D2U:
				regval |= (0<<5)|(0<<4)|(0<<3); 
				break;	 
            
            //从上到下,从左到右
			case U2D_L2R:
				regval |= (1<<5)|(1<<4)|(1<<3); 
				break;
            
            //从上到下,从右到左
			case U2D_R2L:
				regval |= (1<<5)|(0<<4)|(1<<3); 
				break;
            
            //从下到上,从左到右
			case D2U_L2R:
				regval |= (0<<5)|(1<<4)|(1<<3); 
				break;

            //从下到上,从右到左
			case D2U_R2L:
				regval |= (0<<5)|(0<<4)|(1<<3); 
				break;	 
		} 

		dirreg = 0X03;
		regval |= 1<<12; 
		p_drv->write_reg(p_dev, dirreg, regval);
	}

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
    IS_NULL(p_drv);

    lcd_dev_t *lcddev = p_drv->lcd_param;


    uint8_t buf[640];       // TODO: 此处固定大小并不妥，可以通过开辟内存的方式分配空间

    LCD_SetCursor(p_drv, 0x00, 0x00);
    LCD_WriteRAM_Prepare(p_drv);

#if defined (CONFIG_CTRL_GPIO)
    for (uint16_t y = 0; y < lcddev->height; y++)
    {
        for (uint16_t x = 0; x < lcddev->width*2; x += 2)
        {
            buf[x] = (p_drv->background_color >> 8);
            buf[x+1] = (p_drv->background_color & 0xff);
        }
        p_drv->write_burst_data(p_drv->dev, buf, lcddev->width * 2);
    }
#elif defined (CONFIG_CTRL_FSMC)
    uint32_t i = 0;
    uint32_t total_point = lcddev->width;
    total_point *= lcddev->height;

    for (i = 0; i < total_point; i++)
    {
        LCD_WriteRAM(p_drv, p_drv->background_color);
    }
#endif

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
int LCD_Fill(driver_info_t *p_drv, fill_area_info_t fill_area)
{
    IS_NULL(p_drv);

    if ((fill_area.coord_s.x > p_drv->lcd_param->width) 
        || (fill_area.coord_s.y > p_drv->lcd_param->height)
        || (fill_area.coord_e.x > p_drv->lcd_param->width) 
        || (fill_area.coord_e.y > p_drv->lcd_param->height)
        || (fill_area.width > p_drv->lcd_param->width))
    {
        return -EINVAL;
    }    

	uint16_t i, j;
	uint16_t xlen=0;
    uint8_t buf[320*2];

    for (i = 0; i < (p_drv->lcd_param->width)*2; i += 2)
    {
        buf[i] = (fill_area.color >> 8);
        buf[i+1] = (fill_area.color & 0xff);
    }

    xlen = fill_area.coord_e.x - fill_area.coord_s.x + 1;	 
    for (i = fill_area.coord_s.y; i <= fill_area.coord_e.y; i++)
    {
        LCD_SetCursor(p_drv, fill_area.coord_s.x, i);      				//设置光标位置 
        LCD_WriteRAM_Prepare(p_drv);     			                    //开始写入GRAM	
#if defined (CONFIG_CTRL_GPIO)
        p_drv->write_burst_data(p_drv->dev, buf, xlen*2); 
#elif defined (CONFIG_CTRL_FSMC)
        for (j = 0; j <= xlen; j++)
        {
            LCD_WriteRAM(p_drv, fill_area.color);
        }
#endif
    }	 

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
int LCD_DrawPoint(driver_info_t *p_drv, point_info_t point)
{
    IS_NULL(p_drv);

    if ((point.coord.x > p_drv->lcd_param->width) 
        || (point.coord.y > p_drv->lcd_param->height))
    {
        return -EINVAL;
    }

    LCD_SetCursor(p_drv, point.coord.x, point.coord.y);
    LCD_WriteRAM_Prepare(p_drv);
    LCD_WriteRAM(p_drv, point.color);

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
int LCD_ShowChar(driver_info_t *p_drv, chars_info_t chars)
{  			
    IS_NULL(p_drv);

    if ((chars.coord.x > p_drv->lcd_param->width) || (chars.coord.y > p_drv->lcd_param->height)
        || ((chars.mode != 0) && (chars.mode != 1)))
    {
        return -EINVAL;
    }
    
    uint8_t temp, t1, t;
    point_info_t point;     // 构建点变量
	uint16_t y0;

    //得到字体一个字符对应点阵集所占的字节数	
	uint8_t csize = (chars.size/8 + ((chars.size % 8) ? 1:0))*(chars.size / 2);	
    lcd_dev_t *lcddev = p_drv->lcd_param;
    
    y0 = chars.coord.y;
    point.coord =   chars.coord;

    //得到偏移后的值(ASCII字库是从空格开始取模，所以-' '就是对应字符的字库)
    chars.num = chars.num - ' ';       
    
	for (t = 0; t < csize; t++)
	{   
		if (chars.size == 12)
        {
            temp = asc2_1206[chars.num][t]; 	 	//调用1206字体
        }
		else if (chars.size == 16)
        {
            temp = asc2_1608[chars.num][t];	        //调用1608字体
        }
		else if (chars.size == 24)
        {
            temp = asc2_2412[chars.num][t];	        //调用2412字体
        }
		else 
        {
            return -1;                            //没有的字库
        }

		for (t1 = 0; t1 < 8; t1++)
		{			    
			if (temp & 0x80)
            {
                point.color =   p_drv->point_color;
                LCD_DrawPoint(p_drv, point);
            }
			else if (chars.mode == 0)
            {
                point.color =   p_drv->background_color;
                LCD_DrawPoint(p_drv, point);
            }

			temp <<= 1;
			point.coord.y++;
			if (point.coord.y >= lcddev->height)
            {
                return -1;		//超区域
            }

			if ((point.coord.y - y0) == chars.size)
			{
				point.coord.y = y0;
				point.coord.x++;
				if (point.coord.x >= lcddev->width)
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
int LCD_ShowString(driver_info_t *p_drv, string_info_t string)
{         
    IS_NULL(p_drv);

    if ((string.coord.x > p_drv->lcd_param->width) || (string.coord.y > p_drv->lcd_param->height)
        || (string.width > p_drv->lcd_param->width) 
        || (string.height > p_drv->lcd_param->height))
    {
        return -EINVAL;
    }

    uint8_t x0;
    chars_info_t chars;
    char *p = string.str;

	x0  = string.coord.x;
	string.width    += string.coord.x;
	string.height   += string.coord.y;
    chars.size      =   string.size;
    chars.mode      =   1;
    chars.coord.x   = string.coord.x;
    chars.coord.y   = string.coord.y;

    while ((*p <= '~') && (*p>=' '))    //判断是不是非法字符!
    {       
        if (chars.coord.x >= string.width)
        {
            chars.coord.x = x0;
            chars.coord.y += string.size;
        }
        if (chars.coord.y >= string.height)
        {
            break;                      //退出
        }
        chars.num = *p;
        LCD_ShowChar(p_drv, chars);
        chars.coord.x += string.size / 2;
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
int LCD_ShowNum(driver_info_t *p_drv, number_info_t number)
{         	
    IS_NULL(p_drv);

    if ((number.coord.x > p_drv->lcd_param->width) || (number.coord.y > p_drv->lcd_param->height))
    {
        return -EINVAL;
    }

	uint8_t t,temp;
	uint8_t enshow = 0;	
    chars_info_t chars;	

    chars.coord.y = number.coord.y;
    chars.mode    = 0;
    chars.size    = number.size;

	for (t = 0; t < number.len; t++)
	{
		temp = (number.num / LCD_Pow(10, number.len-t-1)) % 10;
        chars.coord.x = (number.coord.x + (number.size/2)*t);

		if ((enshow == 0) && (t < (number.len - 1)))
		{
			if (temp == 0)
			{
                chars.num = ' ';
                chars.coord.x = (number.coord.x + (number.size/2)*t);
				LCD_ShowChar(p_drv, chars);
				continue;
			}
            else 
            {
                enshow=1; 
            }
		}

        chars.num = temp + '0';
        LCD_ShowChar(p_drv, chars); 
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
int LCD_DrawUnitLine(driver_info_t *p_drv, line_info_t line)
{
    IS_NULL(p_drv);

    if ((line.coord_s.x > p_drv->lcd_param->width) 
        || (line.coord_s.y > p_drv->lcd_param->height)
        || (line.coord_e.x > p_drv->lcd_param->width) 
        || (line.coord_e.y > p_drv->lcd_param->height)
        || (line.width > p_drv->lcd_param->width))
    {
        return -EINVAL;
    }   

	uint16_t t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
    point_info_t point;

	delta_x = line.coord_e.x - line.coord_s.x;                          //计算坐标增量 
	delta_y = line.coord_e.y - line.coord_s.y;
	uRow = line.coord_s.x; 
	uCol = line.coord_s.y; 
    point.color   = line.color;

	if (delta_x > 0)
    {
        incx = 1;                                 //设置单步方向 
    }
	else if (delta_x == 0)
    {
        incx = 0;                                 //垂直线 
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
        point.coord.x = uRow;
        point.coord.y = uCol;
		LCD_DrawPoint(p_drv, point);//画点 
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
int LCD_DrawLine(driver_info_t *p_drv, line_info_t line)
{
    IS_NULL(p_drv);

    if ((line.coord_s.x > p_drv->lcd_param->width) 
        || (line.coord_s.y > p_drv->lcd_param->height)
        || (line.coord_e.x > p_drv->lcd_param->width) 
        || (line.coord_e.y > p_drv->lcd_param->height)
        || (line.width > p_drv->lcd_param->width))
    {
        return -EINVAL;
    } 

    uint16_t delta_x = line.coord_e.x-line.coord_s.x;
    uint16_t delta_y = line.coord_e.y-line.coord_s.y;
    line_info_t tmp;
    tmp = line;
    tmp.width = 1;

    for (uint8_t i = 0; i < line.width; i++)
    {
        if (delta_y == 0)
        {
            tmp.coord_s.y = line.coord_s.y + i;
            tmp.coord_e.y = line.coord_e.y + i;
            LCD_DrawUnitLine(p_drv, tmp);
        }
        else if (delta_x == 0)
        {
            tmp.coord_s.x = line.coord_s.x + i;
            tmp.coord_e.x = line.coord_e.x + i;
            LCD_DrawUnitLine(p_drv, tmp);
        }
        else
        {
            // TODO: 斜线增粗有点缺陷
            tmp.coord_s.y = line.coord_s.y + i;
            tmp.coord_e.y = line.coord_e.y + i;
            LCD_DrawUnitLine(p_drv, tmp);
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
int LCD_DrawRectangle(driver_info_t *p_drv, rect_info_t rect)
{
    IS_NULL(p_drv);

    if ((rect.coord_s.x > p_drv->lcd_param->width) 
        || (rect.coord_s.y > p_drv->lcd_param->height)
        || (rect.coord_e.x > p_drv->lcd_param->width) 
        || (rect.coord_e.y > p_drv->lcd_param->height)
        || (rect.width > p_drv->lcd_param->width))
    {
        return -EINVAL;
    } 

    line_info_t line;
    uint16_t x1 = rect.coord_s.x;
    uint16_t x2 = rect.coord_e.x;
    uint16_t y1 = rect.coord_s.y;
    uint16_t y2 = rect.coord_e.y;
    uint8_t width = rect.width;

    if (((x1 + width) > (x2 - width)) || (y1 + width) > (y2 - width))
    {
        return -1;
    }

    line.width = 1;
    line.color = rect.color;
    for (uint8_t i = 0; i < width; i++)
    {
        line.coord_s.x = rect.coord_s.x;
        line.coord_e.x = rect.coord_e.x;
        line.coord_s.y = rect.coord_s.y + i;
        line.coord_e.y = rect.coord_s.y + i;
        LCD_DrawLine(p_drv, line);              // 上

        line.coord_s.y = rect.coord_s.y;
        line.coord_e.y = rect.coord_e.y;
        line.coord_s.x = rect.coord_s.x + i;
        line.coord_e.x = rect.coord_s.x + i;
        LCD_DrawLine(p_drv, line);              // 左

        line.coord_s.x = rect.coord_s.x;
        line.coord_e.x = rect.coord_e.x;
        line.coord_s.y = rect.coord_e.y - i;
        line.coord_e.y = rect.coord_e.y - i;
        LCD_DrawLine(p_drv, line);              // 下

        line.coord_s.y = rect.coord_s.y;
        line.coord_e.y = rect.coord_e.y;
        line.coord_s.x = rect.coord_e.x - i;
        line.coord_e.x = rect.coord_e.x - i;
        LCD_DrawLine(p_drv, line);              // 右
    }

    return 0;
}


/**
 * @brief  按百分比块填充
 * 
 * @param[i] p_drv              驱动设备结构体 
 * @param[i] (x1, y1)(x2, y2)   矩形的对角坐标
 * @param[i] color              填充块颜色
 * @param[i] rate               填充百分比
 * @param[i] coord_x_end        保留上次的横轴终止坐标
 * 
 * @retval [description] 
 */
int LCD_FillRectanglePercent(driver_info_t *p_drv, rect_dynamic_info_t *p_rect)
{
    IS_NULL(p_drv);
    IS_NULL(p_rect->rect);

    if ((p_rect->rate > 100) || (p_rect->end_x > p_drv->lcd_param->width))
    {
        return -EINVAL;
    } 

    fill_area_info_t fill;
    uint16_t x_end = p_rect->rect->coord_s.x + 
                     (p_rect->rect->coord_e.x - p_rect->rect->coord_s.x)*p_rect->rate/100;

    p_rect->end_x = (p_rect->end_x > p_rect->rect->coord_s.x) ? p_rect->end_x : p_rect->rect->coord_s.x;
    p_rect->rate = ((p_rect->rate > 100) ? 100 : p_rect->rate);

    if (x_end == p_rect->rect->coord_s.x)
    {
        x_end += p_rect->rect->width;
    }

    if (x_end == p_rect->rect->coord_e.x)
    {
        x_end -= p_rect->rect->width;
    }

    fill.coord_s.y = p_rect->rect->coord_s.y + p_rect->rect->width;
    fill.coord_e.y = p_rect->rect->coord_e.y - p_rect->rect->width;

    // (x_new x_old)对这个区间进行填充/清屏操作
    if (x_end < p_rect->end_x)
    {
        fill.coord_s.x = x_end;
        fill.coord_e.x = p_rect->end_x;
        fill.color = BLACK;
        LCD_Fill(p_drv, fill);
    }
    else
    {
        fill.coord_s.x = p_rect->end_x;
        fill.coord_e.x = x_end;
        fill.color = p_rect->rect->color;
        LCD_Fill(p_drv, fill);
    }

    p_rect->end_x = x_end;
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
int LCD_DrawTriangle(driver_info_t *p_drv, triangle_info_t triangle)
{
    IS_NULL(p_drv);

    if ((triangle.coord.x > p_drv->lcd_param->width) 
        || (triangle.coord.y > p_drv->lcd_param->height)
        || (triangle.coord.x > p_drv->lcd_param->width) 
        || (triangle.coord.y > p_drv->lcd_param->height)
        || (triangle.base > p_drv->lcd_param->width) 
        || (triangle.height > p_drv->lcd_param->height)
        || (triangle.width > p_drv->lcd_param->width)
        || (triangle.coord.x + triangle.base > p_drv->lcd_param->width)
        || (triangle.coord.y - triangle.height < 0))
    {
        return -EINVAL;
    } 

    // 三个坐标(x, y), (x+base, y), (x+base, y-height)
    line_info_t line;

    line.width = 1;
    line.color = triangle.color;
    for (uint8_t i = 0; i < triangle.width; i++)
    {
        line.coord_s.x = triangle.coord.x;
        line.coord_s.y = triangle.coord.y + i;
        line.coord_e.x = triangle.coord.x + triangle.base;
        line.coord_e.y = triangle.coord.y + i;
        LCD_DrawLine(p_drv, line);

        line.coord_s.x = triangle.coord.x + triangle.base - i;
        line.coord_s.y = triangle.coord.y;
        line.coord_e.x = triangle.coord.x + triangle.base - i;
        line.coord_e.y = triangle.coord.y - triangle.height;
        LCD_DrawLine(p_drv, line);
    }

    line.coord_s.x = triangle.coord.x;
    line.coord_s.y = triangle.coord.y;
    line.coord_e.x = triangle.coord.x + triangle.base;
    line.coord_e.y = triangle.coord.y - triangle.height;
    line.width = triangle.width;
    LCD_DrawLine(p_drv, line);

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
int LCD_FillTriangle(driver_info_t *p_drv,  triangle_fill_info_t *tri_fill)
{
    IS_NULL(p_drv);
    IS_NULL(tri_fill->triangle);

    if (tri_fill->rate > 100) 
    {
        return -EINVAL;
    } 

    line_info_t line;
    // 斜边的表达式
    uint8_t comp = 10;      // 补偿系数，否则计算唔差大
    int16_t k = -1 * tri_fill->triangle->height * comp / tri_fill->triangle->base;
    uint32_t b = tri_fill->triangle->coord.y - k * tri_fill->triangle->coord.x /comp;
    uint16_t x_end = tri_fill->triangle->base * tri_fill->rate / 100;
    
    line.color = tri_fill->triangle->color;
    line.width = 1;
    for (uint16_t i = 0; i < x_end; i++)
    {
        line.coord_s.x = tri_fill->triangle->coord.x + i;
        line.coord_s.y = tri_fill->triangle->coord.y;
        line.coord_e.x = tri_fill->triangle->coord.x + i;
        line.coord_e.y = k * (tri_fill->triangle->coord.x + i)/comp+b;
        LCD_DrawLine(p_drv, line);
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
int LCD_ShowBMP(driver_info_t *p_drv, bmp_info_t bmp_info)
{
    IS_NULL(p_drv);
    if ((bmp_info.coord.x > p_drv->lcd_param->width) 
        || (bmp_info.coord.y > p_drv->lcd_param->height)
        || (bmp_info.height > p_drv->lcd_param->height))
    {
        return -EINVAL;
    } 

    uint16_t y0 = bmp_info.coord.y;
    uint16_t i, k = 0;
    uint8_t tmp;
    lcd_dev_t *lcddev = p_drv->lcd_param;
    point_info_t point;

    point.color = bmp_info.color;

    for (i = 0; i < bmp_info.size; i++)
    {
        tmp = bmp_info.bmp[i];

        for (k = 0; k < 8; k++)
        {
            if (tmp & 0x80)
            {
                point.coord.x = bmp_info.coord.x;
                point.coord.y = bmp_info.coord.y;
                LCD_DrawPoint(p_drv, point);
            }
            
            tmp <<= 1;
            bmp_info.coord.y++;
            if (bmp_info.coord.y > lcddev->height)
            {
                return -1;
            }

            if ((bmp_info.coord.y - y0) >= bmp_info.height)
            {
                bmp_info.coord.x++;
                bmp_info.coord.y = y0;
                if (bmp_info.coord.x > lcddev->width)
                {
                    return -1;
                }
                break;
            }
        }
    }

    return 0;
}

