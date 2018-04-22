#include "control.h"	
#include "filter.h"	
#define PI 3.14159265
/**************************************************************************
作者：Mini Balance 
淘宝店铺：http://shop114407458.taobao.com/
**************************************************************************/

/**************************************************************************
函数功能：5MS定时中断函数 5MS控制周期
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//10ms定时中断
	{   
		TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
  		//Led_Flash(400);                                          //===LED闪烁;

		Key();                                                  //===扫描按键状态

		if(PID_OK==1)
		{
		/*if(in_10ms<700)
		{
		 in_10ms++;
		}
		else
		{
			LED=~LED;
			PID_CONTROL+=3;
			//in_10ms=0;
		}*/
			if(gyro[0]>3000||gyro[0]<-3000)			{				gyro[0]=gyro_0_last;			}
			if(gyro[1]>3000||gyro[1]<-3000)			{				gyro[1]=gyro_1_last;			}
														gyro_0_last=gyro[0];
														gyro_1_last=gyro[1];
			
			gyro[0]/=16.40;gyro[1]/=16.40;gyro[2]/=16.40;//转换成角度值
			
			//作积分限幅
			if(Pitch>5||Pitch<-5)			{				PID_ki=0;			}	
			if(Roll>5||Roll<-5)				{				PID_kiy=0;		}
			
	//以下是x模式
			//调好的X轴  3.3  1.5
//			Moto1=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd;//平衡的PD控制  
//			Moto2=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd;//可自行加入Z轴P控制抑制自旋 
//			Moto3=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd;////////////// 
//			Moto4=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd;////////////// 
			//调好的Y轴  4.4 2.0
//			Moto1=PID_CONTROL + PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;//平衡的PD控制  
//			Moto2=PID_CONTROL - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;//可自行加入Z轴P控制抑制自旋 
//			Moto3=PID_CONTROL + PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;////////////// 
//			Moto4=PID_CONTROL - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;////////////// 
			
			//可以在这里加一段缓慢启动的代码，每进入几次  +PID_CONTROL
			//最终x模式代码
//			Moto1=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd 
//							+ PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;//平衡的PD控制  
//			Moto2=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd
//							 - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;//可自行加入Z轴P控制抑制自旋 
//			Moto3=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd
//								 + PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;////////////// 
//			Moto4=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd
//									 - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;////////////// 

			//PID_CONTROL=PID_CONTROL-gyro[2]*0.1;
			//调好的十字模式
			Moto4=PID_CONTROL + PID_kp*(Pitch+0) + gyro[1]*PID_kd + PID_ki*Pitch_integ;//平衡的PD控制  
			Moto3=PID_CONTROL - PID_kp*(Pitch+0) - gyro[1]*PID_kd - PID_ki*Pitch_integ;//可自行加入Z轴P控制抑制自旋 
			Moto2=PID_CONTROL - PID_kpy*(Roll-0) - gyro[0]*PID_kdy - PID_kiy*Roll_integ;////////////// 
			Moto1=PID_CONTROL + PID_kpy*(Roll-0) + gyro[0]*PID_kdy + PID_kiy*Roll_integ;////////////// 		
			
			Pitch_integ+=Pitch;    Roll_integ+=Roll;//对误差的累加
			
			Xianfu_Pwm();                                            //===PWM限幅
			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器
//	    if(Turn_Off()==0)                   //===如果不存在异常
//				Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器
//			else
//			{
//				Set_Pwm(18000,18000,18000,18000);
//				PID_OK=0;
//			}
		}
		    		
	}       
} 


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
作    者：Mini Balance
**************************************************************************/
void Set_Pwm(int moto1,int moto2,int moto3,int moto4)
{
			PWMA=myabs(moto1);
			PWMB=myabs(moto2);	
	   	PWMC=myabs(moto3);
			PWMD=myabs(moto4);	
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int high_throttle=PID_CONTROL+PID_limit;
	  int low_throllte=PID_CONTROL-PID_limit;

    if(Moto1<low_throllte) Moto1=low_throllte;	
		if(Moto1>high_throttle)  Moto1=high_throttle;
			
	  if(Moto2<low_throllte) Moto2=low_throllte;	
		if(Moto2>high_throttle)  Moto2=high_throttle;
				
	  if(Moto3<low_throllte) Moto3=low_throllte;	
		if(Moto3>high_throttle)  Moto3=high_throttle;
			
	  if(Moto4<low_throllte) Moto4=low_throllte;	
		if(Moto4>high_throttle)  Moto4=high_throttle;			
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
作    者：Mini Balance
**************************************************************************/
u8 Turn_Off(void)
{
	  u8 temp;
	  //if(Pitch<-40||Pitch>40||1==Flag_Stop||Roll<-40||Roll>40)
		if(Pitch<-80||Pitch>80||Roll<-80||Roll>80)
	  {	                                                 //===倾角大于40度关闭电机
      	temp=1;                                            //===Flag_Stop置1关闭电机
      }
	  else
      	temp=0;
    return temp;			
}
	

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click(); 
	if(tmp==1)
	
		//delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
		//PWMA=(40000-2500)/2;		PWMB=(40000-2500)/2; 		PWMC=(40000-2500)/2;		PWMD=(40000-2500)/2;
		//PWMA=17000;		PWMB=17000; 		PWMC=17000;		PWMD=17000;
		//PID_OK=1;
		Flag_Stop=!Flag_Stop;
	
//	if(tmp==2)Flag_Show=~Flag_Show;
}

