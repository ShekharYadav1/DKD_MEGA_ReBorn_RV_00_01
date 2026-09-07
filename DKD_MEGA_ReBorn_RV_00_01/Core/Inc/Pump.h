/*
 * Pump.h
 *
 *  Created on: Oct 09, 2026
 *      Author: Yash.Choudhary
 */

#ifndef INC_PUMP_H_
#define INC_PUMP_H_

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g0xx_hal.h"  // Adjust to your MCU series
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

//UART DEFINES//
#define PUMP1ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)//CHEMICAL 1 ON
#define PUMP1OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)//CHEMICAL 1 OFF

#define PUMP2ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)//CHEMICAL 2 ON
#define PUMP2OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)//CHEMICAL 2 OFF

#define PUMP3ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)//CHEMICAL 3 ON
#define PUMP3OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)//CHEMICAL 3 OFF

#define PUMP4ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)//CHEMICAL 4 ON
#define PUMP4OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)//CHEMICAL 4 OFF

#define PUMP5ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)//CHEMICAL 5 ON
#define PUMP5OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)//CHEMICAL 5 OFF

#define PUMP6ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)//WATER AS CHEMICAL ON
#define PUMP6OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)//WATER AS CHEMICAL OFF

#define PUMP7ON HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)//WATER FOR WASH INTERNAL ON
#define PUMP7OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)//WATER FOR WASH INTERNAL OFF

#define PUMP8ON HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET)//DRAIN ON
#define PUMP8OFF HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET)//DRAIN OFF

#define MOTOR1ON HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)//MIXER ON
#define MOTOR1OFF HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)//MIXER OFF

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */


void pump1( uint8_t flag);
void pump2( uint8_t flag);
void pump3( uint8_t flag);
void pump4( uint8_t flag);
void pump5( uint8_t flag);
void pump6( uint8_t flag);
void pump7( uint8_t flag);
void pump8( uint8_t flag);
void motor1( uint8_t flag);
void pump_timer_1msec(void);
void mot_tm_rst(void);
void pump_handler(void);
/* USER CODE END EFP */


#endif /* INC_PUMP_H_ */
