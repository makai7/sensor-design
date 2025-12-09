/* Core/Src/sonar.c */
#include "sonar.h"

// === 微秒延时辅助函数 (基于 DWT 计数器) ===
// STM32F103 主频 72MHz，意味着 1us = 72 个时钟周期
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 开启 DWT 追踪
    DWT->CYCCNT = 0;                                // 清零计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // 启动计数器
}

void delay_us(uint32_t us) {
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (SystemCoreClock / 1000000); // 72MHz下，1us=72ticks

    while (DWT->CYCCNT - startTick < delayTicks);
}

// === 超声波驱动实现 ===

void Sonar_Init(void) {
    // 1. 初始化微秒计时器
    DWT_Init(); 
    // 2. 确保 Trig 拉低，准备发射
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

float Sonar_Measure(void) {
    uint32_t local_time = 0;
    
    // 1. 触发信号：发送 10us 高电平
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(12); // 稍微多一点，保证大于10us
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

    // 2. 等待 Echo 变高 (等待回波开始)
    // 增加超时保护，防止程序死锁 (比如线断了)
    uint32_t timeout = 100000; 
    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET) {
        if (timeout-- == 0) return -1.0f; // 超时返回错误
    }

    // 3. 测量 Echo 高电平时间
    uint32_t startTick = DWT->CYCCNT;
    timeout = 100000;
    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET) {
        if (timeout-- == 0) break;
    }
    uint32_t endTick = DWT->CYCCNT;

    // 4. 计算时间差 (Ticks) 并转换为微秒
    uint32_t ticks = endTick - startTick;
    float time_us = (float)ticks / (SystemCoreClock / 1000000);

    // 5. 计算距离: 距离 = 时间(us) / 58.0 (声速公式简化版)
    float distance = time_us / 58.0f;

    return distance;
}