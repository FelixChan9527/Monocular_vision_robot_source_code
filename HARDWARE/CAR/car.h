#ifndef __CAR_H
#define __CAR_H
#include "sys.h"
//void PWM_Init(u16 arr,u16 psc);
//void PID_Output(void);
//void car(void);
//void front(void);//p1为左电机，p2为右电机pwm  int p1,int p2,int p3,int p4,u8 ag
//void back(void);//p1为左电机，p2为右电机pwm
//void left(void);
//void right(void);
//void PID_Operation(void);
//void P_to_PWM(void);
//void turn_90(void);
//void turn(void);
//void stop(void);
////void TIM5_Int_Init(u16 arr,u16 psc);
void Car_Init(void);
void stop(void);
void front(void);
void back(void);
void left(void);
void right(void);
void Gohead(int ee);
void Clockwise(int ee);
void Backward(int ee);
void Anticlockwise(int ee);
void Stand_init();
void turn(int i);
void carmpu(void);
#endif
