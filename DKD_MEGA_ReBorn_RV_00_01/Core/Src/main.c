/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
// #include "cmsis_os2.h"
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
#include "my_struct.h"
// #include "Sen_AS7343.h"
#include "Sen_TCS34725.h"
#include "RGB_led.h"
#include "GUI_Comm.h"
#include "Pump.h"
#include "Memory.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ARC_CAL
// #define OLD_CAL

#define TM_MULTIPLIER 20

#define FLASH_OVER_WRITE
#define MEM_ID1 0xAA
#define MEM_ID2 0x55
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
sys_var_typdef sys_info;
union_bk_var_typdef save_sys_info;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void sys_mem_validate(void);
void auto_zero_adjust(unn_std_var_typdef *opt_std_vars);
// void value_calculation2(void);
// void value_calculation(void);
void cal_result(void);
void auto_set_adjust(void);
void PHOS_value_calculation2(unn_std_var_typdef *opt_std_vars);
void SUL_value_calculation1(unn_std_var_typdef *opt_std_vars);
void NIT_value_calculation1(unn_std_var_typdef *opt_std_vars);
void POT_value_calculation1(unn_std_var_typdef *opt_std_vars);
// void ZIN_value_calculation1(unn_std_var_typdef *opt_std_vars);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	//  osKernelInitialize();
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();		   // motors
	MX_I2C1_Init();		   // sensor
	MX_TIM3_Init();		   // led pwm
	MX_USART2_UART_Init(); // uart rs485
	/* USER CODE BEGIN 2 */
	// HAL_Delay(100);
	Flash_Read_Data(MEM_STRT_ADD, save_sys_info.bkdata, MEM_SIZE);
	sys_mem_validate();
	TCS3472X_init(&hi2c1);
	RGB_Led_init(&htim3);
	GUI_Comm_init(&huart2);
	pump1(0);
	pump2(0);
	pump3(0);
	pump4(0);
	pump5(0);
	pump6(0);
	pump7(0);
	pump8(0);
	motor1(0);
	// PUMP1OFF;PUMP2OFF;PUMP3OFF;PUMP4OFF;PUMP5OFF;PUMP6OFF;PUMP7OFF;
	// PUMP8OFF;MOTOR1OFF;//pump & motors
	set_Chnl_PWM(sys_info.led_pwm_red, sys_info.led_pwm_green, sys_info.led_pwm_blue);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		if ((sys_info.sys_f.mem_save == 1) && (sys_info.sys_f.comm_mode == RCV_MODE))
		{
			Flash_Write_Data(MEM_STRT_ADD, save_sys_info.bkdata, MEM_SIZE);
			sys_info.sys_f.mem_save = 0;
		}
		TCS3472X_handler();
		get_sen_stat() ? (sys_info.Stat_L.sen_stat |= 1) : (sys_info.Stat_L.sen_stat &= 0);
		if (sys_info.Stat_L.sen_stat)
		{
			get_mavg_rgbc_val(&sys_info.curr_rgbc_vars.curr_red_rcv, &sys_info.curr_rgbc_vars.curr_green_rcv, &sys_info.curr_rgbc_vars.curr_blue_rcv, &sys_info.curr_rgbc_vars.curr_clear_rcv);
			if (!sys_info.Stat_L.warm_up_stat)
			{
				if (sys_info.Stat_M.tak_data_stat)
				{
					if (sys_info.tak_data_skp_run_tm >= sys_info.tak_data_skp_tm)
					{
						if ((!sys_info.curr_ResVal) && (!sys_info.curr_Result_cat))
						{
							cal_result();
						}
						// sys_info.Stat_L.drain_wsh_stat = 1; // this is for auto drain after result
						sys_info.Stat_M.tak_data_stat = 0;
						sys_info.Stat_L.alt_func = 0;
						sys_info.tak_data_skp_run_tm = 0;
					}
				}
				if (sys_info.Stat_L.auto_zero)
				{
					if (sys_info.auto_zero_skp_run_tm >= sys_info.auto_zero_skp_tm)
					{
#ifdef ARC_CAL
						if (sys_info.Stat_L.alt_func == 0)
						{
							auto_zero_adjust(&sys_info.opt_std_vars);
							auto_zero_adjust(&sys_info.opt_std_vars2);
						}
						else if (sys_info.Stat_L.alt_func == 1)
						{
							auto_zero_adjust(&sys_info.opt_std_vars);
							auto_zero_adjust(&sys_info.opt_std_vars2);
							sys_info.Stat_L.alt_func = 0;
						}
						// drift% = ((NEW standard - OLD saved standard) / OLD saved standard) * 100
						// NEW = opt_std_vars (overwritten by auto_zero_adjust with live readings)
						// OLD = hrd_std_vars (hard-coded saved, unchanged during auto-zero)
						sys_info.rgbc_drift[0] = (int16_t)(((int32_t)sys_info.opt_std_vars.stan_0_red - (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_red) * 100 / (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_red);
						sys_info.rgbc_drift[1] = (int16_t)(((int32_t)sys_info.opt_std_vars.stan_0_green - (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_green) * 100 / (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_green);
						sys_info.rgbc_drift[2] = (int16_t)(((int32_t)sys_info.opt_std_vars.stan_0_blue - (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_blue) * 100 / (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_blue);
						sys_info.rgbc_drift[3] = (int16_t)(((int32_t)sys_info.opt_std_vars.stan_0_clear - (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_clear) * 100 / (int32_t)save_sys_info.bk_var.hrd_std_vars.stan_0_clear);
#endif
						sys_info.Stat_L.auto_zero_save = 1;
						sys_info.Stat_L.alt_func = 0;
						sys_info.Stat_L.auto_zero = 0;
						// sys_info.Stat_L.drain_wsh_stat = 1;
						sys_info.auto_zero_skp_run_tm = 0;
						sys_info.curr_ResVal = 0;
						sys_info.curr_Result_cat = 0;
					}
				}
				/*if(sys_info.Stat_M2.auto_set)
				{
					if(sys_info.auto_set_skp_run_tm >= sys_info.auto_set_skp_tm)
					{
						auto_set_adjust();
						//sys_info.Stat_L.drain_wsh_stat = 1;
						sys_info.Stat_M2.auto_set = 0;
						sys_info.auto_set_skp_run_tm = 0;
					}
				}*/
			}
			else
			{
				sys_info.Stat_M.tak_data_stat = 0;
				sys_info.Stat_L.auto_zero = 0;
			}
		}
		GUI_Comm_handler();
		pump_handler(); // command lestion from GUI_Comm_handler() and pump status update to sys_info.pump_stat

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void auto_set_adjust(void)
{
	float factor;
	factor = (float)sys_info.curr_rgbc_vars.curr_red_rcv / (float)sys_info.opt_std_vars.stan_4_red;
	sys_info.opt_std_vars.stan_4_red = sys_info.curr_rgbc_vars.curr_red_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		if (i == 4)
		{
			continue;
		}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][0] =
				(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][0]);
		}
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_green_rcv / (float)sys_info.opt_std_vars.stan_4_green;
	sys_info.opt_std_vars.stan_4_green = sys_info.curr_rgbc_vars.curr_green_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		if (i == 4)
		{
			continue;
		}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][1] =
				(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][1]);
		}
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_blue_rcv / (float)sys_info.opt_std_vars.stan_4_blue;
	sys_info.opt_std_vars.stan_4_blue = sys_info.curr_rgbc_vars.curr_blue_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		if (i == 4)
		{
			continue;
		}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][2] =
				(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][2]);
		}
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_clear_rcv / (float)sys_info.opt_std_vars.stan_4_clear;
	sys_info.opt_std_vars.stan_4_clear = sys_info.curr_rgbc_vars.curr_clear_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		if (i == 4)
		{
			continue;
		}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][3] =
				(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][3]);
		}
	}
}

void PHOS_value_calculation2(unn_std_var_typdef *opt_std_vars)
{
	// sys_info.curr_rgbc_vars.curr_red_rcv = 9901;
	// sys_info.curr_rgbc_vars.curr_green_rcv = 10660;
	// sys_info.curr_rgbc_vars.curr_blue_rcv = 13163;
	// sys_info.curr_rgbc_vars.curr_clear_rcv = 3172;

	sys_info.curr_absrb_val = log10(((double)opt_std_vars->strd_vars[0][sys_info.val_cal_y] * (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_x]) / ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_x] * (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_y]));
	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		sys_info.std_absrb_val[i] = log10(((double)opt_std_vars->strd_vars[0][sys_info.val_cal_y] * (double)opt_std_vars->strd_vars[i][sys_info.val_cal_x]) / ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_x] * (double)opt_std_vars->strd_vars[i][sys_info.val_cal_y]));
	}
	// log10( ($G$8 * J12)/($F$8 *K12))

	if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[0])
	{
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[1])
	{
		sys_info.curr_ResVal = ((sys_info.curr_absrb_val / sys_info.std_absrb_val[1]) * sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[2])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[1]) / (sys_info.std_absrb_val[2] - sys_info.std_absrb_val[1])) * (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[3])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[2]) / (sys_info.std_absrb_val[3] - sys_info.std_absrb_val[2])) * (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[4])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[3]) / (sys_info.std_absrb_val[4] - sys_info.std_absrb_val[3])) * (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[5])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[4]) / (sys_info.std_absrb_val[5] - sys_info.std_absrb_val[4])) * (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[6])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[5]) / (sys_info.std_absrb_val[6] - sys_info.std_absrb_val[5])) * (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[7])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[6]) / (sys_info.std_absrb_val[7] - sys_info.std_absrb_val[6])) * (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[8])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[7]) / (sys_info.std_absrb_val[8] - sys_info.std_absrb_val[7])) * (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[9])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[8]) / (sys_info.std_absrb_val[9] - sys_info.std_absrb_val[8])) * (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[10])
	{
		sys_info.curr_ResVal = (((sys_info.curr_absrb_val - sys_info.std_absrb_val[9]) / (sys_info.std_absrb_val[10] - sys_info.std_absrb_val[9])) * (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_absrb_val < sys_info.std_absrb_val[10])
	{
		sys_info.curr_ResVal = (sys_info.act_stan_vals[10] / sys_info.std_absrb_val[10]) * (sys_info.curr_absrb_val);
		sys_info.curr_Result_cat = 5; //////////////Verry High
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

void SUL_value_calculation1(unn_std_var_typdef *opt_std_vars)
{
	// uint32_t sum_x, sum_y, sum_xy, sum_xx;
	sys_info.sum_x = 0;
	sys_info.sum_y = 0;
	sys_info.sum_xy = 0;
	sys_info.sum_xx = 0;
	uint8_t i;
	for (i = 0; i < NOS_STD; i++)
	{
		sys_info.sum_x += opt_std_vars->strd_vars[i][sys_info.val_cal_x];
		sys_info.sum_y += opt_std_vars->strd_vars[i][sys_info.val_cal_y];
		sys_info.sum_xy += opt_std_vars->strd_vars[i][sys_info.val_cal_x] * opt_std_vars->strd_vars[i][sys_info.val_cal_y];
		sys_info.sum_xx += opt_std_vars->strd_vars[i][sys_info.val_cal_x] * opt_std_vars->strd_vars[i][sys_info.val_cal_x];
	}
	sys_info.mslp = (((double)NOS_STD * (double)sys_info.sum_xy) - ((double)sys_info.sum_x * (double)sys_info.sum_y)) / (((double)NOS_STD * (double)sys_info.sum_xx) - ((double)sys_info.sum_x * (double)sys_info.sum_x));

	sys_info.cshft = ((double)sys_info.sum_y - (sys_info.mslp * (double)sys_info.sum_x)) / NOS_STD;

	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		sys_info.ref_stan_x[i] = ((sys_info.mslp * ((double)opt_std_vars->strd_vars[i][sys_info.val_cal_y] - sys_info.cshft)) + (double)opt_std_vars->strd_vars[i][sys_info.val_cal_x]) / ((sys_info.mslp * sys_info.mslp) + 1);
		sys_info.ref_stan_y[i] = (sys_info.mslp * sys_info.ref_stan_x[i]) + sys_info.cshft;
		sys_info.ref_stan_dist[i] = sqrt(((sys_info.ref_stan_x[0] - sys_info.ref_stan_x[i]) * (sys_info.ref_stan_x[0] - sys_info.ref_stan_x[i])) + ((sys_info.ref_stan_y[0] - sys_info.ref_stan_y[i]) * (sys_info.ref_stan_y[0] - sys_info.ref_stan_y[i])));
	}

	// sys_info.curr_red_rcv = 16174;
	// sys_info.curr_green_rcv = 12952;
	// sys_info.curr_blue_rcv = 14442;
	// sys_info.curr_clear_rcv = 45558;
	sys_info.curr_x = ((sys_info.mslp * ((double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_y] - sys_info.cshft)) + (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_x]) / ((sys_info.mslp * sys_info.mslp) + 1);
	sys_info.curr_y = (sys_info.mslp * sys_info.curr_x) + sys_info.cshft;
	sys_info.curr_dist = sqrt(((sys_info.ref_stan_x[0] - sys_info.curr_x) * (sys_info.ref_stan_x[0] - sys_info.curr_x)) + ((sys_info.ref_stan_y[0] - sys_info.curr_y) * (sys_info.ref_stan_y[0] - sys_info.curr_y)));

	if (sys_info.curr_dist <= sys_info.ref_stan_dist[0])
	{
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[1])
	{
		sys_info.curr_ResVal = ((sys_info.curr_dist / sys_info.ref_stan_dist[1]) * sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[2])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[1]) / (sys_info.ref_stan_dist[2] - sys_info.ref_stan_dist[1])) * (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[3])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[2]) / (sys_info.ref_stan_dist[3] - sys_info.ref_stan_dist[2])) * (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[4])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[3]) / (sys_info.ref_stan_dist[4] - sys_info.ref_stan_dist[3])) * (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[5])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[4]) / (sys_info.ref_stan_dist[5] - sys_info.ref_stan_dist[4])) * (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[6])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[5]) / (sys_info.ref_stan_dist[6] - sys_info.ref_stan_dist[5])) * (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[7])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[6]) / (sys_info.ref_stan_dist[7] - sys_info.ref_stan_dist[6])) * (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[8])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[7]) / (sys_info.ref_stan_dist[8] - sys_info.ref_stan_dist[7])) * (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[9])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[8]) / (sys_info.ref_stan_dist[9] - sys_info.ref_stan_dist[8])) * (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[10])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[9]) / (sys_info.ref_stan_dist[10] - sys_info.ref_stan_dist[9])) * (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_dist > sys_info.ref_stan_dist[10])
	{
		sys_info.curr_ResVal = ((sys_info.act_stan_vals[10] / sys_info.ref_stan_dist[10]) * (sys_info.curr_dist - sys_info.ref_stan_dist[10])) + sys_info.act_stan_vals[10];
		sys_info.curr_Result_cat = 5; //////////////Verry High
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

void NIT_value_calculation3(unn_std_var_typdef *opt_std_vars)
{
	
	double absrb0, absrb1, absrb2, absrb3, avg_absrb;
	double std_absrb_val_mul_sum, std_absrb_sqr_sum;
	std_absrb_val_mul_sum = 0;
	std_absrb_sqr_sum = 0;
	sys_info.std_multplr = 0;
	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		absrb0 = log10((double)opt_std_vars->stan_0_red / (double)opt_std_vars->strd_vars[i][0]);
		absrb1 = log10((double)opt_std_vars->stan_0_green / (double)opt_std_vars->strd_vars[i][1]);
		absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)opt_std_vars->strd_vars[i][2]);
		absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)opt_std_vars->strd_vars[i][3]);
		avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4.00;
		// sys_info.std_absrb_val_mul[i] = (avg_absrb * (double)sys_info.act_stan_vals[i]);
		// sys_info.std_absrb_sqr[i] = (avg_absrb * avg_absrb);
		std_absrb_val_mul_sum = std_absrb_val_mul_sum + (avg_absrb * (double)sys_info.act_stan_vals[i]);
		std_absrb_sqr_sum = std_absrb_sqr_sum + (avg_absrb * avg_absrb);
	}
	sys_info.std_multplr = std_absrb_val_mul_sum / std_absrb_sqr_sum; // constant factor

	absrb0 = log10((double)opt_std_vars->stan_0_red / (double)sys_info.curr_rgbc_vars.curr_red_rcv);
	absrb1 = log10((double)opt_std_vars->stan_0_green / (double)sys_info.curr_rgbc_vars.curr_green_rcv);
	absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)sys_info.curr_rgbc_vars.curr_blue_rcv);
	absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)sys_info.curr_rgbc_vars.curr_clear_rcv);
	avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4;
	sys_info.curr_ResVal = avg_absrb * sys_info.std_multplr;

	if (sys_info.curr_ResVal <= sys_info.act_stan_vals[0])
	{
		sys_info.curr_ResVal = sys_info.act_stan_vals[0];
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[1])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[2])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[3])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[4])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[5])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[6])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[7])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[8])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[9])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

/* noipa prevents GCC identical-code-folding from aliasing POT_value_calculation5 to NIT_value_calculation3 (identical algorithm duplicates) */
void __attribute__((noipa)) POT_value_calculation5(unn_std_var_typdef *opt_std_vars)
{
	double absrb0, absrb1, absrb2, absrb3, avg_absrb;                           /* absorbance for each RGBC channel and their average */
	double std_absrb_val_mul_sum, std_absrb_sqr_sum;                            /* regression accumulators: sum of (absorbance*actual) and sum of (absorbance^2) */
	std_absrb_val_mul_sum = 0;                                                   /* initialize regression numerator accumulator to zero */
	std_absrb_sqr_sum = 0;                                                       /* initialize regression denominator accumulator to zero */
	sys_info.std_multplr = 0;                                                     /* initialize multiplier (slope) to zero before calculation */

	/* --- STEP 1 & 2: Loop through all 11 calibration standards to compute regression slope --- */
	for (uint8_t i = 0; i < NOS_STD; i++)                                       /* iterate over all 11 standards (index 0 to 10) */
	{
		absrb0 = log10((double)opt_std_vars->stan_0_red / (double)opt_std_vars->strd_vars[i][0]);    /* absorbance of red channel:   log10(std0_red / stdi_red) */
		absrb1 = log10((double)opt_std_vars->stan_0_green / (double)opt_std_vars->strd_vars[i][1]);  /* absorbance of green channel: log10(std0_green / stdi_green) */
		absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)opt_std_vars->strd_vars[i][2]);   /* absorbance of blue channel:  log10(std0_blue / stdi_blue) */
		absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)opt_std_vars->strd_vars[i][3]);  /* absorbance of clear channel: log10(std0_clear / stdi_clear) */
		avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4.00;    // STANDARDS ka absorbance              /* average absorbance across all 4 RGBC channels */
		// sys_info.std_absrb_val_mul[i] = (avg_absrb * (double)sys_info.act_stan_vals[i]);
		// sys_info.std_absrb_sqr[i] = (avg_absrb * avg_absrb);
		std_absrb_val_mul_sum = std_absrb_val_mul_sum + (avg_absrb * (double)sys_info.act_stan_vals[i]); /* accumulate: absorbance * actual potassium value (numerator) */
		std_absrb_sqr_sum = std_absrb_sqr_sum + (avg_absrb * avg_absrb);                            /* accumulate: absorbance squared (denominator) */
	}
	sys_info.std_multplr = std_absrb_val_mul_sum / std_absrb_sqr_sum;           /* compute regression slope (multiplier) = sum(x*y) / sum(x^2) */


	absrb0 = log10((double)opt_std_vars->stan_0_red / (double)sys_info.curr_rgbc_vars.curr_red_rcv);   //current reading from sensor /* unknown sample red absorbance:   log10(std0_red / unknown_red) */
	absrb1 = log10((double)opt_std_vars->stan_0_green / (double)sys_info.curr_rgbc_vars.curr_green_rcv);/* unknown sample green absorbance: log10(std0_green / unknown_green) */
	absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)sys_info.curr_rgbc_vars.curr_blue_rcv);  /* unknown sample blue absorbance:  log10(std0_blue / unknown_blue) */
	absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)sys_info.curr_rgbc_vars.curr_clear_rcv);/* unknown sample clear absorbance: log10(std0_clear / unknown_clear) *///ye sensor se abhi-abhi liya gaya live/current sample reading hai (jo standards nahi hai, balki wo sample hai jiska aap actually concentration jaanna chahte ho).
	                                                            
	avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4;                         // absorbance of current sample  ( STANDARDS ka absorbance )                 /* average absorbance across all 4 RGBC channels for unknown sample */
	sys_info.curr_ResVal = avg_absrb * sys_info.std_multplr;                    /* potassium concentration = average_absorbance * regression_slope */

	/* --- STEP 4: Categorize result into 5 levels based on threshold breakpoints --- */
	/* Thresholds: act_stan_vals[0]=0, [1]=2, [2]=4, [3]=6, [4]=6, [5]=8, [6]=8, [7]=10, [8]=10, [9]=20, [10]=20 */
	if (sys_info.curr_ResVal <= sys_info.act_stan_vals[0])                      /* if result <= 0 ppm (below lowest standard) */
	{
		sys_info.curr_ResVal = sys_info.act_stan_vals[0];                       /* clamp result to minimum (0 ppm) */
		sys_info.curr_Result_cat = 1;                                           /* category 1 = Very Low */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[1])                 /* if result <= 2 ppm */
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW                   /* category 1 = Very Low */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[2])                 /* if result <= 4 ppm */
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW                   /* category 1 = Very Low */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[3])                 /* if result <= 6 ppm */
	{
		sys_info.curr_Result_cat = 2; //////////////LOW                         /* category 2 = Low */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[4])                 /* if result <= 6 ppm (duplicate threshold for Low) */
	{
		sys_info.curr_Result_cat = 2; //////////////LOW                         /* category 2 = Low */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[5])                 /* if result <= 8 ppm */
	{
		sys_info.curr_Result_cat = 3; //////////////Medium                      /* category 3 = Medium */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[6])                 /* if result <= 8 ppm (duplicate threshold for Medium) */
	{
		sys_info.curr_Result_cat = 3; //////////////Medium                      /* category 3 = Medium */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[7])                 /* if result <= 10 ppm */
	{
		sys_info.curr_Result_cat = 4; //////////////High                        /* category 4 = High */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[8])                 /* if result <= 10 ppm (duplicate threshold for High) */
	{
		sys_info.curr_Result_cat = 4; //////////////High                        /* category 4 = High */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[9])                 /* if result <= 20 ppm */
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High                  /* category 5 = Very High */
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[10])                /* if result <= 20 ppm (duplicate threshold for Very High) */
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High                  /* category 5 = Very High */
	}
	else if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])                 /* if result > 20 ppm (above highest standard) */
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High                  /* category 5 = Very High (cap at highest category) */
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}


void MAG_value_calculation4(unn_std_var_typdef *opt_std_vars)
{
	sys_info.curr_ResVal = (((double)opt_std_vars->stan_0_red / (double)sys_info.curr_rgbc_vars.curr_red_rcv) * sys_info.act_stan_vals[0]);
	if (sys_info.curr_ResVal <= sys_info.act_stan_vals[0])
	{
		sys_info.curr_ResVal = sys_info.act_stan_vals[0];
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[1])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[2])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[3])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[4])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[5])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[6])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[7])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[8])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[9])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

void BOR_value_calculation6(unn_std_var_typdef *opt_std_vars)
{
	sys_info.curr_ResVal = (((double)opt_std_vars->stan_0_green / (double)sys_info.curr_rgbc_vars.curr_green_rcv) * sys_info.act_stan_vals[0]);
	if (sys_info.curr_ResVal <= sys_info.act_stan_vals[0])
	{
		sys_info.curr_ResVal = sys_info.act_stan_vals[0];
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[1])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[2])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[3])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[4])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[5])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[6])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[7])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[8])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[9])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	     // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

void cal_result(void)
{

#ifdef ARC_CAL

	if (sys_info.Stat_L.alt_func == 0)
	{
		switch (sys_info.set_sys_add)
		{
		case SULPHUR:
			SUL_value_calculation1(&sys_info.opt_std_vars);
			break; // calibration done
		case PHOSPHORUS:
			PHOS_value_calculation2(&sys_info.opt_std_vars);
			break; // calibration done
		case NITROGEN:
			 NIT_value_calculation3(&sys_info.opt_std_vars);
			break; // calibration done
		case MAGNESIUM:
			MAG_value_calculation4(&sys_info.opt_std_vars);
			break; // pending calibration
		case POTASSIUM:
			POT_value_calculation5(&sys_info.opt_std_vars);   /* calculate potassium concentration using primary calibration standards */
			break; // testing calibration
		case BORON:
		    BOR_value_calculation6(&sys_info.opt_std_vars);
		    break;    // pending calibration
		default:
			// value_calculation1(&sys_info.opt_std_vars);
			// value_calculation(&sys_info.opt_std_vars);
			break;
		}
	}
	else if (sys_info.Stat_L.alt_func == 1)
	{
		switch (sys_info.set_sys_add)
		{
		case SULPHUR:
			SUL_value_calculation1(&sys_info.opt_std_vars2);
			break; // calibration done
		case PHOSPHORUS:
			PHOS_value_calculation2(&sys_info.opt_std_vars2);
			break; // calibration done
		case NITROGEN:
			NIT_value_calculation3(&sys_info.opt_std_vars2);
			break; // calibration done
		case MAGNESIUM:
			MAG_value_calculation4(&sys_info.opt_std_vars2);
			break; // pending calibration
		case POTASSIUM:
			/* POTASSIUM alternate path: alt_func==1, uses alternate (acidic) calibration data set (opt_std_vars2) */
			POT_value_calculation5(&sys_info.opt_std_vars2);  /* calculate potassium concentration using alternate calibration standards */
			break; // testing calibration
		case BORON:
		    BOR_value_calculation6(&sys_info.opt_std_vars2);
		     break;    // pending calibration
		default:
	///		 value_calculation1(&sys_info.opt_std_vars2);
			// value_calculation(&sys_info.opt_std_vars2);
			break;
		}
		sys_info.Stat_L.alt_func = 0;
	}
#endif
}

void auto_zero_adjust(unn_std_var_typdef *opt_std_vars)
{
	float factor;
	factor = (float)sys_info.curr_rgbc_vars.curr_red_rcv / (float)opt_std_vars->stan_0_red;
	opt_std_vars->stan_0_red = sys_info.curr_rgbc_vars.curr_red_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][0] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][0]);
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_green_rcv / (float)opt_std_vars->stan_0_green;
	opt_std_vars->stan_0_green = sys_info.curr_rgbc_vars.curr_green_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][1] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][1]);
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_blue_rcv / (float)opt_std_vars->stan_0_blue;
	opt_std_vars->stan_0_blue = sys_info.curr_rgbc_vars.curr_blue_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][2] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][2]);
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_clear_rcv / (float)opt_std_vars->stan_0_clear;
	opt_std_vars->stan_0_clear = sys_info.curr_rgbc_vars.curr_clear_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][3] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][3]);
	}
}

int mem_fresh_check(void)
{
	if ((save_sys_info.bk_data[0] == MEM_ID1) && (save_sys_info.bk_data[0] == MEM_ID2))
	{
		return 0;
	}
	    return 1;
}

int sw_update(void)
{
	if ((save_sys_info.bk_var.maj_sw_rv_no != MAJ_SW_RV_NO) || (save_sys_info.bk_var.mid_sw_rv_no != MID_SW_RV_NO) || (save_sys_info.bk_var.min_sw_rv_no != MIN_SW_RV_NO))
	{
		return 1;
	}
	    return 0;
}

int validate_data(void)
{
	// sys_address check
	if (save_sys_info.bk_var.curr_sys_add != sys_info.set_sys_add)
	{
		return 1;
	}
	return 0;
}

#ifdef ARC_CAL
void init_hrd_strd(void)
{
switch(save_sys_info.bk_var.curr_sys_add)
	{
	case MAGNESIUM:
		//alt func = 0;  												//alt func = 1;
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case IRON:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case COPPER:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case ZINC:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case BORON:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case SULPHUR://15-05-2026
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18994;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18994;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 19019;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 19019;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 21154;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 21154;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 53306;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 53306;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 18702;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 18702;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 18626;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 18626;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 20685;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 20685;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 52283;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 52283;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 18702;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 18702;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 18626;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 18626;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 20685;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 20685;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 52283;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 52283;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 16999;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 16999;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 16769;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 16769;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 18524;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 18524;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 47202;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 47202;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 15441;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 15441;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 15215;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 15215;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 16753;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 16753;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 42886;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 42886;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 13027;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 13027;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 12591;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 12591;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 13724;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 13724;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 35669;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 35669;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 11156;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 11156;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 10652;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 10652;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 11524;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 11524;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 30284;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 30284;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 9716;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 9716;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 9119;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 9119;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 9833;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 9833;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 26104;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 26104;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 9716;        save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 9716;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 9119;      save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 9119;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 9833;       save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 9833;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 26104;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 26104;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 8092;        save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 8092;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 7519;      save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 7519;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8029;       save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8029;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 21533;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 21533;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 8092;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 8092;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 7519;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 7519;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 8029;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 8029;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 21533;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 21533;
		break;
	case POTASSIUM:  //standard on date
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 19509;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;     /* stan_0: 0 ppm (blank) - Red channel: primary=19509, alternate=18270 */
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 19674;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;   /* stan_0: 0 ppm (blank) - Green channel: primary=19674, alternate=14522 */
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 21081;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;    /* stan_0: 0 ppm (blank) - Blue channel: primary=21081, alternate=16275 */
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 55231;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;   /* stan_0: 0 ppm (blank) - Clear channel: primary=55231, alternate=51293 */

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 18489;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;     /* stan_1: 2 ppm - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 18664;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;   /* stan_1: 2 ppm - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 19997;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;    /* stan_1: 2 ppm - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 52411;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;   /* stan_1: 2 ppm - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14879;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;     /* stan_2: 4 ppm - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 15148;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;   /* stan_2: 4 ppm - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 16205;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;    /* stan_2: 4 ppm - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 42431;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;   /* stan_2: 4 ppm - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 12434;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;     /* stan_3: 6 ppm - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12713;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;   /* stan_3: 6 ppm - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13558;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;    /* stan_3: 6 ppm - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 35561;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;   /* stan_3: 6 ppm - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 12434;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;     /* stan_4: 6 ppm (duplicate) - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 12713;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;   /* stan_4: 6 ppm (duplicate) - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 13558;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;    /* stan_4: 6 ppm (duplicate) - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 35561;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;   /* stan_4: 6 ppm (duplicate) - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10470;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;     /* stan_5: 8 ppm - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 10735;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;   /* stan_5: 8 ppm - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11418;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;    /* stan_5: 8 ppm - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 30004;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;   /* stan_5: 8 ppm - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 10470;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;     /* stan_6: 8 ppm (duplicate) - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 10735;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;   /* stan_6: 8 ppm (duplicate) - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 11418;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;    /* stan_6: 8 ppm (duplicate) - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 30004;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;   /* stan_6: 8 ppm (duplicate) - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 9211;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;     /* stan_7: 10 ppm - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 9457;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;   /* stan_7: 10 ppm - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10033;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;    /* stan_7: 10 ppm - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 26414;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;   /* stan_7: 10 ppm - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 9211;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;     /* stan_8: 10 ppm (duplicate) - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 9457;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;   /* stan_8: 10 ppm (duplicate) - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 10033;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;   /* stan_8: 10 ppm (duplicate) - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 26414;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;   /* stan_8: 10 ppm (duplicate) - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5346;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;     /* stan_9: 20 ppm - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 5534;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;   /* stan_9: 20 ppm - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 5824;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;    /* stan_9: 20 ppm - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 15398;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;   /* stan_9: 20 ppm - Clear channel */

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 5346;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;    /* stan_10: 20 ppm (duplicate) - Red channel */
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5534;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;  /* stan_10: 20 ppm (duplicate) - Green channel */
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 5824;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;   /* stan_10: 20 ppm (duplicate) - Blue channel */
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 15398;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;  /* stan_10: 20 ppm (duplicate) - Clear channel */
		break;
	case PHOSPHORUS://29-04-2026 alkline							//19-05-2026 acidic
        save_sys_info.bk_var.hrd_std_vars.stan_0_red   = 16604;  save_sys_info.bk_var.hrd_std_vars2.stan_0_red   = 16713;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 15462;  save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 15570;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue  = 18440;  save_sys_info.bk_var.hrd_std_vars2.stan_0_blue  = 18772;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 47486;  save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 47914;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red   = 14995;  save_sys_info.bk_var.hrd_std_vars2.stan_1_red   = 15262;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 14440;  save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 14729;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue  = 17352;  save_sys_info.bk_var.hrd_std_vars2.stan_1_blue  = 17901;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 43996;  save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 44955;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red   = 13285;  save_sys_info.bk_var.hrd_std_vars2.stan_2_red   = 13632;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 13287;  save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 13667;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue  = 16187;  save_sys_info.bk_var.hrd_std_vars2.stan_2_blue  = 16770;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 40162;  save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41371;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red   = 10378;  save_sys_info.bk_var.hrd_std_vars2.stan_3_red   = 10755;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 11221;  save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 11699;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue  = 13916;  save_sys_info.bk_var.hrd_std_vars2.stan_3_blue  = 14661;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 33405;  save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 34847;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red   = 10378;  save_sys_info.bk_var.hrd_std_vars2.stan_4_red   = 10755;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 11221;  save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 11699;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue  = 13916;  save_sys_info.bk_var.hrd_std_vars2.stan_4_blue  = 14661;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33405;  save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 34847;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red   = 8011;   save_sys_info.bk_var.hrd_std_vars2.stan_5_red   = 8207;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9411;   save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9766;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue  = 11962;  save_sys_info.bk_var.hrd_std_vars2.stan_5_blue  = 12556;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 27648;  save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 28672;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red   = 8011;   save_sys_info.bk_var.hrd_std_vars2.stan_6_red   = 8207;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 9411;   save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 9766;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue  = 11962;  save_sys_info.bk_var.hrd_std_vars2.stan_6_blue  = 12556;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27648;  save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 28672;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red   = 6181;   save_sys_info.bk_var.hrd_std_vars2.stan_7_red   = 6370;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 7914;   save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8293;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue  = 10319;  save_sys_info.bk_var.hrd_std_vars2.stan_7_blue  = 10927;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 22971;  save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 24034;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red   = 6181;   save_sys_info.bk_var.hrd_std_vars2.stan_8_red   = 6370;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 7914;   save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 8293;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue  = 10319;  save_sys_info.bk_var.hrd_std_vars2.stan_8_blue  = 10927;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22971;  save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 24034;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red   = 4516;   save_sys_info.bk_var.hrd_std_vars2.stan_9_red   = 4979;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6480;   save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 7100;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue  = 8728;   save_sys_info.bk_var.hrd_std_vars2.stan_9_blue  = 9585;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 18561;  save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 20341;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red   = 4516;  save_sys_info.bk_var.hrd_std_vars2.stan_10_red   = 4979;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 6480;  save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 7100;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue  = 8728;  save_sys_info.bk_var.hrd_std_vars2.stan_10_blue  = 9585;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18561; save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 20341;
		break;
	case NITROGEN:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 17073;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 17073;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 16682;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 16682;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 17398;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 17398;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 48515;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 48515;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16314;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16314;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13538;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13538;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 12862;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 12862;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 40536;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 40536;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 16314;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 16314;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 13538;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 13538;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 12862;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 12862;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 40536;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 40536;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 16101;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 16101;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 11230;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 11230;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 9825;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 9825;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 35260;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 35260;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 16101;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 16101;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 11230;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 11230;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 9825;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 9825;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 35260;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 35260;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 15347;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 15347;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9500;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9500;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 7923;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 7923;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 31112;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 31112;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 15347;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 15347;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 9500;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 9500;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 7923;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 7923;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 31112;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 31112;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 14752;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 14752;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8199;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8199;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 6557;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 6557;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 28013;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 28013;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 14752;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 14752;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 8199;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 8199;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 6557;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 6557;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 28013;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 28013;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 13030;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 13030;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6087;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6087;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 4636;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 4636;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22535;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22535;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 13030;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 13030;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 6087;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 6087;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 4636;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 4636;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 22535;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 22535;
		break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	}
}
#endif

void wrt_dflt(void)
{
	save_sys_info.bk_var.start_add1 = MEM_ID1;
	save_sys_info.bk_var.start_add2 = MEM_ID2;
	save_sys_info.bk_var.maj_sw_rv_no = MAJ_SW_RV_NO;
	save_sys_info.bk_var.mid_sw_rv_no = MID_SW_RV_NO;
	save_sys_info.bk_var.min_sw_rv_no = MIN_SW_RV_NO;
	save_sys_info.bk_var.curr_sys_add = sys_info.set_sys_add;
#ifdef ARC_CAL
	init_hrd_strd();
#endif

	save_sys_info.bk_var.end_add1 = 0x55;
	save_sys_info.bk_var.end_add2 = 0xAA;
}

#ifdef ARC_CAL
void sys_var_init(void)
{
	// system chemical quantity info
	switch (save_sys_info.bk_var.curr_sys_add)
	{
	case MAGNESIUM:
		sys_info.chem_1a_qty = 2000; // 20.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;		 // for water quantity in grams
		sys_info.led_pwm_red = 0xff; // pwm value
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.val_cal_y = 2; // blue//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
		break;
	case IRON:
		sys_info.chem_1a_qty = 200; /// 2.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 200;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
		break;
	case COPPER:
		sys_info.chem_1a_qty = 300;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 150;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.10;
		sys_info.act_stan_vals[2] = 0.10;
		sys_info.act_stan_vals[3] = 0.20;
		sys_info.act_stan_vals[4] = 0.20;
		sys_info.act_stan_vals[5] = 0.40;
		sys_info.act_stan_vals[6] = 0.40;
		sys_info.act_stan_vals[7] = 1.00;
		sys_info.act_stan_vals[8] = 1.00;
		sys_info.act_stan_vals[9] = 5.00;
		sys_info.act_stan_vals[10] = 10.00;
		break;
	case ZINC:
		sys_info.chem_1a_qty = 2000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 300;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
		break;
	case BORON:
		sys_info.chem_1a_qty = 2000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 1;
		sys_info.act_stan_vals[2] = 1;
		sys_info.act_stan_vals[3] = 2;
		sys_info.act_stan_vals[4] = 2;
		sys_info.act_stan_vals[5] = 4;
		sys_info.act_stan_vals[6] = 4;
		sys_info.act_stan_vals[7] = 10;
		sys_info.act_stan_vals[8] = 10;
		sys_info.act_stan_vals[9] = 20;
		sys_info.act_stan_vals[10] = 20;
		break;
	case SULPHUR:
		sys_info.chem_1a_qty = 1500;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 30;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 120;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 0;			  //
		sys_info.val_cal_y = 2;			  //
		sys_info.act_stan_vals[0] = 0;	  // verry low							1
		sys_info.act_stan_vals[1] = 1.00; // verry low						1
		sys_info.act_stan_vals[2] = 1.00; ////verry low						1
		sys_info.act_stan_vals[3] = 5;	  // low					//Deficient		2
		sys_info.act_stan_vals[4] = 10;	  // low				//Sufficient	2
		sys_info.act_stan_vals[5] = 20;	  // Medium				//Sufficient	3
		sys_info.act_stan_vals[6] = 30;	  // Medium								3
		sys_info.act_stan_vals[7] = 40;	  // High								4
		sys_info.act_stan_vals[8] = 40;	  // High								4
		sys_info.act_stan_vals[9] = 50;	  // Verry High 						5
		sys_info.act_stan_vals[10] = 50;  // Verry High 						5
		break;
	case POTASSIUM:

		sys_info.chem_1a_qty = 500;   /* Reagent 1A quantity for potassium test */
		sys_info.chem_1b_qty = 100;  /* Reagent 1B quantity for potassium test */
		sys_info.chem_2c_qty = 100;  /* Reagent 2C quantity for potassium test */
		sys_info.chem_3d_qty = 55;   /* Reagent 3D quantity for potassium test */
		sys_info.chem_4e_qty = 100;  /* Reagent 4E quantity for potassium test */
		sys_info.chem_5f_qty = 0;    /* Reagent 5F quantity = 0 (not used for potassium) */
		sys_info.wtr_g_qty = 520;    /* Water/glass wash quantity for potassium test */
		sys_info.led_pwm_red = 0xff;   /* Red LED PWM = 0xFF (max brightness, 255) */
		sys_info.led_pwm_green = 0xff; /* Green LED PWM = 0xFF (max brightness, 255) */
		sys_info.led_pwm_blue = 0xff;  /* Blue LED PWM = 0xFF (max brightness, 255) */
		sys_info.val_cal_x = 1; // green   /* Calibration X-axis = 1 (Green channel) */
		sys_info.val_cal_y = 2; // blue    /* Calibration Y-axis = 2 (Blue channel) */
		sys_info.act_stan_vals[0] = 0;    /* Standard 0 concentration = 0 ppm (blank) --> category 1: Very Low */
		sys_info.act_stan_vals[1] = 2;    /* Standard 1 concentration = 2 ppm --> category 1: Very Low */
		sys_info.act_stan_vals[2] = 4;    /* Standard 2 concentration = 4 ppm --> category 1: Very Low */
		sys_info.act_stan_vals[3] = 6;    /* Standard 3 concentration = 6 ppm --> category 2: Low */
		sys_info.act_stan_vals[4] = 6;    /* Standard 4 concentration = 6 ppm (duplicate) --> category 2: Low */
		sys_info.act_stan_vals[5] = 8;    /* Standard 5 concentration = 8 ppm --> category 3: Medium */
		sys_info.act_stan_vals[6] = 8;    /* Standard 6 concentration = 8 ppm (duplicate) --> category 3: Medium */
		sys_info.act_stan_vals[7] = 10;   /* Standard 7 concentration = 10 ppm --> category 4: High */
		sys_info.act_stan_vals[8] = 10;   /* Standard 8 concentration = 10 ppm (duplicate) --> category 4: High */
		sys_info.act_stan_vals[9] = 20;   /* Standard 9 concentration = 20 ppm --> category 5: Very High */
		sys_info.act_stan_vals[10] = 20;  /* Standard 10 concentration = 20 ppm (duplicate) --> category 5: Very High */
		break;
	case PHOSPHORUS:
		sys_info.chem_1a_qty = 1000; // 10.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 25;	// 0.25ml
		sys_info.chem_3d_qty = 200; // 2.00ml
		sys_info.chem_4e_qty = 100; // 1.00ml
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 475;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.1;
		sys_info.act_stan_vals[2] = 0.2;
		sys_info.act_stan_vals[3] = 0.4;
		sys_info.act_stan_vals[4] = 0.4;
		sys_info.act_stan_vals[5] = 0.6;
		sys_info.act_stan_vals[6] = 0.6;
		sys_info.act_stan_vals[7] = 0.8;
		sys_info.act_stan_vals[8] = 0.8;
		sys_info.act_stan_vals[9] = 1.0;
		sys_info.act_stan_vals[10] = 1.0;
		break;
	case NITROGEN:
		sys_info.chem_1a_qty = 1000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 100;
		sys_info.chem_3d_qty = 100;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 580;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 2;
		sys_info.act_stan_vals[2] = 2;
		sys_info.act_stan_vals[3] = 4;
		sys_info.act_stan_vals[4] = 4;
		sys_info.act_stan_vals[5] = 6;
		sys_info.act_stan_vals[6] = 6;
		sys_info.act_stan_vals[7] = 8;
		sys_info.act_stan_vals[8] = 8;
		sys_info.act_stan_vals[9] = 10;
		sys_info.act_stan_vals[10] = 10;
		break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
		sys_info.chem_1a_qty = 1000; // 10.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 1000; // 10.00ml
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
		break;
	}
	sys_info.wtr_f_wsh = 1300;
	sys_info.wtr_f_wsh_in_tm = sys_info.wtr_f_wsh * TM_MULTIPLIER;
	sys_info.drain_tm = (1000 * 55);	   // 55 sec
	sys_info.drain_mix_tm = (1000 * 10);   // 10 sec
	sys_info.tak_data_skp_tm = (10 * 25);  // solution rest time//25 sec
	sys_info.auto_zero_skp_tm = (10 * 25); // solution rest time//25 sec
	// sys_info.auto_set_skp_tm = (10*25);//solution rest time//25 sec

	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			sys_info.opt_std_vars.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars.strd_vars[i][j];
			sys_info.opt_std_vars2.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars2.strd_vars[i][j];
		}
	}

	sys_info.Stat_L.warm_up_stat = 0;
	sys_info.Stat_L.drain_wsh_stat = 0; // 1
}
#endif

void dflt_tm_set(void)
{
	save_sys_info.bk_var.chem_1a_in_tm = sys_info.chem_1a_qty * TM_MULTIPLIER; // time=qty x 20ms
	save_sys_info.bk_var.chem_1b_in_tm = sys_info.chem_1b_qty * TM_MULTIPLIER; // time=qty x 20ms
	save_sys_info.bk_var.chem_2c_in_tm = sys_info.chem_2c_qty * TM_MULTIPLIER; // time=qty x 20ms
	save_sys_info.bk_var.chem_3d_in_tm = sys_info.chem_3d_qty * TM_MULTIPLIER; // time=qty x 20ms
	save_sys_info.bk_var.chem_4e_in_tm = sys_info.chem_4e_qty * TM_MULTIPLIER; // time=qty x 20ms
	save_sys_info.bk_var.chem_5f_in_tm = sys_info.chem_5f_qty * TM_MULTIPLIER; // time=qty x 20ms
	save_sys_info.bk_var.wtr_g_in_tm = sys_info.wtr_g_qty * TM_MULTIPLIER;	   // time=qty x 20ms

	sys_info.sys_f.mem_save = 1;
}

void gpio_config_check(void)
{
	// gpio check
	//	sys_info.set_sys_add = MAGNESIUM;
	//	sys_info.set_sys_add = IRON;
	//	sys_info.set_sys_add = COPPER;
	//	sys_info.set_sys_add = ZINC;
	//	sys_info.set_sys_add = BORON;
     // sys_info.set_sys_add = SULPHUR;               //DONE
   	 sys_info.set_sys_add = POTASSIUM;               // DONE TESTING
    // sys_info.set_sys_add = PHOSPHORUS;             //DONR
//     sys_info.set_sys_add = NITROGEN;               // DONE
	//	sys_info.set_sys_add = ORGANIC_CARBON;
}

void sys_mem_validate(void)
{
	// note:- we should not do mid program parameter change, if we do it will remain till machine "ON"
	gpio_config_check();

	if ((mem_fresh_check()) || (sw_update()) || (validate_data()))
	{
		wrt_dflt();
#ifdef ARC_CAL
		sys_var_init();
#endif
		dflt_tm_set();
	}
	else
	{
#ifdef ARC_CAL
		sys_var_init();
#endif
	}
}

void global_timer_100msec(void)
{
	if (sys_info.Stat_L.warm_up_stat)
	{ // manage warmup time
		sys_info.wrm_up_tm++;
		if (sys_info.wrm_up_tm >= WRM_UP_TM)
		{
			sys_info.Stat_L.warm_up_stat = 0;
			sys_info.wrm_up_tm = WRM_UP_TM;
		}
	}

	if (sys_info.Stat_L.sen_stat)
	{
		// manage take data skipp time
		if (sys_info.Stat_M.tak_data_stat)
		{
			sys_info.tak_data_skp_run_tm++;
			/*if(sys_info.tak_data_skp_run_tm >= sys_info.tak_data_skp_tm)
			{ sys_info.tak_data_skp_run_tm = sys_info.tak_data_skp_tm; }
			*/
		}

		// manage take data skipp time
		if (sys_info.Stat_L.auto_zero)
		{
			sys_info.auto_zero_skp_run_tm++;
			/*if(sys_info.auto_zero_skp_run_tm >= sys_info.auto_zero_skp_tm)
			{ sys_info.auto_zero_skp_run_tm = sys_info.auto_zero_skp_tm; }
			*/
		}

		// manage auto_set skipp time
		if (sys_info.Stat_M2.auto_set)
		{
			sys_info.auto_set_skp_run_tm++;
			/*if(sys_info.auto_set_skp_run_tm >= sys_info.auto_set_skp_tm)
			{ sys_info.auto_set_skp_run_tm = sys_info.auto_set_skp_tm; }
			*/
		}
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
