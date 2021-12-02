#include "LCD.h"
#include "sys.h"
#include "math.h" 
#include "delay.h"
#include "usart.h"
#include "dcmi.h" 
#include "Py.h" 
extern  u16  LCD_ReadPoint(u16 x,u16 y); 

int r, g , b;

int w_end=240,w_s=0;
int h_end=320,h_s=0;
int mubiao_w=30,mubiao_h=30;

float rw=3.85;//3.73  //-10.93 //-12.37
float gw=6.66;//6.42   -8.20  -8.69
float bw=12.40;//12.01  -2.89   -3.32

double b22=5.09;//5.08   4.95    4.94

double z;
double output,lost=0;
double y_out=1;//Ô¤²â
double	y_xun=0;//ÑµÁ·±êÇ©
int x5,y5;
int x8=0,y8=0,w8=0,h8=0;
double a22=0.1;
extern int w2,h2;
int m=10;
int mal[3]={0};
int mil[3]={0};
int mah[3]={0};
int mih[3]={0};
int lp1[3]={0};
int hp1[3]={0};
int lsign=0;
int hsign=0;

double k[2][3]={1};
int x_guji,y_guji;
double p_biao=1;
double b225=0.5;

int l[3]={0};
int h[3]={0};
typedef struct
	  {
    unsigned char  red;             // [0,255]
    unsigned char  green;           // [0,255]
    unsigned char  blue;            // [0,255]
    }COLOR_RGB1;//RGB¸ñÊ½ÑÕÉ«

typedef struct
	  {
    unsigned char hue;              // [0,240]
    unsigned char saturation;       // [0,240]
    unsigned char luminance;        // [0,240]
    }COLOR_HSL1;//HSL¸ñÊ½ÑÕÉ«	
		COLOR_RGB1 Rgb;
			COLOR_HSL1 Hsl;
 void ReadColor2(unsigned int x,unsigned int y,COLOR_RGB1 *Rgb)

	{
		unsigned short C16;

		C16 = LCD_ReadPoint(x,y);     //¶ÁÄ³µãÑÕÉ«

		Rgb->red   =	 (unsigned char)((C16&0xf800)>>8);
		Rgb->green =	 (unsigned char)((C16&0x07e0)>>3);
		Rgb->blue  =   (unsigned char)((C16&0x001f)<<3);
	}

#define min(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))//È¡×î´ó
#define max(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))//È¡×îÐ¡Öµ

//RGB×ªHSL
void RGBtoHSL2(const COLOR_RGB1 *Rgb, COLOR_HSL1 *Hsl)
	{
			int h,s,l,maxVal,minVal,difVal;
			int r  = Rgb->red;
			int g  = Rgb->green;
			int b  = Rgb->blue;
		
				maxVal = max(r, g, b);
				minVal = min(r, g, b);
				
				difVal = maxVal-minVal;
		
		//¼ÆËãÁÁ¶È
			l = (maxVal+minVal)*240/255/2;
		
		if(maxVal == minVal)//Èôr=g=b
			{
				h = 0; 
				s = 0;
			}
		else
		{
			//¼ÆËãÉ«µ÷
			if(maxVal==r)
			{
				if(g>=b)
					h = 40*(g-b)/(difVal);
				else
					h = 40*(g-b)/(difVal) + 240;
			}
			else if(maxVal==g)
					h = 40*(b-r)/(difVal) + 80;
			else if(maxVal==b)
					h = 40*(r-g)/(difVal) + 160;
			//¼ÆËã±¥ºÍ¶È
			if(l == 0)
					s = 0;
			else if(l<=120)
					s = (difVal)*240/(maxVal+minVal);
			else
					s = (difVal)*240/(511 - (maxVal+minVal));
		}
			Hsl->hue =        (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));//É«¶È
			Hsl->saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));//±¥ºÍ¶È
			Hsl->luminance =  (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));//ÁÁ¶È
		 // printf ("(r=%d,g=%d,b=%d)\r\n",r,g,b);
		printf ("h:%d\r\n",h);
		printf ("s:%d\r\n",s);
//		  printf ("l=%d\r\n",l);
		
	}

void test2222(int x2,int y2 )
{ 
	
	int i,j;

   int x3_start,y3_start,
		   x3_end,y3_end;
	 
	 x3_start=  x2-w2/2+5; //x2-15  120  160  ///100~160   140~180
	 x3_end=  x2+w2/2-5; //x2+15    w2/2=20
	 
   y3_start=  y2-h2/2+4; //y3-16
	 
	 y3_end=  y2+h2/2-2;//y3+22

	for(j=y3_start;j<y3_end;j++)
	 {

	        for(i=x3_start;i<x3_end;i++)
	    { 
          ReadColor2(i,j,&Rgb);
          RGBtoHSL2(&Rgb, &Hsl);
//           ColorMatch22(&Hsl);//HSL¸ñÊ½ÑÕÉ«¡¢ÅÐ¶¨ÎªµÄÄ¿±êÌ
	 
     	}				
	
	
  }
	 

	
 }  							



