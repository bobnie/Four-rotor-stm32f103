#include "sys.h"
/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
此代码的控制算法是通过验证的，但是请同学们不要直接使用到四轴上面，
因为不同的电调，不同的解法，螺旋桨的安装都可能影响到最终的控制系统
请根据程序的底层和控制代码与您的硬件一个一个测试匹配
**************************************************************************/
u8 Way_Angle=1;                             //获取角度的算法，1:DMP2：卡尔曼 可以改成3 即互补滤波，
u8 Flag_Show=0,Flag_Stop=1;                 //显示标志位 启停标志位 显示屏非常影响处理速度
int Temperature;                            //显示温度
float Moto1,Moto2,Moto3,Moto4;               //电机PWM变量 应是Motor的 向Moto致敬	
int PID_OK=0;
int PID_CONTROL=17500;
float PID_kp=9.15;//3.3
float PID_kpy=9.50;//4.4
float PID_ki=0.1;
float PID_kiy=0.1;
float PID_kd=1.62;//1.5
float PID_kdy=2.00;//2.0
int gyro_count=0;
int in_10ms=0;
long gyro_0_last=0;
long gyro_1_last=0;
short gyro_0[8]={0};
short gyro_1[8]={0};
int PID_limit=400;
int Compensate=10;
char pitch_store[1]={0};
int pitch_count=0;
int main(void)
{ 
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
	JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
	JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试

	//开机四个50%占空比的电平解锁电调
	KEY_Init();                     //按键初始化
	MiniBalance_PWM_Init(19999,35); //飞控输出电压的频率周期设置。  默认：999~200hz	   40000~50hz
	//PWMA=10000;		PWMB=10000;		PWMC=10000;			PWMD=10000;
	//delay_ms(50);
	//电平范围：6.25%~10%   2500~4000 	首先设置为高电平  1000~2000
	//PWMA=16000;		PWMB=16000; 		PWMC=16000;		PWMD=16000;
	//delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
	PWMA=18000;		PWMB=18000; 		PWMC=18000;		PWMD=18000;
	//PWMA=5000;		PWMB=10000; 		PWMC=15000;		PWMD=20000;  //测试看看各个PWM对应哪个引脚
	
  Timer1_Init(49,14399);	 //每10ms进一次中断函数

	LED_Init();                     //初始化与 LED 连接的硬件接口
	KEY_Init();                     //按键初始化
	OLED_Init();                    //OLED初始化
	uart_init(72,115200);           //初始化串口1	 发送到电脑的串口
  uart2_init(36,9600);            //串口2初始化    蓝牙接收
	IIC_Init();                     //模拟IIC初始化
  MPU6050_initialize();           //=====MPU6050初始化	
	DMP_Init();                     //初始化DMP     
	

	while(1)
	{
	 	if(Way_Angle==1)                     //在主函数读取
		{
			Read_DMP();                      //===读取角速度和倾角
		}  
		if(Flag_Stop==1)//使用OLED显示屏和PC端串口调试助手
		{
			Way_Angle=1;//使用DMP
			if(Pitch<0)		OLED_ShowNumber(45,40,Pitch+360,3,12);
			else					OLED_ShowNumber(45,40,Pitch,3,12);
			
			OLED_ShowNumber(0,40,PID_kp*100,3,12);
			OLED_ShowNumber(0,50,PID_kd*100,3,12);
			
			//Temperature=Read_Temperature();  //===读取MPU6050内置温度传感器数据，近似表示主板温度。	
			//oled_show(); //===显示屏打开
			printf("%4d\r\n",gyro[2]);
			//printf("X轴倾角%f  Y轴倾角%f   \r\n",Pitch,Roll);//向上位机发送数据  x轴 Pitch  y轴 Roll
			//printf("GYRO[0]%6d     GYRO[1]%6d      GYRO[2]%6d   \r\n",gyro[0],gyro[1],gyro[2]);//向上位机发送数据  x轴 Pitch  y轴 Roll
			//}
		}	
	} 
}
