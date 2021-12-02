#include "led.h" 
#include "math.h" 
#include "delay.h"
#include "timer.h"
#include "sys.h"
#include "usart.h" 
#include "Robotarm.h"
#include "dcmi.h" 
#include "car.h" 
double arm1=320;//爪子舵机与关节舵机的长度
double arm2=320;//关节舵机与第三个舵机的长度
extern double angle,juli; 
double Angleduo2;
extern double joint;
extern int duoji88,duoji11,duoji22,duoji33;
double angleaim;//底板舵机转动目标角度 以正前方为0°，左正右负
double jointaim;//关节舵机转动目标角度  伸直时为180°，弯曲减少
extern int pp,duoji55,way;

void anglechange(double cc)//底板舵机转去目标角度 偏差±1°
{
	int aa=1;
	while(aa)
	{
		if(Angleduo2<cc-1.0)
		{
			duoji88++;
			delay_ms(5);
		}
		else if(Angleduo2>cc+1.0)
		{
			duoji88--;
			delay_ms(5);
		}
		else aa=0;
//		printf("duoji88:%d\r\n",duoji88);

	}
}
void jointchange(double cc)//关节舵机转去目标角度 偏差±1°
{
	int aa=1;
	while(aa)
	{
		if(joint<cc-1.0)
		{
			duoji22--;
			delay_ms(5);
//			printf("duoji22=%d",duoji22);
		}
		else if(joint>cc+1.0)
		{
			duoji22++;
			delay_ms(5);
//			printf("duoji22=%d",duoji22);
		}
		
		else aa=0;
	}
	
}
void Pickup(double a,double b)//a为小球距离值，b为小球相对舵机8的相对角度值
{
	if(duoji55>203)
	{
		angleaim=b-acos((arm2*arm2+a*a-arm1*arm1)/(2*arm2*a))*180/3.1415926;
		jointaim=360-(acos((arm1*arm1+arm2*arm2-a*a)/(2*arm1*arm2))*180/3.1415926);	
	}
	else
	{
		angleaim=b+acos((arm2*arm2+a*a-arm1*arm1)/(2*arm2*a))*180/3.1415926;
		jointaim=(acos((arm1*arm1+arm2*arm2-a*a)/(2*arm1*arm2))*180/3.1415926);
	}
//	angleaim=b+acos((arm2*arm2+a*a-arm1*arm1)/(2*arm2*a))*180/3.1415926;
//	jointaim=(acos((arm1*arm1+arm2*arm2-a*a)/(2*arm1*arm2))*180/3.1415926);	
	printf("ok1\n");
	anglechange(angleaim);
	jointchange(jointaim);
//	delay_ms(400);
	while(duoji33<=195)
	{
		duoji33+=2;
		delay_ms(50);
	}
	while(duoji11<=220)
	{
		duoji11++;
		delay_ms(3);
	}	
	if(pp==18)
	{
		while(duoji33>=183)
		{
				duoji33-=2;
				delay_ms(50);
		}		
		while(duoji88<=117)
		{
			duoji88++;
			delay_ms(5);
		}
		while(duoji88>=117)
		{
			duoji88--;
			delay_ms(5);
		}	
		while(duoji33>=183)
	{
		duoji33-=2;
		delay_ms(50);
	}	
	Backward(171);
		pp=17;
	}
}

void Putdown(double dd)//指定方向伸直放球
{		
	while(duoji33>=183)
	{
		duoji33-=2;
		delay_ms(50);
	}
//	jointchange(166);
	delay_ms(100);
	if((pp==16)||(pp==18))
	{
		while(duoji22<170)
		{
			duoji22++;
			delay_ms(2);
		}	
		while(duoji22>170)
		{
			duoji22--;
			delay_ms(2);
		}			
	}
	else 
	{
		while(duoji22<166)
		{
			duoji22++;
			delay_ms(2);
		}	
		while(duoji22>166)
		{
			duoji22--;
			delay_ms(2);
		}			
	}

	
//****************************************************************************//	
	if(way==1)
	{
			if(pp==4)
			{
				while(duoji88<=150)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=150)
				{
					duoji88--;
					delay_ms(5);
				}
			}	
			if(pp==6)
			{
					while(duoji88<=218)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=218)
				{
					duoji88--;
					delay_ms(5);
				}
			}	
			if(pp==8)
			{
					while(duoji88<=237)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=237)
				{
					duoji88--;
					delay_ms(5);
				}		
			}	
			if(pp==10)
			{
					while(duoji88<=247)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=247)
				{
					duoji88--;
					delay_ms(5);
				}					
			}
			if(pp==12)
			{
					while(duoji88<=247)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=247)
				{
					duoji88--;
					delay_ms(5);
				}					
			}
			if(pp==14)
			{
					while(duoji88<=227)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=227)
				{
					duoji88--;
					delay_ms(5);
				}					
			}
			if(pp==16)
			{
					while(duoji88<=117)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=117)
				{
					duoji88--;
					delay_ms(5);
				}					
			}	
			if(pp==18)
			{
					while(duoji88<=117)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=117)
				{
					duoji88--;
					delay_ms(5);
				}					
			}	
	}	
//*****************************************************************************************//

	if(way==2)
	{
			if(pp==4)
			{
					while(duoji88<=202)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=202)
				{
					duoji88--;
					delay_ms(5);
				}
			}	
			if(pp==6)
			{
					while(duoji88<=142)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=142)
				{
					duoji88--;
					delay_ms(5);
				}
			}	
			if(pp==8)
			{
					while(duoji88<=117)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=117)
				{
					duoji88--;
					delay_ms(5);
				}		
			}	
			if(pp==10)
			{
					while(duoji88<=117)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=117)
				{
					duoji88--;
					delay_ms(5);
				}					
			}
			if(pp==12)
			{
					while(duoji88<=117)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=117)
				{
					duoji88--;
					delay_ms(5);
				}					
			}
			if(pp==16)
			{
					while(duoji88<=242)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=242)
				{
					duoji88--;
					delay_ms(5);
				}					
			}	
			if(pp==18)
			{
					while(duoji88<=242)
				{
					duoji88++;
					delay_ms(5);
				}
				while(duoji88>=242)
				{
					duoji88--;
					delay_ms(5);
				}					
			}	

	}	
//*****************************************************************************//	
	
	delay_ms(1000);
	duoji11=140;
}

