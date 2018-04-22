#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#define KEY PAin(15)
void KEY_Init(void);          //按键初始化
u8 click_N_Double (u8 time);  //单击按键扫描和双击按键扫描
u8 click(void);               //单击按键扫描
#endif 
