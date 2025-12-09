/* Core/Inc/sccb.h */
#ifndef __SCCB_H
#define __SCCB_H

#include "main.h"

// 引脚定义 (模拟I2C)
#define SCCB_SCL_PIN    GPIO_PIN_10
#define SCCB_SCL_PORT   GPIOB
#define SCCB_SDA_PIN    GPIO_PIN_11
#define SCCB_SDA_PORT   GPIOB

// 宏函数：控制引脚电平
#define SCCB_SCL_H()    HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET)
#define SCCB_SCL_L()    HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_RESET)
#define SCCB_SDA_H()    HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET)
#define SCCB_SDA_L()    HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_RESET)

#define SCCB_READ_SDA() HAL_GPIO_ReadPin(SCCB_SDA_PORT, SCCB_SDA_PIN)

// 关键宏：SDA方向切换 (模拟I2C需要频繁切换输入输出)
// F103寄存器操作速度最快
#define SCCB_SDA_IN()  {GPIOB->CRH &= 0xFFFF0FFF; GPIOB->CRH |= 0x00008000;} // PB11 Input Mode
#define SCCB_SDA_OUT() {GPIOB->CRH &= 0xFFFF0FFF; GPIOB->CRH |= 0x00003000;} // PB11 Output 50MHz OD

// 函数声明
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_Write_Byte(uint8_t data);
uint8_t SCCB_Read_Byte(void);

// 摄像头寄存器操作接口
uint8_t SCCB_WR_Reg(uint8_t reg, uint8_t data);
uint8_t SCCB_RD_Reg(uint8_t reg);

#endif