#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

#define PWMA   TIM4->CCR1
#define PWMB   TIM4->CCR2
#define PWMC   TIM4->CCR3
#define PWMD   TIM4->CCR4
void MiniBalance_PWM_Init(u16 arr,u16 psc);
#endif
