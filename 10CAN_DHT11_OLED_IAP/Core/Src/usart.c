/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "main.h"
static uint8_t ota_flag = 0; 
static uint8_t uart1_rec_byte = 0; 
static UART_RX_typeDef uart1_rx_data; 
uint8_t u_buf[256];
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void uart_receive_enable()     //使能中断接收
{
  HAL_UART_Receive_IT(&huart1, (uint8_t*)&uart1_rec_byte, 1);				//串口1接收中断开启
}

/**
 * @brief 串口接收ota标志
 * 
 * @return uint8_t 
 */
uint8_t uart_get_ota_flag()
{
  return ota_flag;
}


/*
    串口升级协议：
    |  HEAD  |  LEN　       |  DATA  |  TAIL
    |  0xAF  |  除头尾的长度  |  DATA  |  0xFA

*/

/**
 * @brief 串口接收回调函数
 * 
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* 判断是哪个串口触发的中断 */
  if(huart ->Instance == USART1)
  { 
    uart1_rx_data.rx_buf[uart1_rx_data.rx_cnt] =  uart1_rec_byte;
    uart1_rx_data.rx_cnt ++;

  
		if(uart1_rx_data.rx_buf[0] == 0xAF)
		{
			if(uart1_rx_data.rx_cnt >= 48)      //数据过长
			{ 
				uart1_rx_data.rx_cnt = 0;
        printf("ota data len error\r\n");
			}	
      else
      {
        if(uart1_rx_data.rx_cnt > 2)
        {
          uint8_t data_len = uart1_rx_data.rx_buf[1];

          if((data_len > 0) && (data_len < 50))     //数据长度合法检测
          {

            if(uart1_rx_data.rx_cnt >= (data_len + 3))
            {
              if(uart1_rx_data.rx_buf[data_len + 2] == 0xFA)     //数据尾
              {
                ota_flag = 1;     //使能升级标志  
								uart1_rx_data.rx_cnt = 0;
                printf("OTA FLAG SET, VER:%d.%d.%d\r\n",uart1_rx_data.rx_buf[2],uart1_rx_data.rx_buf[3],uart1_rx_data.rx_buf[4]);
              }
              else
              {
                uart1_rx_data.rx_cnt = 0;
                printf("ota data tail error\r\n");
              }
            }
          }
          else
          {
            uart1_rx_data.rx_cnt = 0;
            printf("ota data len error\r\n");
          }
        }
      }	
		}
		else
		{
		uart1_rx_data.rx_cnt = 0;
		}


    HAL_UART_Receive_IT(&huart1, &uart1_rec_byte, 1);
  }
}

/* USER CODE END 1 */
