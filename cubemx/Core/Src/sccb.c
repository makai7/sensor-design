/* Core/Src/sccb.c */
#include "sccb.h"

// 简单的微秒延时，确保时序稳定
static void SCCB_Delay(void) {
    for(volatile int i = 0; i < 100; i++); 
}

void SCCB_Init(void) {
    SCCB_SCL_H();
    SCCB_SDA_H();
    SCCB_SDA_OUT();
}

void SCCB_Start(void) {
    SCCB_SDA_H();
    SCCB_SCL_H();
    SCCB_Delay();
    SCCB_SDA_L();
    SCCB_Delay();
    SCCB_SCL_L();
}

void SCCB_Stop(void) {
    SCCB_SDA_L();
    SCCB_SCL_H();
    SCCB_Delay();
    SCCB_SDA_H();
    SCCB_Delay();
}

void SCCB_No_Ack(void) {
    SCCB_Delay();
    SCCB_SDA_H();
    SCCB_SCL_H();
    SCCB_Delay();
    SCCB_SCL_L();
    SCCB_Delay();
    SCCB_SDA_L();
    SCCB_Delay();
}

uint8_t SCCB_Write_Byte(uint8_t data) {
    uint8_t i;
    for(i = 0; i < 8; i++) {
        if(data & 0x80) SCCB_SDA_H();
        else SCCB_SDA_L();
        data <<= 1;
        SCCB_Delay();
        SCCB_SCL_H();
        SCCB_Delay();
        SCCB_SCL_L();
    }
    
    // 等待ACK
    SCCB_SDA_IN(); // 切换为输入
    SCCB_Delay();
    SCCB_SCL_H();
    SCCB_Delay();
    if(SCCB_READ_SDA()) {
        SCCB_SCL_L();
        SCCB_SDA_OUT();
        return 1; // 发送失败（无ACK）
    }
    SCCB_SCL_L();
    SCCB_SDA_OUT(); // 切换回输出
    return 0; // 发送成功
}

uint8_t SCCB_Read_Byte(void) {
    uint8_t i, data = 0;
    SCCB_SDA_IN();
    for(i = 0; i < 8; i++) {
        SCCB_Delay();
        SCCB_SCL_H();
        data <<= 1;
        if(SCCB_READ_SDA()) data++;
        SCCB_Delay();
        SCCB_SCL_L();
    }
    SCCB_SDA_OUT();
    return data;
}

// === 高层写寄存器 ===
// OV2640写地址：0x60
uint8_t SCCB_WR_Reg(uint8_t reg, uint8_t data) {
    SCCB_Start();
    if(SCCB_Write_Byte(0x60)) { SCCB_Stop(); return 1; } // 写设备地址
    if(SCCB_Write_Byte(reg))  { SCCB_Stop(); return 1; } // 写寄存器地址
    if(SCCB_Write_Byte(data)) { SCCB_Stop(); return 1; } // 写数据
    SCCB_Stop();
    return 0;
}

// === 高层读寄存器 ===
uint8_t SCCB_RD_Reg(uint8_t reg) {
    uint8_t val = 0;
    // 1. 写寄存器地址
    SCCB_Start();
    SCCB_Write_Byte(0x60); 
    SCCB_Write_Byte(reg);
    SCCB_Stop();
    
    // 2. 读数据
    SCCB_Start();
    SCCB_Write_Byte(0x61); // 0x61 表示读
    val = SCCB_Read_Byte();
    SCCB_No_Ack();
    SCCB_Stop();
    return val;
}