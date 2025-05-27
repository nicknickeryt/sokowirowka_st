/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define BUTTON_ENCODER_Pin GPIO_PIN_1
#define BUTTON_ENCODER_GPIO_Port GPIOA
#define PUMP1_Pin GPIO_PIN_2
#define PUMP1_GPIO_Port GPIOA
#define PUMP2_Pin GPIO_PIN_3
#define PUMP2_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOA
#define ECHO2_TIM_Pin GPIO_PIN_6
#define ECHO2_TIM_GPIO_Port GPIOA
#define XSHUT1_Pin GPIO_PIN_1
#define XSHUT1_GPIO_Port GPIOB
#define XSHUT2_Pin GPIO_PIN_2
#define XSHUT2_GPIO_Port GPIOB
#define TRIG2_Pin GPIO_PIN_13
#define TRIG2_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_14
#define BUTTON_GPIO_Port GPIOB
#define BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define LCD_SCL_Pin GPIO_PIN_8
#define LCD_SCL_GPIO_Port GPIOA
#define ENCB_Pin GPIO_PIN_15
#define ENCB_GPIO_Port GPIOA
#define ENCA_Pin GPIO_PIN_3
#define ENCA_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_4
#define LCD_SDA_GPIO_Port GPIOB
#define ECHO1_TIM_Pin GPIO_PIN_5
#define ECHO1_TIM_GPIO_Port GPIOB
#define TRIG1_Pin GPIO_PIN_6
#define TRIG1_GPIO_Port GPIOB
#define SERVO_PWM_Pin GPIO_PIN_8
#define SERVO_PWM_GPIO_Port GPIOB
#define MUG_DET_Pin GPIO_PIN_9
#define MUG_DET_GPIO_Port GPIOB
#define MUG_DET_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
