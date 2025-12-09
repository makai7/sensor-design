/* Core/Src/vision.c */
#include "vision.h"
#include "ov2640.h"

// 引脚定义 (参考 IO口_new.md)
#define VSYNC_PIN   GPIO_PIN_8
#define HREF_PIN    GPIO_PIN_9
#define PCLK_PIN    GPIO_PIN_8  // 注意：PCLK在PA8，不在GPIOB
#define DATA_PORT   GPIOB       // PB0-PB7 是数据

// 颜色阈值 (RGB565格式: RRRRR GGGGGG BBBBB)
// 红色物体通常 R 分量大，G/B 分量小
// 这是一个简单的“红色过滤器”
#define IS_RED_PIXEL(p)  ( ((p & 0xF800) >> 11) > 20 && ((p & 0x07E0) >> 5) < 10 ) 

void Vision_Init(void) {
    // 这里的GPIO初始化已经在 gpio.c 里做过了，所以可以空着
    // 或者做一些变量复位
}

// 读取一个像素 (RGB565是两个字节)
// 警告：这个函数必须极快，否则会跟不上摄像头速度
static inline uint16_t Read_Pixel(void) {
    uint8_t high_byte, low_byte;
    
    // 1. 等待 PCLK 上升沿 (读取高字节)
    while((GPIOA->IDR & GPIO_PIN_8) == 0); 
    high_byte = (uint8_t)(GPIOB->IDR & 0xFF); // 读取 PB0-PB7
    while((GPIOA->IDR & GPIO_PIN_8) != 0);    // 等待 PCLK 变低
    
    // 2. 等待 PCLK 上升沿 (读取低字节)
    while((GPIOA->IDR & GPIO_PIN_8) == 0);
    low_byte = (uint8_t)(GPIOB->IDR & 0xFF);
    while((GPIOA->IDR & GPIO_PIN_8) != 0);

    return (high_byte << 8) | low_byte;
}

Vision_Result_t Vision_Process_Frame(void) {
    Vision_Result_t res = {0, 0, 0, 0};
    uint32_t sum_x = 0;
    uint32_t sum_y = 0;
    uint32_t count = 0;
    
    // 1. 等待 VSYNC 信号 (帧开始)
    // VSYNC 通常是高脉冲或低脉冲，OV2640默认高有效(取决于配置)
    // 这里假设等待 VSYNC 变高 (开始新的一帧)
    uint32_t timeout = 1000000;
    while((GPIOB->IDR & VSYNC_PIN) == 0) {
        if(--timeout == 0) return res; // 超时返回空结果
    }
    while((GPIOB->IDR & VSYNC_PIN) != 0); // 等待 VSYNC 结束，图像开始传输

    // 2. 扫描图像 (假设分辨率 160x120)
    for (int y = 0; y < 120; y++) {
        
        // 等待 HREF 变高 (行开始)
        timeout = 50000;
        while((GPIOB->IDR & HREF_PIN) == 0) {
             if(--timeout == 0) break; 
        }
        
        // 读取这一行的 160 个像素
        for (int x = 0; x < 160; x++) {
            uint16_t pixel = Read_Pixel();
            
            // 3. 实时颜色识别 (如果是红色)
            if (IS_RED_PIXEL(pixel)) {
                sum_x += x;
                sum_y += y;
                count++;
            }
        }
        
        // 等待 HREF 变低 (行结束)
        while((GPIOB->IDR & HREF_PIN) != 0);
    }

    // 3. 计算重心
    if (count > 10) { // 至少看到10个红色点才算找到
        res.x = sum_x / count;
        res.y = sum_y / count;
        res.size = count;
        res.detected = 1;
    }

    return res;
}