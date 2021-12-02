#include "car.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
extern float roll,pitch,yaw;//欧拉角转化值
//#include "delay.h"
extern int ff;
extern int gg;
extern int kk;
extern int pp;
#define zuoqian PAout(13)
#define zuohou PFout(13)
#define youqian PFout(14)
#define youhou PFout(10)
double P;
#define Balance_point 0
#define Balance_wan1 -75
#define Balance_wan1_1 75
#define Balance_wan2 -162
#define Balance_wan2_1 162
#define Balance_wan3 -161.5
#define Balance_wan3_1 161.5
#define Balance_wan4 -120
#define Balance_wan4_1 120
#define Balance_wan5 -300
#define Balance_wan5_1 300
double Kp_balance=15,kp_wan=15;
extern float yaw;
float yawcar,yawcar_1;
int pwm;
extern int pwm_zq,pwm_zh,pwm_yq,pwm_yh,way;
int zq,zh,yq,yh;
void carmpu(void)
{
	mpu_dmp_get_data(&pitch,&roll,&yaw);
	if(yaw>0)
	{
		yawcar=yaw-360;
	}
	else yawcar=yaw;
	
}
//*************************//
void carmpu_1(void)
{
	mpu_dmp_get_data(&pitch,&roll,&yaw);
	if(yawcar_1<0)
	{
		yawcar_1=360+yaw;
	}
	else yawcar_1=yaw;
	
}
//**************************************************************************************//
void Car_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOB/C时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_10
	|GPIO_Pin_14|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	stop();


}
void stop(void)//p1为左电机，p2为右电机pwm int p1,int p2,int p3,int p4,u8 ag
{
 	GPIO_ResetBits(GPIOA,GPIO_Pin_2);//0
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);//1  左前
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//1    右前
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//0

	
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//0  左后
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//1     
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0   右后
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//1 
}

void Stand_init()//直立函数，输入角度和角速度(PD控制)[直立环]
{
	mpu_dmp_get_data(&pitch,&roll,&yaw);	
	float ex_angel;
	if(way==1)
	{
			if(pp==1)
			{
				ex_angel = yaw - Balance_point;
				pwm = ex_angel*Kp_balance ;
			}
			if(pp==3)
			{
				ex_angel = yaw - Balance_wan1;
				pwm = ex_angel*Kp_balance ;
			}
			if((pp==7)||(pp==9)||(pp==11)||(pp==5))
			{
				ex_angel = yaw - Balance_wan3;
				pwm = ex_angel*Kp_balance ;
			}	
	}

	if(way==2)
	{
			if((pp==1)||(pp==12))
			{
				ex_angel = Balance_point - yaw;
				pwm = ex_angel*Kp_balance ;
			}
			if((pp==3)||(pp==4))
			{
				ex_angel =Balance_wan1_1 - yaw;
				pwm = ex_angel*Kp_balance ;
			}
			if((pp==7)||(pp==9)||(pp==11)||(pp==5))
			{
				ex_angel =  Balance_wan3_1 - yaw;
				pwm = ex_angel*Kp_balance ;
			}	
	}
	
}



void front()//p1为左电机，p2为右电机pwm  int p1,int p2,int p3,int p4,u8 ag
{
	
	GPIO_SetBits(GPIOF,GPIO_Pin_3);//1    左前
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0

	GPIO_SetBits(GPIOA,GPIO_Pin_3);//1  左后
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_6);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//0   右前
	
	GPIO_SetBits(GPIOF,GPIO_Pin_4);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0  右后
	Stand_init();
	if(way==1)
	{
		zq=1800+30*pwm;
		zh=1800+30*pwm;
		yq=1800-30*pwm;
		yh=1800-30*pwm;	
	}	
	
	if(way==2)
	{
		zq=1800-30*pwm;
		zh=1800-30*pwm;
		yq=1800+30*pwm;
		yh=1800+30*pwm;	
	}	
	
	if(zq>2000)
		zq=2000;
	if(zh>2000)
		zh=2000;
	if(yq>2000)
		yq=2000;
	if(yh>2000)
		yh=2000;
	pwm_zq=zq;
	pwm_zh=zh;
	pwm_yq=yq;
	pwm_yh=yh;
	printf("pwm_zq=%d\r\n",pwm_zq);
	printf("pwm_zh=%d\r\n",pwm_zh);
	printf("pwm_yq=%d\r\n",pwm_yq);
	printf("pwm_yh=%d\r\n",pwm_yh);
}

void back()//p1为左电机，p2为右电机的pwm  int p1,int p2,int p3,int p4,u8 ag
{

	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//1    左前
	GPIO_SetBits(GPIOF,GPIO_Pin_2);//0

	GPIO_ResetBits(GPIOA,GPIO_Pin_3);//1  后左
	GPIO_SetBits(GPIOA,GPIO_Pin_2);//0
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//1
	GPIO_SetBits(GPIOF,GPIO_Pin_7);//0   右前	
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//1 
	GPIO_SetBits(GPIOF,GPIO_Pin_5);//0  右后
	    
	Stand_init();
	yq=1800+30*pwm;
	yh=1800+30*pwm;
	zq=1800-30*pwm;
	zh=1800-30*pwm;
	if(zq>2000)
		zq=2000;
	if(zh>2000)
		zh=2000;
	if(yq>2000)
		yq=2000;
	if(yh>2000)
		yh=2000;
	pwm_zq=zq;
	pwm_zh=zh;
	pwm_yq=yq;
	pwm_yh=yh;
}
void right()//p1为左电机，p2为右电机pwm  int p1,int p2,int p3,int p4,u8 ag
{
	
	GPIO_SetBits(GPIOF,GPIO_Pin_3);//1    左前
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0

	GPIO_SetBits(GPIOA,GPIO_Pin_3);//1  左后
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);//0	
    
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//1
	GPIO_SetBits(GPIOF,GPIO_Pin_7);//0   右前
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//1 
	GPIO_SetBits(GPIOF,GPIO_Pin_5);//0  右后
	
	

}
void left()//p1为左电机，p2为右电机pwm  int p1,int p2,int p3,int p4,u8 ag
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//1    左前
	GPIO_SetBits(GPIOF,GPIO_Pin_2);//0

	GPIO_ResetBits(GPIOA,GPIO_Pin_3);//1  左后
	GPIO_SetBits(GPIOA,GPIO_Pin_2);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_6);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//0   右前
	
	GPIO_SetBits(GPIOF,GPIO_Pin_4);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0  右后
  
}
void Gohead(int ee)
{
	ff=0;
	while(ff<ee)
	{
		ff++;
		front();
	}
	ff=0;
	while(ff<(ee/10))
	{
		ff++;
		back();
	}
	stop();
	
}

void turn(int i)
{
	int a=10,b=5;
//******************************************************************//	
	if(way==1)
	{
			carmpu();
			if(i==1)
			{
				while(Balance_wan1<yaw)
				{			
					mpu_dmp_get_data(&pitch,&roll,&yaw);
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
		
				stop();
			}
			if(i==2)
			{
				while(Balance_wan2<yaw)
				{			
					mpu_dmp_get_data(&pitch,&roll,&yaw);
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
				stop();
			}
	
			if(i==3)
			{	
				while(Balance_wan4>yaw)
				{			
					mpu_dmp_get_data(&pitch,&roll,&yaw);
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
				stop();
			}
			if(i==4)
			{	
				while(Balance_wan5<yawcar)
				{			
					carmpu();
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
				stop();
			}
	}

//**************************************************************************//	
	if(way==2)
	{
			carmpu_1();
			if(i==1)
			{
				while(Balance_wan1_1>yaw)
				{			
					mpu_dmp_get_data(&pitch,&roll,&yaw);
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
		
				stop();
			}
			if(i==2)
			{
				while(Balance_wan2_1>yaw)
				{			
					mpu_dmp_get_data(&pitch,&roll,&yaw);
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
				stop();
			}
	
			if(i==3)
			{	
				while(Balance_wan4_1<yaw)
				{			
					mpu_dmp_get_data(&pitch,&roll,&yaw);
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
				stop();
			}
			if(i==4)
			{	
				while(Balance_wan5_1>yawcar_1)
				{			
					carmpu_1();
					left();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;		
				}	
				while(a--)
				{
					right();
					pwm_zq=2000;
					pwm_zh=2000;
					pwm_yq=2000;
					pwm_yh=2000;
				}
				stop();
			}
	}
//***********************************************************************//	
}
void Clockwise(int ee)		//顺时针
{
	right();
	kk=0;
	gg=6;
	while(kk<ee)
	{kk++;
	printf("  kk=%d\n",kk);}
	stop();
}
void Backward(int ee)
{
	int jj,kk;
	ff=0;
	jj=ee*10;
	while(ff<jj)
	{
		ff++;
		back();
	}
	ff=0;
	while(ff<(ee/20))
	{
		ff++;
		front();
	}
	stop();
}
void Anticlockwise(int ee)		//逆时针
{
	left();
	kk=0;
	gg=6;
	while(kk<ee)
	{kk++;
	printf("kk=%d",kk);}
	stop();
}
