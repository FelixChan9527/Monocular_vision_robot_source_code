#include "timer.h"
#include "led.h"
#include "usart.h"
#include "lcd.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved			224			117			  
////////////////////////////////////////////////////////////////////////////////// 	 

int duoji11=212;
int duoji22=250;
int duoji33=145;
int duoji88=97;

//int duoji11=145;
int duoji1_kaizhua=145;
int duoji1_zhua=217;

//	int duoji22=166;
//	int duoji88=176;
//	int duoji11=145;
//	int duoji33=183;

//int duoji22=166;
int  duoji2_zhi=166;

//int duoji33=180;//pin40 37//100
int duoji3_shuipin=198;//125,199是冠灵的机械臂
int duoji3_qi=184;
int duoji3_zuigao=140;
int duoji44=130;
int duoji55=68;//摄像头水平转动舵机
int duoji66=232;//210
int duoji77;//用来暂存duoji55的值
//int duoji88=176;
int duo8;
#define zuoqian PAout(13)
#define zuohou PFout(13)
#define youqian PFout(14)
#define youhou PFout(10)
int pwm_zq,pwm_zh,pwm_yq,pwm_yh;

int red1[6]={0,25,   70,185,   0,240};//红色
int red2[6]={220,240, 70,173,  0,240};//红色

int blue2[6]={125,240,     80,240,      30,160};//蓝色
//int blue2[6]={113,240,     80,240,      15,103};//蓝色
		
extern double Angleduo2;
double joint;


int jishu,jishu4;


 int ff;
 int gg;
int hh;
 int kk;
///71 53
extern u8 ov_frame;
extern volatile u16 jpeg_data_len;
extern double r,y,l;

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		jishu++;
		if(jishu==2000)//20ms，10us一个数，对应舵机1个信号值
		{
		jishu=0;
		if(gg==1){ff++;}
		else	if(gg==3)
		{ff--;}
		else	if(gg==5)
		{hh++;}
		else	if(gg==4)
		{hh--;}
		else	if(gg==6)
		{kk++;}
		}	

		if(jishu<duoji11)
		{duoji1=1;}
		else if(jishu>duoji11)
		{duoji1=0;}
		
		if(jishu<duoji22)
		{duoji2=1;}
		else if(jishu>duoji22)
		{duoji2=0;}
		
		if(jishu<duoji33)
		{duoji3=1;}
		else if(jishu>duoji33)
		{duoji3=0;}

	
		
		if(jishu<duoji55)
		{duoji5=1;}
		else if(jishu>duoji55)
		{duoji5=0;}
		if(jishu<duoji66)
		{duoji6=1;}
		else if(jishu>duoji66)
		{duoji6=0;}
		if(duoji88>=247)
			duo8=247;
		else if(duoji88<=117)
			duo8=117;
		else duo8=duoji88;
		if(jishu<duo8)
		{duoji8=1;}
		else if(jishu>duo8)
		{duoji8=0;}
		
		if(jishu<pwm_zq)
			zuoqian=1;
		else if(jishu>pwm_zq)
			zuoqian=0;
		if(jishu<pwm_zh)
			zuohou=1;
		else if(jishu>pwm_zh)
			zuohou=0;
		if(jishu<pwm_yq)
			youqian=1;
		else if(jishu>pwm_yq)
			youqian=0;
		if(jishu<pwm_yh)
			youhou=1;
		else if(jishu>pwm_yh)
			youhou=0;
		
		if(duoji55>202)
		{
			joint=(180-1.6*(duoji22-166));
			Angleduo2=(duoji88-192)*1.5;
		}		
		else if(duoji55<126)
		{
			joint=(180-1.6*(duoji22-166));
			Angleduo2=(duoji88-152)*1.5;
		}
		else if((duoji55>=126)&&(duoji55<=155))
		{
			joint=(180-1.6*(duoji22-166));
			Angleduo2=(duoji88-168)*1.5;
		}
		else if((duoji55<=202)&&(duoji55>155))
		{
			joint=(180-1.6*(duoji22-166));
			Angleduo2=(duoji88-172)*1.5;
		}
		
		
	}
	TIM_ClearITPendingBit(TIM3 ,TIM_IT_Update);  //清除中断标志位
}


