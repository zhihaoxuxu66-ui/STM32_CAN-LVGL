/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
//======== 位带别名宏（Cortex-M3 支持，HAL 工程里替代标准库的 PAout/PBin）========
// 公式：别名地址 = 0x42000000 + ((外设地址-0x40000000)<<5) + (bit<<2)
#define BITBAND(addr, bitnum)  ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define PAout(n)   (*(volatile uint32_t *)BITBAND((uint32_t)&GPIOA->ODR, n))   // PA输出
#define PAin(n)    (*(volatile uint32_t *)BITBAND((uint32_t)&GPIOA->IDR, n))   // PA输入
#define PBout(n)   (*(volatile uint32_t *)BITBAND((uint32_t)&GPIOB->ODR, n))   // PB输出
#define PBin(n)    (*(volatile uint32_t *)BITBAND((uint32_t)&GPIOB->IDR, n))   // PB输入
/* USER CODE END EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RES_Pin 				GPIO_PIN_0
#define RES_GPIO_Port 	GPIOA
#define DC_Pin 					GPIO_PIN_1
#define DC_GPIO_Port 		GPIOA
#define BLK_Pin 				GPIO_PIN_2
#define BLK_GPIO_Port 	GPIOA
#define CS1_Pin 				GPIO_PIN_3
#define CS1_GPIO_Port 	GPIOA
#define CS2_Pin 				GPIO_PIN_4
#define CS2_GPIO_Port 	GPIOA
#define PEN_Pin 				GPIO_PIN_0
#define PEN_GPIO_Port 	GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
