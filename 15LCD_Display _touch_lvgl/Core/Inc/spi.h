/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */
//==== 兼容正点原子 XPT2046 触摸驱动的 SPI 辅助函数 ====
uint8_t SPI1_ReadWriteByte(uint8_t TxData);
void    SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler);

// 标准库风格的波特率分频宏（映射到 HAL 的 SPI_BAUDRATEPRESCALER_x）
#ifndef SPI_BaudRatePrescaler_2
#define SPI_BaudRatePrescaler_2    SPI_BAUDRATEPRESCALER_2
#define SPI_BaudRatePrescaler_4    SPI_BAUDRATEPRESCALER_4
#define SPI_BaudRatePrescaler_8    SPI_BAUDRATEPRESCALER_8
#define SPI_BaudRatePrescaler_16   SPI_BAUDRATEPRESCALER_16
#define SPI_BaudRatePrescaler_32   SPI_BAUDRATEPRESCALER_32
#define SPI_BaudRatePrescaler_64   SPI_BAUDRATEPRESCALER_64
#define SPI_BaudRatePrescaler_128  SPI_BAUDRATEPRESCALER_128
#define SPI_BaudRatePrescaler_256  SPI_BAUDRATEPRESCALER_256
#endif
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

