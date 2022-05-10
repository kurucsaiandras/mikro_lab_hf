/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_accelerometer.h"
#include "bsp_glcd.h"
#include "graphics.h"
#include "bsp_uart.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


extern prog_state state;

static int leftButton_released;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define SPI2_CS_Pin GPIO_PIN_0
#define SPI2_CS_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define DB3_Pin GPIO_PIN_6
#define DB3_GPIO_Port GPIOA
#define DB2_Pin GPIO_PIN_7
#define DB2_GPIO_Port GPIOA
#define GLCD_E_Pin GPIO_PIN_5
#define GLCD_E_GPIO_Port GPIOC
#define DB4_Pin GPIO_PIN_1
#define DB4_GPIO_Port GPIOB
#define GLCD_EN_Pin GPIO_PIN_2
#define GLCD_EN_GPIO_Port GPIOB
#define GLCD_RW_Pin GPIO_PIN_12
#define GLCD_RW_GPIO_Port GPIOB
#define DB7_Pin GPIO_PIN_13
#define DB7_GPIO_Port GPIOB
#define DB6_Pin GPIO_PIN_14
#define DB6_GPIO_Port GPIOB
#define DB5_Pin GPIO_PIN_15
#define DB5_GPIO_Port GPIOB
#define GLCD_CS2_Pin GPIO_PIN_6
#define GLCD_CS2_GPIO_Port GPIOC
#define DB0_Pin GPIO_PIN_7
#define DB0_GPIO_Port GPIOC
#define GLCD_CS1_Pin GPIO_PIN_8
#define GLCD_CS1_GPIO_Port GPIOC
#define GLCD_RESET_Pin GPIO_PIN_11
#define GLCD_RESET_GPIO_Port GPIOA
#define GLCD_DI_Pin GPIO_PIN_12
#define GLCD_DI_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SPI2_INT1_Pin GPIO_PIN_15
#define SPI2_INT1_GPIO_Port GPIOA
#define BUTTON_3_Pin GPIO_PIN_10
#define BUTTON_3_GPIO_Port GPIOC
#define BUTTON_4_Pin GPIO_PIN_12
#define BUTTON_4_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define DB1_Pin GPIO_PIN_6
#define DB1_GPIO_Port GPIOB
#define SPI2_INT2_Pin GPIO_PIN_7
#define SPI2_INT2_GPIO_Port GPIOB
#define BUTTON_1_Pin GPIO_PIN_8
#define BUTTON_1_GPIO_Port GPIOB
#define BUTTON_2_Pin GPIO_PIN_9
#define BUTTON_2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
