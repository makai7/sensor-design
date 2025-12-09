/* Core/Inc/vision.h */
#ifndef __VISION_H
#define __VISION_H

#include "main.h"

// 视觉结果结构体
typedef struct {
    uint16_t x;         // 目标的 X 坐标 (0-160)
    uint16_t y;         // 目标的 Y 坐标 (0-120)
    uint16_t size;      // 目标的大小 (检测到的像素点数)
    uint8_t detected;   // 1: 找到目标, 0: 没找到
} Vision_Result_t;

// 函数声明
void Vision_Init(void);
Vision_Result_t Vision_Process_Frame(void); // 捕捉一帧并返回结果

#endif