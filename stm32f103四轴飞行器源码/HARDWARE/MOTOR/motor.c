#include "motor.h"

void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB1ENR|=1<<2;       //TIM4时钟使能    
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能     
  	GPIOB->CRL&=0X00FFFFFF;   //PA7清0    //PORTB6 7  8  9复用输出
	GPIOB->CRL|=0XBB000000;   //复用功能输出（推挽50MHz输出）
	GPIOB->CRH&=0XFFFFFF00;   
	GPIOB->CRH|=0X000000BB;   
	TIM4->ARR=arr;//设定计数器自动重装值 
	TIM4->PSC=psc;//预分频器不分频
	
	TIM4->CCMR1|=7<<12;  //CH2 PWM2模式	
	TIM4->CCMR1|=7<<4;  //CH1 PWM2模式	
	TIM4->CCMR2|=7<<12;  //CH4 PWM2模式	
	TIM4->CCMR2|=7<<4;  //CH3 PWM2模式	 

	TIM4->CCMR1|=1<<11; //CH2预装载使能	 
	TIM4->CCMR1|=1<<3; //CH1预装载使能	  
	TIM4->CCMR2|=1<<11; //CH4预装载使能	 
	TIM4->CCMR2|=1<<3; //CH3预装载使能	 

	TIM4->CCER|=1<<4;   //OC2 输出使能	   
	TIM4->CCER|=1;   //OC2 输出使能	
	TIM4->CCER|=1<<12;   //OC2 输出使能	   
	TIM4->CCER|=1<<8;   //OC2 输出使能	

	TIM4->CR1=0x8000;   //ARPE使能 
	TIM4->CR1|=0x01;    //使能定时器4 															  
} 

