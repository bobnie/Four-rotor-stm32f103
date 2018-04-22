
#include "usart2.h"
#include "show.h"
int floagzuo=0;
int floagyou=0;
int floagqian=0;
int floaghou=0;

/**************************************************************************
º¯Êý¹¦ÄÜ£º´®¿Ú2³õÊ¼»¯
Èë¿Ú²ÎÊý£ºpclk2:PCLK2 Ê±ÖÓÆµÂÊ(Mhz)    bound:²¨ÌØÂÊ
·µ»Ø  Öµ£ºÎÞ
**************************************************************************/
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//µÃµ½USARTDIV
	mantissa=temp;				 //µÃµ½ÕûÊý²¿·Ö
	fraction=(temp-mantissa)*16; //µÃµ½Ð¡Êý²¿·Ö	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //Ê¹ÄÜPORTA¿ÚÊ±ÖÓ  
	RCC->APB1ENR|=1<<17;  //Ê¹ÄÜ´®¿ÚÊ±ÖÓ 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO×´Ì¬ÉèÖÃ
	GPIOA->ODR|=1<<2;	  
	RCC->APB1RSTR|=1<<17;   //¸´Î»´®¿Ú1
	RCC->APB1RSTR&=~(1<<17);//Í£Ö¹¸´Î»	   	   
	//²¨ÌØÂÊÉèÖÃ
 	USART2->BRR=mantissa; // ²¨ÌØÂÊÉèÖÃ	 
	USART2->CR1|=0X200C;  //1Î»Í£Ö¹,ÎÞÐ£ÑéÎ».
	//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
	USART2->CR1|=1<<8;    //PEÖÐ¶ÏÊ¹ÄÜ
	USART2->CR1|=1<<5;    //½ÓÊÕ»º³åÇø·Ç¿ÕÖÐ¶ÏÊ¹ÄÜ	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//×é2£¬×îµÍÓÅÏÈ¼¶ 
}

/**************************************************************************
º¯Êý¹¦ÄÜ£º´®¿Ú2½ÓÊÕÖÐ¶Ï
Èë¿Ú²ÎÊý£ºÎÞ
·µ»Ø  Öµ£ºÎÞ
**************************************************************************/
void USART2_IRQHandler(void)
{	
	if(USART2->SR&(1<<5))//½ÓÊÕµ½Êý¾Ý
	{	  
	  unsigned char uart_receive;//À¶ÑÀ½ÓÊÕÏà¹Ø±äÁ¿
	  uart_receive=USART2->DR; 
	  //printf("%c",uart_receive);//ÏòÉÏÎ»»ú·¢ËÍÊý¾
		//OLED_ShowString(105,0,&uart_receive);
	  switch(uart_receive)
	  {
	  case 'a'://PWM +=ÊÇÕ¼¿Õ±È±äÐ¡£¬-=ÊÇÕ¼¿Õ±È±ä´ó
			if(floagzuo==0){
			Moto3 = PID_CONTROL - 150;
		Moto4 = PID_CONTROL + 150;
			floagzuo=1;
			}
		  break;
	  case 'b':
			if(floagyou==0){
			Moto3 = PID_CONTROL + 150;
		Moto4 = PID_CONTROL - 150;
			floagyou=1;
			}
		  break;
	  case 'c':
			if(floagzuo==1){
			Moto1=PID_CONTROL - PID_kp*(Pitch+0) - gyro[1]*PID_kd - PID_ki*Pitch_integ;//Æ½ºâµÄPD¿ØÖÆ  
			Moto2=PID_CONTROL + PID_kp*(Pitch+0) + gyro[1]*PID_kd + PID_ki*Pitch_integ;//¿É×ÔÐÐ¼ÓÈëZÖáP¿ØÖÆÒÖÖÆ×ÔÐý 
			Moto3=PID_CONTROL + PID_kpy*(Roll-0) + gyro[0]*PID_kdy + PID_kiy*Roll_integ;////////////// 
			Moto4=PID_CONTROL - PID_kpy*(Roll-0) - gyro[0]*PID_kdy - PID_kiy*Roll_integ;////////////// 	
			}
			if(floagyou==1){
			Moto1=PID_CONTROL - PID_kp*(Pitch+0) - gyro[1]*PID_kd - PID_ki*Pitch_integ;//Æ½ºâµÄPD¿ØÖÆ  
			Moto2=PID_CONTROL + PID_kp*(Pitch+0) + gyro[1]*PID_kd + PID_ki*Pitch_integ;//¿É×ÔÐÐ¼ÓÈëZÖáP¿ØÖÆÒÖÖÆ×ÔÐý 
			Moto3=PID_CONTROL + PID_kpy*(Roll-0) + gyro[0]*PID_kdy + PID_kiy*Roll_integ;////////////// 
			Moto4=PID_CONTROL - PID_kpy*(Roll-0) - gyro[0]*PID_kdy - PID_kiy*Roll_integ;////////////// 	
			}
			if(floagqian==1){
				Moto1=PID_CONTROL - PID_kp*(Pitch+0) - gyro[1]*PID_kd - PID_ki*Pitch_integ;//Æ½ºâµÄPD¿ØÖÆ  
			Moto2=PID_CONTROL + PID_kp*(Pitch+0) + gyro[1]*PID_kd + PID_ki*Pitch_integ;//¿É×ÔÐÐ¼ÓÈëZÖáP¿ØÖÆÒÖÖÆ×ÔÐý 
			Moto3=PID_CONTROL + PID_kpy*(Roll-0) + gyro[0]*PID_kdy + PID_kiy*Roll_integ;////////////// 
			Moto4=PID_CONTROL - PID_kpy*(Roll-0) - gyro[0]*PID_kdy - PID_kiy*Roll_integ;////////////// 	
			}
			if(floaghou==1){
			Moto1=PID_CONTROL - PID_kp*(Pitch+0) - gyro[1]*PID_kd - PID_ki*Pitch_integ;//Æ½ºâµÄPD¿ØÖÆ  
			Moto2=PID_CONTROL + PID_kp*(Pitch+0) + gyro[1]*PID_kd + PID_ki*Pitch_integ;//¿É×ÔÐÐ¼ÓÈëZÖáP¿ØÖÆÒÖÖÆ×ÔÐý 
			Moto3=PID_CONTROL + PID_kpy*(Roll-0) + gyro[0]*PID_kdy + PID_kiy*Roll_integ;////////////// 
			Moto4=PID_CONTROL - PID_kpy*(Roll-0) - gyro[0]*PID_kdy - PID_kiy*Roll_integ;////////////// 	
			}
	  	break;
		
		case 'd'://+KP   PID_kpy+=0.1;
		  if(PID_kpy<100)		  {			PID_kp+=1;	  }		  break;
	  case 'g'://-KP   PID_kpy-=0.1;
		  if(PID_kpy>0)		  {			PID_kp-=1;	  }		  break;
			
		case 'e'://+I
			if(PID_ki<20)			{			PID_ki+=0.1;			}	  	break;
		case 'h'://-I
			if(PID_ki>0)			{			PID_ki-=0.1;			}	  	break;
		case 'f'://+D   PID_kdy+=0.1;
			if(PID_kdy<100)			{			PID_kd+=0.01;			}	  	break;
		case 'i'://-D      
			if(PID_kdy>0)			{			PID_kd-=0.01;			}	  	break;
			
		case 'j'://+PWMA
			
			if(floagqian==0){
				
			   Moto1 = PID_CONTROL - 150;
		     Moto2 = PID_CONTROL + 150;
				floagqian=1;
			}
			break;
		case 'k'://-PWMA
			if(floaghou==0){
			Moto1 = PID_CONTROL + 150;
		Moto2 = PID_CONTROL - 150;
				floaghou=1;
			}
			break;
		case 'l'://+PWMB
			PWMB+=10;
			break;
		case 'm'://-PWMB
			PID_CONTROL+=1;
			break;
		case 'u'://+PWMC
			PID_CONTROL-=1;
			break;
		case 'v'://-PWMC
			PID_CONTROL+=10;
			break;
		case 'o':
			if(PID_OK==0)
			{
				if(PWMA<=17960)
				{
					PWMA+=40;
					PWMB+=40;
					PWMC+=40;
					PWMD+=40;
				}
			}
			if(PID_OK==1)
			{
				if(PID_CONTROL<=17960)
				{
					PID_CONTROL+=20;
				}
			}
	    break;
	  case 'p':  //Æô¶¯PID
	  	PID_OK=1;
			//PID_CONTROL=17400;
	  	break;
		case 'q':
			if(PID_OK==0)
			{
				if(PWMA>=16040)
				{
					PWMA-=40;
					PWMB-=40;
					PWMC-=40;
					PWMD-=40;
				}
			}
			if(PID_OK==1)
			{
				if(PID_CONTROL>=16040)
				{
						PID_CONTROL-=20;
				}
			}
		  break;
			
	  case 's':
	    PID_CONTROL-=10;		break;	
		
		case 'z':
			PID_CONTROL+=5;		break;
		
		case 'y':
			PID_CONTROL-=5; 	break;
		case 'x':
      PID_OK=0;
      PWMA=18000;		PWMB=18000; 		PWMC=18000;		PWMD=18000;		
			break;
	  default:
	    break;
	  }
	}  											 
} 


