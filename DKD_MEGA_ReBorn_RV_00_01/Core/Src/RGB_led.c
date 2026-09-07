/*
 * RGB_led.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Yash_Choudhary
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "RGB_led.h"
//#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
typedef struct RGB_Led_variables
{
	TIM_HandleTypeDef* tim_handler;
}rgb_var_typdef;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */
rgb_var_typdef rgb_var;
/* USER CODE END EV */

/* *****************FUNCTIONS******************/
/* USER CODE BEGIN 1 */

void RGB_Led_init(TIM_HandleTypeDef* tim_handler)
{
	rgb_var.tim_handler = tim_handler;
	//__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, RED_CH_PB0, 0);
	//__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, GREEN_CH_PA7, 0);
	//__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, BLUE_CH_PA6, 0);
	HAL_TIM_PWM_Start_IT(rgb_var.tim_handler, RED_CH_PB0);
	HAL_TIM_PWM_Start_IT(rgb_var.tim_handler, GREEN_CH_PA7);
	HAL_TIM_PWM_Start_IT(rgb_var.tim_handler, BLUE_CH_PA6);
}

void set_Chnl_PWM(uint8_t red, uint8_t gree, uint8_t blu )
{
	if(red == 0){__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, RED_CH_PB0, 0);}
	if(gree == 0){__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, GREEN_CH_PA7, 0);}
	if(blu == 0){__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, BLUE_CH_PA6, 0);}
	if(red != 0){__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, RED_CH_PB0, (red*4)-1);}
	if(gree != 0){__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, GREEN_CH_PA7, (gree*4)-1);}
	if(blu != 0){__HAL_TIM_SET_COMPARE(rgb_var.tim_handler, BLUE_CH_PA6, (blu*4)-1);}
}

/* USER CODE END 1 */

