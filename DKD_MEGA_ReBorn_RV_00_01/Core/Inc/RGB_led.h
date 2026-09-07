/*
 * RGB_led.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Yash_Choudhary
 */

#ifndef INC_RGB_LED_H_
#define INC_RGB_LED_H_

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
#define RED_CH_PB0 TIM_CHANNEL_3
#define GREEN_CH_PA7 TIM_CHANNEL_2
#define BLUE_CH_PA6 TIM_CHANNEL_1
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */
void RGB_Led_init(TIM_HandleTypeDef* tim_handler);
void set_Chnl_PWM(uint8_t red, uint8_t gree, uint8_t blu );
/* USER CODE END EFP */


#endif /* INC_RGB_LED_H_ */
