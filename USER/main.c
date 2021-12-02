#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "usart2.h"  
#include "timer.h" 
#include "ov2640.h" 
#include "dcmi.h" 
#include "easytrace.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "math.h"
#include "car.h"
#include "key.h"
#include "adc.h"
#include "Robotarm.h"
#include "Py.h"
#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))//取最大
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))//取最小值

int x2=40,y9=290, w2=36,h2=36;

TARGET_CONDI Condition; //
extern int red1[6];
extern int red2[6];
extern int blue1[6];
extern int blue2[6];
float roll,pitch,yaw;//欧拉角转化值
int shibie_r_or_g[6];
extern int duoji88,duoji22,duoji33;	

double angle,angle_1; 		//欧拉角
volatile u8 rgb_data_ok=0;								
extern int duoji44,duoji55,duoji66,duoji3_qi;							

double k3=3.2258;
double b3=121000;
double s3=101;
double p1=0.0011626;
double p2=0.65794;
double p3=280.15;
double x_tu,y,juli;
int duojiz,xieaibei,FLAG=0,flag_2=0;
extern int  zhui_zong_sign;
extern int duoji11,duoji1_kaizhua;
extern int m10;
int red,pp=1,way=1;
int q=0;
int zhui_zong_sign;
extern int pwm;
void duoji55_gun(void)
{
	if((duoji55>=147)&&((pp==20)||(pp==4)||(pp==6)||(pp==8)||(pp==10)||(pp==12)||(pp==14)||(pp==16)||(pp==18)))
		duoji88=152;
	else if((duoji55>=126)&&((pp==6)||(pp==8)||(pp==10)||(pp==12)||(pp==18)||(pp==16)))
		duoji88=132;	
	else if((duoji55>=117)&&((pp==6)||(pp==8)||(pp==10)||(pp==12)||(pp==14)||(pp==18)))
		duoji88=135;
//	if(duoji55>=147)
//		duoji88=135;
//	if(duoji55<147)
//		duoji88=182;
}
void duoji55_zhui_zong()////摄像头追踪函数
{	
	static int zhi;	
	if(zhui_zong_sign==0&&duoji55<=260&&duoji55>=68)
	{					
				if((pp==4)||(pp==18))///18
				{
					if(duoji55==68)
							{															 
								zhi=1;
								FLAG++;												
							}							
								if(zhi==1&&duoji55<260)
								{
								  duoji55+=2; delay_ms(150);
								}																				
							if(duoji55>=260)
							{										
								    zhi=0;
								   duoji55=68;
								   delay_ms(500);
							}												
				}
				
				else 
				{
					if(duoji55==68)
					{
								 
						zhi=1;
						FLAG++;												
					}
							
					if(zhi==1&&duoji55<202)
					{
						duoji55+=2; delay_ms(150);
					}																				
					if(duoji55>=202)
					{										
						zhi=0;
						duoji55=68;	
						delay_ms(500);		
					}							
					if(zhi==0&&duoji55>68)
					{
						duoji55-=2;delay_ms(150);									
					}
				}
		}						 
}

void rgb_data_process(void)
{
	if(rgb_data_ok==0)	//rgb数据还未采集完?
		{	
			DMA_Cmd(DMA2_Stream1,DISABLE);//关闭DMA2,Stream1
			while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//等待DMA2_Stream1可配置  

			rgb_data_ok=1; 				//标记rgb数据采集完按成,等待其他函数处理
			
		}
	if(rgb_data_ok==2)	//上一次的rgb数据已经被处理了
		{
	   DMA2_Stream1->NDTR=1; //数据寄存器数目 	
     LCD_SetCursor(0,0);	//锁定显示起点坐标
     LCD_WriteRAM_Prepare();		//开始写入GRAM，写入颜色数据
	   DMA_Cmd(DMA2_Stream1, ENABLE);//开启DMA2,Stream1 重新传输
	   DCMI_CaptureCmd(ENABLE);//DCMI捕获使能  
		 rgb_data_ok=0;						//标记数据未采集

	
		}
}


void rgb565_test_1(void)
{ 
	int i=0;
	RESULT Resu;

	My_DCMI_Init();			//DCMI配置
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,1,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Disable);//DCMI DMA配置   
	
	OV2640_OutSize_Set(lcddev.width,lcddev.height); 
	DCMI_Start(); 		//启动传输
  LCD_SetCursor(0,0);  
	LCD_WriteRAM_Prepare();		//开始写入GRAM
	DMA_Cmd(DMA2_Stream1,ENABLE);	//开启DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI捕获使能 
		
  while(1)      
	{ 
	if(zhui_zong_sign==0)
	{
		duoji55_zhui_zong();		
	}						
		if(rgb_data_ok==1)
		{
			 i++;					
			 if(i>30){ zhui_zong_sign=0;i=0;}	
			 duoji55_gun();
				if(pp==1)
				{
					Gohead(156);
					pp=2;
//					Gohead(200);					
//					pp=100;
				}
				else if(pp==2)
				{					
					turn(1);
					pp=3;
				}
				else if(pp==3)
				{
					Gohead(343);
					while(duoji22<=166)
				{
					duoji22++;
					delay_ms(5);
				}
				while(duoji22>=166)
				{
					duoji22--;
					delay_ms(5);
				}
					while(duoji88<=176)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=176)
				{
					duoji88--;
					delay_ms(5);
				}
					duoji11=145;
					while(duoji33<=183)
				{
					duoji33++;
					delay_ms(10);
				}
				while(duoji33>=183)
				{
					duoji33--;
					delay_ms(10);
				}		
					pp=4;
				}
				else if(pp==5)
				{												
					duoji33=145;
					duoji11=212;
					duoji22=250;					
					while(duoji88<=97)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=97)
				{
					duoji88--;
					delay_ms(5);
				}
					turn(2);
					Gohead(25);
					while(duoji22<=166)
				{
					duoji22++;
					delay_ms(5);
				}
				while(duoji22>=166)
				{
					duoji22--;
					delay_ms(5);
				}
					while(duoji88<=176)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=176)
				{
					duoji88--;
					delay_ms(5);
				}
					duoji11=145;
					while(duoji33<=183)
				{
					duoji33++;
					delay_ms(10);
				}
				while(duoji33>=183)
				{
					duoji33--;
					delay_ms(10);
				}		
					pp=6;
				}
				else if(pp==7)
				{
					Gohead(81);					
					pp=8;				
				}
				else if(pp==9)
				{
					Gohead(81);
					pp=10;				
				}
				else if(pp==11)
				{
//					Gohead(70);
					FLAG=6;
					pp=12;				
				}
				if(pp==13)
				{
					duoji33=145;
					duoji11=212;
					duoji22=250;					
					while(duoji88<=97)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=97)
				{
					duoji88--;
					delay_ms(5);
				}
					turn(3);
					duoji22=166;
					while(duoji22<=166)
				{
					duoji22++;
					delay_ms(5);
				}
				while(duoji22>=166)
				{
					duoji22--;
					delay_ms(5);
				}
					while(duoji88<=176)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=176)
				{
					duoji88--;
					delay_ms(5);
				}
					duoji11=145;
					while(duoji33<=183)
				{
					duoji33++;
					delay_ms(10);
				}
				while(duoji33>=183)
				{
					duoji33--;
					delay_ms(10);
				}		
					pp=14;				
				}
				if(pp==15)
				{
					duoji33=145;
					duoji11=212;
					duoji22=250;					
					while(duoji88<=97)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=97)
				{
					duoji88--;
					delay_ms(5);
				}
					turn(4);
					while(duoji22<=166)
				{
					duoji22++;
					delay_ms(5);
				}
				while(duoji22>=166)
				{
					duoji22--;
					delay_ms(5);
				}
					while(duoji88<=176)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=176)
				{
					duoji88--;
					delay_ms(5);
				}
					duoji11=145;
					while(duoji33<=183)
				{
					duoji33++;
					delay_ms(10);
				}
				while(duoji33>=183)
				{
					duoji33--;
					delay_ms(10);
				}		
					pp=16;				
				}		
				if(pp==17)
				{
					Gohead(151);
					zhui_zong_sign=0;
					pp=18;
				}

				if((pp==4)||(pp==6)||(pp==8)||(pp==10)||(pp==12)||(pp==14)||(pp==16)||(pp==18))
				{					
					zhui_zong_sign=0;
					if((FLAG==2)&&(pp==4))
					{
						pp=5;
						zhui_zong_sign=1;
					}	
					else if((FLAG==3)&&(pp==6))
					{
						pp=7;
						zhui_zong_sign=1;
					}	
					else if((FLAG==4)&&(pp==8))
					{
						pp=9;
						zhui_zong_sign=1;
					}	
					else if((FLAG==5)&&(pp==10))
					{
						pp=11;
						zhui_zong_sign=1;
					}	
					else if((FLAG==6)&&(pp==12))
					{
						pp=13;
						zhui_zong_sign=1;
					}
					else if((FLAG==7)&&(pp==14))
					{
						pp=15;
						zhui_zong_sign=1;
					}
					else if((FLAG==8)&&(pp==16))
					{
						pp=17;
						zhui_zong_sign=1;
					}
					else if((FLAG==9)&&(pp==18))
					{
						zhui_zong_sign=1;
					}
				}
				
				
					
					if(Trace(&Condition,&Resu))                      //API
					{	
					
						LCD_Fill(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.x+Resu.w/2,Resu.y-Resu.h/2+1,0xFFFF);//u16 x,u16 y,u16 width,u16 hight,u16 Color
						LCD_Fill(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.x-Resu.w/2+1,Resu.y+Resu.h/2,0xFFFF);
						LCD_Fill(Resu.x-Resu.w/2,Resu.y+Resu.h/2,Resu.x+Resu.w/2,Resu.y+Resu.h/2+1,0xFFFF);
						LCD_Fill(Resu.x+Resu.w/2,Resu.y-Resu.h/2,Resu.x+Resu.w/2+1,Resu.y+Resu.h/2,0xFFFF);
						LCD_Fill(Resu.x-2,Resu.y-2,Resu.x+2,Resu.y+2,0xFFFF);				
						  x_tu=240-Resu.x;
						  y=320-Resu.y;
						printf("x=%f,y=%f\r\n",x_tu,y);
						zhui_zong_sign=1;

				if(x_tu>=0&&x_tu<103){duoji55-=1;}
				else if(x_tu>109){duoji55+=1;}
				if((duoji55>122)&&(duoji55<130))
					duoji55+=20;
//				if((pp==4)||(pp==16))
				if(pp==4)
				{
					if(duoji55>=260)
					{
						duoji55=68;
						FLAG++;
					}	
					if(duoji55<68)
					{
						duoji55=80;
					}					
				}	
				else 
				{
					if(duoji55>=202)
					{
						duoji55=68;
						FLAG++;
					}	
					if(duoji55<68)
					{
						duoji55=80;
					}					
				}	
				if(x_tu<=109&&x_tu>=103)
				{					 
					angle=0.9*(duoji55-155 );	
					if((angle>=angle_1*0.9)&&(angle<=angle_1*1.05))
					{
						duoji55+=70;
					}
					angle_1=angle;					
					juli=p1*y*y+p2*y+p3;	
					printf("juli=%f",juli);					
						Pickup(juli,angle);					
						Putdown(45); 
						
					zhui_zong_sign=0;					
					
				}
			}	
				  rgb_data_ok=2;//上一次的rgb数据已经被处理了
		}
	
		
	
		
     }
}

void rgb565_test_2(void)
{ 
	int i=0;
	RESULT Resu;

	My_DCMI_Init();			//DCMI配置
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,1,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Disable);//DCMI DMA配置   
	
	OV2640_OutSize_Set(lcddev.width,lcddev.height); 
	DCMI_Start(); 		//启动传输
  LCD_SetCursor(0,0);  
	LCD_WriteRAM_Prepare();		//开始写入GRAM
	DMA_Cmd(DMA2_Stream1,ENABLE);	//开启DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI捕获使能 
		
  while(1)      
	{ 
	if(zhui_zong_sign==0)
	{
		duoji55_zhui_zong();		
	}						
		if(rgb_data_ok==1)
		{
			 i++;					
			 if(i>30){ zhui_zong_sign=0;i=0;}	
			 duoji55_gun();
				if(pp==1)
				{
					Gohead(130);
					pp=2;
//					Gohead(200);					
//					pp=100;
				}
				else if(pp==2)
				{					
					turn(1);
					pp=3;
				}
				else if(pp==3)
				{
					Gohead(365);
					duoji22=166;
					duoji88=176;
					duoji11=145;
					duoji33=180;		
					pp=4;
				}
				else if(pp==5)
				{								
					duoji33=138;
					duoji11=212;
					duoji22=250;					
					duoji88=97;
					turn(2);
					Gohead(30);
					duoji22=166;
					duoji88=176;
					duoji11=145;
					duoji33=180;
					pp=6;
				}
				else if(pp==7)
				{
					Gohead(80);					
					pp=8;				
				}
				else if(pp==9)
				{
					Gohead(30);
					pp=10;				
				}
				else if(pp==11)
				{
//					Gohead(70);
					FLAG=6;
					pp=12;				
				}
				if(pp==13)
				{
					duoji33=138;
					duoji11=212;
					duoji22=250;					
					duoji88=97;
					turn(3);
					duoji22=166;
					duoji88=176;
					duoji11=145;
					duoji33=180;
					pp=14;				
				}
				if(pp==15)
				{
					duoji33=138;
					duoji11=212;
					duoji22=250;					
					duoji88=97;
					turn(4);
					duoji22=166;
					duoji88=176;
					duoji11=145;
					duoji33=180;
					pp=16;				
				}		
				
				if((pp==4)||(pp==6)||(pp==8)||(pp==10)||(pp==12)||(pp==14)||(pp==16))
				{					
					zhui_zong_sign=0;
					if((FLAG==2)&&(pp==4))
					{
						Backward(25);
						pp=5;
//						zhui_zong_sign=1;
					}	
					else if((FLAG==3)&&(pp==6))
					{
						pp=7;
						zhui_zong_sign=1;
					}	
					else if((FLAG==4)&&(pp==8))
					{
						pp=9;
						zhui_zong_sign=1;
					}	
					else if((FLAG==5)&&(pp==10))
					{
						pp=11;
						zhui_zong_sign=1;
					}	
					else if((FLAG==6)&&(pp==12))
					{
						pp=13;
						zhui_zong_sign=1;
					}
					else if((FLAG==7)&&(pp==14))
					{
						pp=15;
						zhui_zong_sign=1;
					}
					else if((FLAG==8)&&(pp==16))
					{
						pp=16;
						zhui_zong_sign=0;
					}
				}				
					if(Trace(&Condition,&Resu))                      //API
					{	
					
						LCD_Fill(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.x+Resu.w/2,Resu.y-Resu.h/2+1,0xFFFF);//u16 x,u16 y,u16 width,u16 hight,u16 Color
						LCD_Fill(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.x-Resu.w/2+1,Resu.y+Resu.h/2,0xFFFF);
						LCD_Fill(Resu.x-Resu.w/2,Resu.y+Resu.h/2,Resu.x+Resu.w/2,Resu.y+Resu.h/2+1,0xFFFF);
						LCD_Fill(Resu.x+Resu.w/2,Resu.y-Resu.h/2,Resu.x+Resu.w/2+1,Resu.y+Resu.h/2,0xFFFF);
						LCD_Fill(Resu.x-2,Resu.y-2,Resu.x+2,Resu.y+2,0xFFFF);				
						  x_tu=240-Resu.x;
						  y=320-Resu.y;
						printf("x=%f,y=%f\r\n",x_tu,y);
						zhui_zong_sign=1;

				if(x_tu>=0&&x_tu<103){duoji55-=1;}
				else if(x_tu>109){duoji55+=1;}
				if(duoji55>=260)
				{
					duoji55=82;
					FLAG++;
				}	
				if(duoji55<82)
				{
					duoji55=90;
				}	
				if(x_tu<=109&&x_tu>=103)
				{					 
					angle=0.9*(duoji55-155 );	
					if((angle>=angle_1*0.9)&&(angle<=angle_1*1.05))
					{
						duoji55+=15;
					}
					angle_1=angle;					
					juli=p1*y*y+p2*y+p3;	
					printf("juli=%f\r\n",juli);
						Pickup(juli,angle);

						Putdown(45); 
						
					zhui_zong_sign=0;					
					
				}
			}	
				  rgb_data_ok=2;//上一次的rgb数据已经被处理了
		}
	
		
	
		
     }
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	LED_Init();		
 	Car_Init();
	KEY_Init();
	uart_init(115200);		//初始化串口波特率为115200
 	LCD_Init();	//LCD初始化
	TIM3_Int_Init(31,24);//0.06ms
//	duoji55=66;

	OV2640_Init();  //初始化OV2640 
	MPU_Init();
	mpu_dmp_init();
	OV2640_RGB565_Mode();	//RGB565模式
	LCD_Clear(WHITE); 
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);
	OV2640_OutSize_Set(lcddev.width,lcddev.height);   
	uart_init(115200);
	if(KEY2==0) 
	{ 
		red=1;
		Condition.H_MIN=red2[0];
		Condition.H_MAX=red2[1];
		
		Condition.S_MIN=red2[2];
		Condition.S_MAX=red2[3];
		
		Condition.L_MIN=red2[4];
		Condition.L_MAX=red2[5];
		
		Condition.WIDTH_MIN=30;
		Condition.HIGHT_MIN=30;
		
		Condition.WIDTH_MAX=200;
		Condition.HIGHT_MAX=200;
		
		shibie_r_or_g[0]=red1[0];
		shibie_r_or_g[1]=red1[1];
		shibie_r_or_g[2]=red1[2];
		shibie_r_or_g[3]=red1[3];
		shibie_r_or_g[4]=red1[4];
		shibie_r_or_g[5]=red1[5];
		 
		
	}
	else if(KEY1==0)
	{
		red=0;
		Condition.H_MIN=blue2[0];
		Condition.H_MAX=blue2[1];
		
		Condition.S_MIN=blue2[2];
		Condition.S_MAX=blue2[3];
		
		Condition.L_MIN=blue2[4];
		Condition.L_MAX=blue2[5];
		
		Condition.WIDTH_MIN=30;
		Condition.HIGHT_MIN=30;
		
		Condition.WIDTH_MAX=200;
		Condition.HIGHT_MAX=200;
		
   
	}
	
	if(way==1)
		rgb565_test_1();
	else if(way==2)
		rgb565_test_2();
}

