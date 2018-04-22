#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
extern	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
void TIM1_UP_IRQHandler(void);  
void Set_Pwm(int moto1,int moto2,int moto3,int moto4);
void Xianfu_Pwm(void);
u8 Turn_Off(void);
int myabs(int a);
void Key(void);
#endif
