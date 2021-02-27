/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOD
#define LED_ORANGE_Pin GPIO_PIN_13
#define LED_ORANGE_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_15
#define LED_BLUE_GPIO_Port GPIOD
#define GLCD_DB0_Pin GPIO_PIN_0
#define GLCD_DB0_GPIO_Port GPIOD
#define GLCD_DB1_Pin GPIO_PIN_1
#define GLCD_DB1_GPIO_Port GPIOD
#define GLCD_DB2_Pin GPIO_PIN_2
#define GLCD_DB2_GPIO_Port GPIOD
#define GLCD_DB3_Pin GPIO_PIN_3
#define GLCD_DB3_GPIO_Port GPIOD
#define GLCD_DB4_Pin GPIO_PIN_4
#define GLCD_DB4_GPIO_Port GPIOD
#define GLCD_DB5_Pin GPIO_PIN_5
#define GLCD_DB5_GPIO_Port GPIOD
#define GLCD_DB6_Pin GPIO_PIN_6
#define GLCD_DB6_GPIO_Port GPIOD
#define GLCD_DB7_Pin GPIO_PIN_7
#define GLCD_DB7_GPIO_Port GPIOD
#define GLCD_RST_Pin GPIO_PIN_4
#define GLCD_RST_GPIO_Port GPIOB
#define GLCD_CS2_Pin GPIO_PIN_5
#define GLCD_CS2_GPIO_Port GPIOB
#define GLCD_CS1_Pin GPIO_PIN_6
#define GLCD_CS1_GPIO_Port GPIOB
#define GLCD_EN_Pin GPIO_PIN_7
#define GLCD_EN_GPIO_Port GPIOB
#define GLCD_RW_Pin GPIO_PIN_8
#define GLCD_RW_GPIO_Port GPIOB
#define GLCD_RS_Pin GPIO_PIN_9
#define GLCD_RS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
