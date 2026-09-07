/*
 * GUI_Comm.h
 *
 *  Created on: Oct 09, 2026
 *      Author: Yash.Choudhary
 */

#ifndef INC_GUI_COMM_H_
#define INC_GUI_COMM_H_

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
#define TMT HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define TMT_MODE 1
#define RCV HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define RCV_MODE 0

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */

void GUI_Comm_init(UART_HandleTypeDef* uart_handler);
void GUI_timer_1msec(void);
void GUI_Comm_handler(void);

/* USER CODE END EFP */


#endif /* INC_GUI_COMM_H_ */
