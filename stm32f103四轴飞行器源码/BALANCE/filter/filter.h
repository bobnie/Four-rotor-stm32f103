#ifndef __FILTER_H
#define __FILTER_H
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/*一维kalman滤波结构体定义,A=1,B=0,H=1*/
typedef struct Kalman_filter
{
	float C_last;				    /*上次预测过程协方差矩阵 C(k|k-1)*/
	float X_last;				    /*系统状态预测矩阵，列矩阵*/
	
	float Q;						/*过程噪声协方差*/
	float R;						/*量测噪声协方差*/
	
	float K;						/*卡尔曼增益，列矩阵*/
	float X;						/*最优估计输出矩阵，列矩阵*/
	float C;						/*最优估计协方差矩阵C(k|k)*/
                            
	float input;				    /*量测值，即Z(k)*/
}
kal_filter;
extern float angle, angle_dot; 	
void Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo(float angle_m, float gyro_m);
float kalman_filter(kal_filter* k_flt, float input);
#endif
