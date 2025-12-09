/* Core/Inc/servo.h */
#ifndef __SERVO_H
#define __SERVO_H

#include "main.h" // 包含 HAL 库定义

// 舵机物理限制定义
#define SERVO_MIN_PWM 500   // 0.5ms -> 0度
#define SERVO_MAX_PWM 2500  // 2.5ms -> 180度
#define SERVO_MID_PWM 1500  // 1.5ms -> 90度

// 函数声明
void Servo_Init(void);
void Servo_Set_PWM(uint8_t servo_id, uint16_t pwm_value); // 0:水平, 1:垂直

#endif