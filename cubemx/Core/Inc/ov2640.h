/* Core/Inc/ov2640.h */
#ifndef __OV2640_H
#define __OV2640_H

#include "main.h"
#include "sccb.h"

// OV2640 ID寄存器
#define OV2640_PID  0x0A
#define OV2640_VER  0x0B
#define OV2640_MIDH 0x1C
#define OV2640_MIDL 0x1D

uint8_t OV2640_Init(void); // 返回0表示成功，1表示失败

#endif