/*
 * Sen_TCS34725.h
 *
 *  Created on: Oct 29, 2025
 *      Author: Yash.Choudhary
 */

#ifndef INC_SEN_TCS34725_H_
#define INC_SEN_TCS34725_H_

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
#define MAX_SEN_DATA 20
#define MAX_SEN_CNTRL_DATA 10
#define RD_DELAY_CNT 700
#define STOP (0x00) /**< Value for ENABLE resistor to stop RGBC operation **/
#define START (0x03) /**< Value for ENABLE resistor to start RGBC operation **/
#define TCS34725_COMMAND_BYTE (0x80) /**< Command bit **/
#define TCS34725_ADD (0x29)     /**< I2C address **/
#define TCS34725_ENABLE (0x00)      /**< Interrupt Enable register */
#define TCS34725_ATIME (0x01) /**< Integration time */
#define TCS34725_WTIME (0x03) /**< Wait time (if TCS34725_ENABLE_WEN is asserted) */
#define TCS34725_AILTL (0x04) /**< Clear channel lower interrupt threshold (lower byte) */
#define TCS34725_AILTH (0x05) /**< Clear channel lower interrupt threshold (higher byte) */
#define TCS34725_AIHTL (0x06) /**< Clear channel upper interrupt threshold (lower byte) */
#define TCS34725_AIHTH (0x07) /**< Clear channel upper interrupt threshold (higher byte) */
#define TCS34725_PERS  (0x0C) /**< Persistence register - basic SW filtering mechanism for interrupts */
#define TCS34725_CONFIG (0x0D) /**< Configuration **/
#define TCS34725_CONTROL (0x0F) /**< Set the gain level for the sensor */
#define TCS34725_ID  (0x12) /**< 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */
#define TCS34725_STATUS (0x13)      /**< Device status **/
#define TCS34725_CDATAL (0x14) /**< Clear channel data low byte */
#define TCS34725_CDATAH (0x15) /**< Clear channel data high byte */
#define TCS34725_RDATAL (0x16) /**< Red channel data low byte */
#define TCS34725_RDATAH (0x17) /**< Red channel data high byte */
#define TCS34725_GDATAL (0x18) /**< Green channel data low byte */
#define TCS34725_GDATAH (0x19) /**< Green channel data high byte */
#define TCS34725_BDATAL (0x1A) /**< Blue channel data low byte */
#define TCS34725_BDATAH (0x1B) /**< Blue channel data high byte */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */
void sensor_timer_1msec(void);
void TCS3472X_init(I2C_HandleTypeDef* i2cHandle);
void TCS3472X_handler(void);
int get_sen_stat(void);
void get_rgbc_val(uint16_t* Red_var, uint16_t* Green_var, uint16_t* Blue_var, uint16_t* Clear_var);
void get_mavg_rgbc_val(uint16_t* Red_var, uint16_t* Green_var, uint16_t* Blue_var, uint16_t* Clear_var);
/* USER CODE END EFP */


#endif /* INC_SEN_TCS34725_H_ */
