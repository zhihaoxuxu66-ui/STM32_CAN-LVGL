#ifndef __DELAY_H__
#define __DELAY_H__
#include "main.h"

void delay_init(void);                 // 初始化（开启 DWT 周期计数器）
void delay_ms(uint16_t nms);           // 毫秒延时（基于 SysTick / HAL_Delay）
void delay_us(uint32_t nus);           // 微秒延时（基于 DWT->CYCCNT，精度 1/72M ≈ 13.9ns）

#endif
