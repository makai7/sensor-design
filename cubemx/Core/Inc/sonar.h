/* Core/Inc/sonar.h */
#ifndef __SONAR_H
#define __SONAR_H

#include "main.h"

// 引脚定义 (对应 CubeMX 生成的宏)
#define TRIG_PORT SONAR_TRIG_GPIO_Port
#define TRIG_PIN  SONAR_TRIG_Pin
#define ECHO_PORT GPIOA            // 注意：CubeMX可能没生成 ECHO 的 Port 宏，手动指定 GPIOA
#define ECHO_PIN  SONAR_ECHO_Pin

// 函数声明
void Sonar_Init(void);       // 初始化DWT计数器用于微秒延时
float Sonar_Measure(void);   // 返回距离，单位 cm

#endif