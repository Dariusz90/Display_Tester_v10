/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"
#include "stm32f0xx_ll_crs.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Tester.h"
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
#define DO_DISP_a_Pin LL_GPIO_PIN_0
#define DO_DISP_a_GPIO_Port GPIOA
#define DO_DISP_b_Pin LL_GPIO_PIN_1
#define DO_DISP_b_GPIO_Port GPIOA
#define DO_DISP_c_Pin LL_GPIO_PIN_2
#define DO_DISP_c_GPIO_Port GPIOA
#define DO_DISP_d_Pin LL_GPIO_PIN_3
#define DO_DISP_d_GPIO_Port GPIOA
#define DO_DISP_e_Pin LL_GPIO_PIN_4
#define DO_DISP_e_GPIO_Port GPIOA
#define DO_DISP_f_Pin LL_GPIO_PIN_5
#define DO_DISP_f_GPIO_Port GPIOA
#define DO_DISP_g_Pin LL_GPIO_PIN_6
#define DO_DISP_g_GPIO_Port GPIOA
#define DO_DISP_dp_Pin LL_GPIO_PIN_7
#define DO_DISP_dp_GPIO_Port GPIOA
#define ADC_IN8_Current2_Pin LL_GPIO_PIN_0
#define ADC_IN8_Current2_GPIO_Port GPIOB
#define ADC_IN9_Current1_Pin LL_GPIO_PIN_1
#define ADC_IN9_Current1_GPIO_Port GPIOB
#define DO_Ext_LED_G_Pin LL_GPIO_PIN_9
#define DO_Ext_LED_G_GPIO_Port GPIOA
#define DO_Ext_LED_R_Pin LL_GPIO_PIN_10
#define DO_Ext_LED_R_GPIO_Port GPIOA
#define DO_LED1_Pin LL_GPIO_PIN_11
#define DO_LED1_GPIO_Port GPIOA
#define DO_LED2_Pin LL_GPIO_PIN_12
#define DO_LED2_GPIO_Port GPIOA
#define DI_SW1_Pin LL_GPIO_PIN_4
#define DI_SW1_GPIO_Port GPIOB
#define DI_SW2_Pin LL_GPIO_PIN_5
#define DI_SW2_GPIO_Port GPIOB
#define DI_SW3_Pin LL_GPIO_PIN_6
#define DI_SW3_GPIO_Port GPIOB
#define DI_SW4_Pin LL_GPIO_PIN_7
#define DI_SW4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
