# **STM32F103C8T6 项目所用GPIO功能分配表**

## **摄像头接口 (OV2640)**
| 引脚号 | GPIO | 功能 | 备注 |
|--------|------|------|------|
| 18 | PB0 | 摄像头数据位 D0 | DCMI_D0 |
| 19 | PB1 | 摄像头数据位 D1 | DCMI_D1 |
| 20 | PB2 | 摄像头数据位 D2 | DCMI_D2 |
| 39 | PB3 | 摄像头数据位 D3 | DCMI_D3 |
| 40 | PB4 | 摄像头数据位 D4 | DCMI_D4 |
| 41 | PB5 | 摄像头数据位 D5 | DCMI_D5 |
| 42 | PB6 | 摄像头数据位 D6 | DCMI_D6 |
| 43 | PB7 | 摄像头数据位 D7 | DCMI_D7 |
| 29 | PA8 | 摄像头像素时钟 PCLK | DCMI_PIXCLK |
| 45 | PB8 | 摄像头帧同步 VSYNC | DCMI_VSYNC |
| 46 | PB9 | 摄像头行同步 HREF | DCMI_HSYNC |
| 25 | PB10 | 摄像头I2C时钟 SCL | I2C2_SCL |
| 27 | PB11 | 摄像头I2C数据 SDA | I2C2_SDA |

## **超声波传感器 (HC-SR04)**
| 引脚号 | GPIO | 功能 | 备注 |
|--------|------|------|------|
| 12 | PA2 | 超声波触发信号 Trig | GPIO_Output |
| 13 | PA3 | 超声波回波信号 Echo | GPIO_Input, TIM2_CH4/EXTI3 |

## **舵机控制 (SG90)**
| 引脚号 | GPIO | 功能 | 备注 |
|--------|------|------|------|
| 16 | PA6 | 水平舵机 PWM信号 | TIM3_CH1 |
| 17 | PA7 | 垂直舵机 PWM信号 | TIM3_CH2 |

## **调试与系统**
| 引脚号 | GPIO | 功能 | 备注 |
|--------|------|------|------|
| 34 | PA13 | SWD调试数据线 | SWDIO |
| 37 | PA14 | SWD调试时钟线 | SWCLK |
| 7  | NRST | 系统复位 | 外部复位电路 |
| 44 | BOOT0 | 启动模式选择 | 10k下拉到GND |

## **电源与地**
| 引脚号 | GPIO | 功能 | 备注 |
|--------|------|------|------|
| 24,36,48 | VDD | 3.3V电源输入 | 各接100nF滤波电容 |
| 23,35,47 | VSS | 电源地 | |
| 9 | VDDA | 模拟电源 | 3.3V经LC滤波 |
| 8 | VSSA | 模拟地 | |

## **时钟**
| 引脚号 | GPIO | 功能 | 备注 |
|--------|------|------|------|
| 5 | PD0/OSC_IN | 外部晶振输入 | 8MHz晶体 |
| 6 | PD1/OSC_OUT | 外部晶振输出 | 8MHz晶体 |

---

### **代码配置建议**
```c
// 超声波引脚定义
#define TRIG_PIN    GPIO_PIN_2
#define TRIG_PORT   GPIOA
#define ECHO_PIN    GPIO_PIN_3
#define ECHO_PORT   GPIOA

// 舵机引脚定义
#define SERVO_H_PIN GPIO_PIN_6  // 水平舵机
#define SERVO_V_PIN GPIO_PIN_7  // 垂直舵机
#define SERVO_PORT  GPIOA

// 摄像头I2C定义
#define CAM_I2C     I2C2
#define CAM_SCL_PIN GPIO_PIN_10
#define CAM_SDA_PIN GPIO_PIN_11
#define CAM_I2C_PORT GPIOB

// DCMI接口定义
#define DCMI_PORT   GPIOB
#define DCMI_PINS   GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                    GPIO_PIN_8 | GPIO_PIN_9
#define DCMI_PCLK_PIN GPIO_PIN_8
#define DCMI_PCLK_PORT GPIOA
```

### **未使用引脚处理**
- **所有未使用的GPIO**：设置为模拟输入模式（避免浮空）
- **PC13**：可连接LED作为状态指示
- **VBAT**：接3.3V电源（通过100nF电容滤波）

