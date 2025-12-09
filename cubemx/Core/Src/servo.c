/* Core/Src/servo.c */
#include "servo.h"
#include "tim.h" // 引入CubeMX生成的定时器定义

// 初始化：启动PWM信号
void Servo_Init(void) {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 启动水平舵机
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // 启动垂直舵机
}

// 设置PWM值 (直接操作寄存器，比HAL库函数快)
void Servo_Set_PWM(uint8_t servo_id, uint16_t pwm_value) {
    // 限幅保护，防止舵机撞坏机械结构
    if (pwm_value < SERVO_MIN_PWM) pwm_value = SERVO_MIN_PWM;
    if (pwm_value > SERVO_MAX_PWM) pwm_value = SERVO_MAX_PWM;

    if (servo_id == 0) {
        // 水平舵机 (PA6 -> Channel 1)
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_value);
    } else {
        // 垂直舵机 (PA7 -> Channel 2)
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwm_value);
    }
}