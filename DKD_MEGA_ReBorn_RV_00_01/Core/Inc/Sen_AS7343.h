/*
 * Sen_AS7343.h
 *
 *  Created on: Mar 23, 2026
 *      Author: Yash.Choudhary
 */

#ifndef INC_SEN_AS7343_H_
#define INC_SEN_AS7343_H_

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
//#define MAX_SEN_DATA 20
//#define MAX_SEN_CNTRL_DATA 10
//#define RD_DELAY_CNT 700
//#define STOP (0x00) /**< Value for ENABLE resistor to stop RGBC operation **/
//#define START (0x03) /**< Value for ENABLE resistor to start RGBC operation **/
//#define TCS34725_COMMAND_BYTE (0x80) /**< Command bit **/
#define AS7343_ADD (0x39)     /**< I2C address **/
#define AS7343_AUXID (0x58)     /**<  */
#define AS7343_REVID (0x59) 	/**<  */
#define AS7343_ID (0x5A) 		/**<  */
#define AS7343_CFG12 (0x066) 	/**<  */
#define AS7343_ENABLE (0x80) 	/**<  */
#define AS7343_ATIME (0x81) 	/**<  */
#define AS7343_WTIME (0x83) 	/**<  */
#define AS7343_SP_TH_L_LSB  (0x84) /**<  */
#define AS7343_SP_TH_L_MSB (0x85) /**<  **/
#define AS7343_SP_TH_H_LSB (0x86) /**<  */
#define AS7343_SP_TH_H_MSB  (0x87) /**<  */
#define AS7343_STATUS (0x93)      /**<  **/
#define AS7343_ASTATUS (0x94) /**<  */
#define AS7343_DATA_0_L (0x95) /**<  */
#define AS7343_DATA_0_H (0x96) /**<  */
#define AS7343_DATA_1_L (0x97) /**<  */
#define AS7343_DATA_1_H (0x98) /**<  */
#define AS7343_DATA_2_L (0x99) /**<  */
#define AS7343_DATA_2_H (0x9A) /**<  */
#define AS7343_DATA_3_L (0x9B) /**<  */
#define AS7343_DATA_3_H (0x9C) /**<  */
#define AS7343_DATA_4_L (0x9D) /**<  */
#define AS7343_DATA_4_H (0x9E) /**<  */
#define AS7343_DATA_5_L (0x9F) /**<  */
#define AS7343_DATA_5_H (0xA0) /**<  */
#define AS7343_DATA_6_L (0xA1) /**<  */
#define AS7343_DATA_6_H (0xA2) /**<  */
#define AS7343_DATA_7_L (0xA3) /**<  */
#define AS7343_DATA_7_H (0xA4) /**<  */
#define AS7343_DATA_8_L (0xA5) /**<  */
#define AS7343_DATA_8_H (0xA6) /**<  */
#define AS7343_DATA_9_L (0xA7) /**<  */
#define AS7343_DATA_9_H (0xA8) /**<  */
#define AS7343_DATA_10_L (0xA9) /**<  */
#define AS7343_DATA_10_H (0xAA) /**<  */
#define AS7343_DATA_11_L (0xAB) /**<  */
#define AS7343_DATA_11_H (0xAC) /**<  */
#define AS7343_DATA_12_L (0xAD) /**<  */
#define AS7343_DATA_12_H (0xAE) /**<  */
#define AS7343_DATA_13_L (0xAF) /**<  */
#define AS7343_DATA_13_H (0xB0) /**<  */
#define AS7343_DATA_14_L (0xB1) /**<  */
#define AS7343_DATA_14_H (0xB2) /**<  */
#define AS7343_DATA_15_L (0xB3) /**<  */
#define AS7343_DATA_15_H (0xB4) /**<  */
#define AS7343_DATA_16_L (0xB5) /**<  */
#define AS7343_DATA_16_H (0xB6) /**<  */
#define AS7343_DATA_17_L (0xB7) /**<  */
#define AS7343_DATA_17_H (0xB8) /**<  */
#define AS7343_STATUS_2 (0x90) /**<  */
#define AS7343_STATUS_3 (0x91) /**<  */
#define AS7343_STATUS_5 (0xBB) /**<  */
#define AS7343_STATUS_4 (0xBC) /**<  */
#define AS7343_CFG_0 (0xBF) /**<  */
#define AS7343_CFG_1 (0xC6) /**<  */
#define AS7343_CFG_3 (0xC7) /**<  */
#define AS7343_CFG_6 (0xF5) /**<  */
#define AS7343_CFG_8 (0xC9) /**<  */
#define AS7343_CFG_9 (0xCA) /**<  */
#define AS7343_CFG_10 (0x65) /**<  */
#define AS7343_PERS (0xCF) /**<  */
#define AS7343_GPIO (0x6B) /**<  */
#define AS7343_ASTEP_L (0xD4) /**<  */
#define AS7343_ASTEP_H (0xD5) /**<  */
#define AS7343_CFG20 (0xD6) /**<  */
#define AS7343_LED (0xCD) /**<  */
#define AS7343_AGC_GAIN_MAX (0xD7) /**<  */
#define AS7343_AZ_CONFIG (0xDE) /**<  */
#define AS7343_FD_TIME_1 (0xE0) /**<  */
#define AS7343_FD_TIME_2 (0xE2) /**<  */
#define AS7343_FD_CFG0 (0xDF) /**<  */
#define AS7343_FD_STATUS (0xE3) /**<  */
#define AS7343_INTENAB (0xF9) /**<  */
#define AS7343_CONTROL (0xFA) /**<  */
#define AS7343_FIFO_MAP (0xFC) /**<  */
#define AS7343_FIFO_LVL (0xFD) /**<  */
#define AS7343_FDATA_L (0xFE) /**<  */
#define AS7343_FDATA_H (0xFF) /**<  */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */
/*void sensor_timer_1msec(void);
void TCS3472X_init(I2C_HandleTypeDef* i2cHandle);
void TCS3472X_handler(void);
int get_sen_stat(void);
void get_rgbc_val(uint16_t* Red_var, uint16_t* Green_var, uint16_t* Blue_var, uint16_t* Clear_var);
void get_mavg_rgbc_val(uint16_t* Red_var, uint16_t* Green_var, uint16_t* Blue_var, uint16_t* Clear_var);
*/
/* USER CODE END EFP */

#endif /* INC_SEN_AS7343_H_ */
