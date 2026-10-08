/**
  ******************************************************************************
  * @file    delay.c
  * @brief   兼容正点原子风格 delay_ms / delay_us 的 HAL 实现
  * @note
  *   - delay_ms: 用 SysTick + HAL_Delay（毫秒级，中断里别用）
  *   - delay_us: 用 Cortex-M3 的 DWT->CYCCNT 周期计数器
  *               72MHz 下 1us ≈ 72 个 cycle
  *   - delay_init 必须在 delay_us 之前调用一次（放在 main 的 HAL_Init 之后即可）
  ******************************************************************************
  */

#include "delay.h"

/* DWT (Data Watchpoint and Trace) 寄存器定义 —— CMSIS 没全列出来，手动补一下 */
#define DWT_CR        (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT    (*(volatile uint32_t *)0xE0001004)
#define DEM_CR        (*(volatile uint32_t *)0xE000EDFC)
#define TRCENA        (1 << 24)   /* DEM_CR 的 TRCENA 位：使能 DWT / ITM */
#define DWT_CYCCNTENA (1 << 0)    /* DWT_CR  的 CYCCNTENA 位：使能 CYCCNT */

/**
 * @brief  初始化 DWT 周期计数器
 * @note   必须在 SystemClock_Config() 之后调用（默认 72MHz）
 */
void delay_init(void)
{
    /* 使能 TRCENA */
    DEM_CR |= TRCENA;
    /* 计数器清零并使能 */
    DWT_CYCCNT = 0;
    DWT_CR |= DWT_CYCCNTENA;
}

/**
 * @brief  毫秒级延时
 * @param  nms  毫秒数
 */
void delay_ms(uint16_t nms)
{
    HAL_Delay(nms);
}

/**
 * @brief  微秒级延时
 * @param  nus  微秒数
 * @note   HAL_Delay 走 SysTick 1ms 一跳，不够细；这里用 DWT 周期计数器
 *         SystemCoreClock 来自 system_stm32f1xx.c，默认 72MHz
 */
void delay_us(uint32_t nus)
{
    uint32_t start = DWT_CYCCNT;
    /* 需要的 cycle 数 = nus * (SystemCoreClock / 1000000) */
    uint32_t ticks = nus * (SystemCoreClock / 1000000U);
    /* 防止 32 位溢出 */
    if (ticks == 0) ticks = 1;

    /* 等待 CYCCNT 达到目标值（处理 CYCCNT 32 位回绕：差值为无符号减法） */
    while ((DWT_CYCCNT - start) < ticks) { __NOP(); }
}
