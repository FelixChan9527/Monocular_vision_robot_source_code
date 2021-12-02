#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED端口定义

#define LED0 PBout(5)// PB5
#define LED1 PAout(5)//	//方向
#define duoji1 PBout(10)// PB5
#define duoji2 PBout(11)// PB5
#define duoji3 PBout(12)// PB5
#define duoji6 PBout(13)// PB5
#define duoji5 PBout(14)// PB5
#define duoji8 PGout(0)// PG0
void LED_Init(void);//初始化		 				    
#endif

