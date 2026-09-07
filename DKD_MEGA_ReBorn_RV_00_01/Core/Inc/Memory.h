/*
 * Pump.h
 *
 *  Created on: feb 11, 2026
 *      Author: Yash.Choudhary
 */

#ifndef INC_MEMORY_H_
#define INC_MEMORY_H_

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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */

void Flash_Read_Data (uint32_t StartPageAddress, uint64_t *RxBuf, uint16_t numberofwords);
uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint64_t *Data, uint16_t numberofwords);

/* USER CODE END EFP */


#endif /* INC_MEMORY_H_ */
