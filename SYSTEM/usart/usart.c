#include "sys.h"
#include "usart.h"	
#include "led.h"
#include "inv_mpu.h"
#include "duoji_math.h"
#include "math.h"
#include "car.h"
extern int gg;
extern int ff;
extern int hh;
extern int kk;

extern int duoji,xieaibei;
extern int bujin;
extern int duoji11;
extern int duoji22;
extern int duoji33;
extern int duoji44;
extern int duoji55;
extern int duoji66;
extern int duoji88;
double jiaodu2;
double andlre=0;
double y22=400;
extern float angle;
extern double juli;
extern double x_tu,y;

int m10;

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
 
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}
extern float pitch,roll,yaw,photo_jiao;
int bujin;
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		

		 if(Res==1)
		{
			printf("x=%f  y=%f   \r\n",x_tu,y);
			printf("s=%f   \r\n",juli);
			printf("angle=%f   \r\n",angle);	
		}
		else if(Res==4)
		{
			duoji66=165;
			printf("duoji66=%d   \r\n",duoji66);	
		}
		else if(Res==0x10)
		{
		x_tu=0;
		y=0;
		xieaibei=1;
		}
		else if(Res==0x11)
		{

		}
		else if(Res==0x12)
		{
			duoji22-=5;
			printf("duoji22=%d\r\n",duoji22);
		}
		else if(Res==0x13)
		{
			duoji22+=5;
			printf("duoji22=%d\r\n",duoji22);
		}
		else if(Res==0x21)
		{

		}
		else if(Res==0x22)
		{
			andlre+=5;printf("andlre=%f\r\n",andlre);
		}
		else if(Res==0x23)
		{
			andlre-=5;printf("andlre=%f\r\n",andlre);
		}
		else if(Res==0x24)
		{
			duoji22-=1;
			printf("duoji22=%d\r\n",duoji22);
		}
		
		else if(Res==0x29)
		{
			duoji66+=5;
			printf("duoji66=%d   \r\n",duoji66);
		}
		else if(Res==0x30)
		{
			duoji66-=5;
			printf("duoji66=%d  \r\n",duoji66);
		}
		else if(Res==0x25)
		{
			duoji33+=5;
		  printf("duoji33=%d   \r\n",duoji33);
		}
		else if(Res==0x26)
		{
			duoji33-=5;
		//	printf("cos=%f   \r\n",cos(45));
			printf("duoji33=%d   \r\n",duoji33);
		}
		else if(Res==0x27)
		{
			duoji33-=1;
			printf("duoji33=%d",duoji33);
		}
		else if(Res==0x28)
		{
			duoji33++;
			printf("duoji33=%d",duoji33);
		}
		else if(Res==0x36)
		{
			duoji55+=10;
			printf("duoji=%d",duoji55);
		}
		else if(Res==0x37)
		{
			duoji55-=10;
			printf("duoji55=%d",duoji55);
		}
		else if(Res==0x38)
		{
			duoji55++;
			printf("duoji55=%d",duoji55);
		}
		else if(Res==0x39)
		{
			duoji55--;
				printf("duoji55=%d",duoji55);
		}
		else if(Res==0x40)
		{
			duoji22+=5;
		  printf("duoji22=%d   \r\n",duoji22);
		}
		else if(Res==0x41)
		{
			duoji22-=5;
			printf("duoji22=%d   \r\n",duoji22);
		}else if(Res==0x42)
		{
			duoji22++;
		  printf("duoji22=%d   \r\n",duoji22);
		}
		else if(Res==0x43)
		{
			duoji22--;
			printf("duoji22=%d   \r\n",duoji22);
		}
		else if(Res==0x43)
		{
			duoji11--;
			printf("duoji66=%d   \r\n",duoji66);
		}
		 if(Res==0x61)
		{
			front();
			gg=1;
			printf("go ahead\r\n");
		}
		 if(Res==0x62)
		{
			stop();
			gg=2;
				printf("stop\r\n");
		}
		 if(Res==0x63)
		{
			duoji88+=10;
			printf("duoji88:%d \r\n",duoji88);
		}
		else if(Res==0x64)
		{
			duoji88-=10;
			printf("duoji88:%d \r\n",duoji88);
		}
		else if(Res==0x65)
		{
			duoji88++;
			printf("duoji88:%d \r\n",duoji88);
		}
		else if(Res==0x67)
		{
	      	duoji88--;
			printf("duoji88:%d \r\n",duoji88);

		}
		else if(Res==0x68)
		{
		     front();
			 ff=0;
			 gg=1;
			 while(ff<100);
			
			 kk=0;
			 gg=6;
			 while(kk<100);

		}
		 if(Res==0x71)
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);//0
			GPIO_SetBits(GPIOA,GPIO_Pin_3);//1  左前  //左后
		}
		 if(Res==0x72)
		{
			
			GPIO_SetBits(GPIOF,GPIO_Pin_2);//1    右前   //左前
			GPIO_ResetBits(GPIOF,GPIO_Pin_3);//0
		}
		 if(Res==0x73)
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_4);//0  左后    //右后
			GPIO_SetBits(GPIOF,GPIO_Pin_5);//1     
		}
		else if(Res==0x74)
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0   右后		//右前
			GPIO_SetBits(GPIOF,GPIO_Pin_7);//1     
		}
		else if(Res==0x66)
		{
			printf("ff:%d\r\n",ff);
			printf("gg:%d\r\n",gg);
			printf("hh:%d\r\n",hh);
			printf("kk:%d\r\n",kk);
			printf("\r\n");
		}
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

 



