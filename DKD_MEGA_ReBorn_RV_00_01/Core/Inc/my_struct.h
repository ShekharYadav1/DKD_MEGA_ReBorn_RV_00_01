/*
 * my_struct.h
 *
 *  Created on: Feb 13, 2026
 *      Author: Yash.Choudhary
 */

#ifndef INC_MY_STRUCT_H_
#define INC_MY_STRUCT_H_

/* Includes ------------------------------------------------------------------*/


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g0xx_hal.h"  // Adjust to your MCU series
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define MEM_SIZE 30
#define MEM_STRT_ADD 0x0800F800

#define WRM_UP_TM (10*60*30)//10 * X(sec/min) * Y(min) millisec//(10*30)//

#include "stm32g0xx.h"   // Device header (adjust for your MCU)

typedef enum sensor_ok_or_not
{
	SEN_NG = 0,
	SEN_OK = 1
}sen_stat_typdef;

// Union for Stat_M
typedef union MSB_Satus_bit_byte_define
{
	uint8_t Byte;   // Full byte access
	struct
	{
		uint8_t tak_data_stat     : 1;	//0_bit lsb
		uint8_t water_g_disp_stat : 1;	//1_bit
		uint8_t chem_5f_disp_stat : 1;	//2_bit
		uint8_t chem_4e_disp_stat : 1;	//3_bit
		uint8_t chem_3d_disp_stat : 1;	//4_bit
		uint8_t chem_2c_disp_stat : 1;	//5_bit
		uint8_t chem_1b_disp_stat : 1;	//6_bit
		uint8_t chem_1a_disp_stat : 1;	//7_bit msb
	};
}byte_M_typedef;

// Union for Stat_L
typedef union LSB_Satus_bit_byte_define
{
	uint8_t Byte;   // Full word access
	struct
	{
		uint8_t fixed          : 1;	//0_bit lsb
		uint8_t sen_stat       : 1;	//1_bit
		uint8_t warm_up_stat   : 1;	//2_bit
		uint8_t alt_func       : 1;	//3_bit
		uint8_t auto_zero_save : 1;	//4_bit
		uint8_t auto_zero      : 1;	//5_bit
		uint8_t drain_only     : 1;	//6_bit
		uint8_t drain_wsh_stat : 1;	//7_bit msb
	};
} byte_L_typedef;

// Union for Stat2_M
typedef union MSB_Satus_bit_byte_define_for_second_pair
{
	uint8_t Byte;   // Full byte access
	struct
	{
		uint8_t reserve1	: 1;	//0_bit lsb
		uint8_t reserve2	: 1;	//1_bit
		uint8_t reserve3	: 1;	//2_bit
		uint8_t reserve4	: 1;	//3_bit
		uint8_t reserve5	: 1;	//4_bit
		uint8_t reserve6	: 1;	//5_bit
		uint8_t reserve7	: 1;	//6_bit
		uint8_t auto_set	: 1;	//7_bit msb
	};
}byte_M2_typedef;

// Union for Stat2_L
typedef union LSB_Satus_bit_byte_define_for_second_pair
{
	uint8_t Byte;   // Full word access
	struct
	{
		uint8_t reserve1	: 1;	//0_bit lsb
		uint8_t reserve2	: 1;	//1_bit
		uint8_t reserve3	: 1;	//2_bit
		uint8_t reserve4	: 1;	//3_bit
		uint8_t reserve5	: 1;	//4_bit
		uint8_t reserve6	: 1;	//5_bit
		uint8_t reserve7	: 1;	//6_bit
		uint8_t reserve8	: 1;	//7_bit msb
	};
}byte_L2_typedef;

// Union for pump_stat
typedef union byte_status_for_pumps_n_motors_Satus_bit_byte_define
{
	uint16_t Bytes;   // Full byte access
	struct
	{
		uint16_t pump1		: 1;	//0_bit lsb
		uint16_t pump2		: 1;	//1_bit
		uint16_t pump3		: 1;	//2_bit
		uint16_t pump4      : 1;	//3_bit
		uint16_t pump5      : 1;	//4_bit
		uint16_t pump6      : 1;	//5_bit
		uint16_t pump7      : 1;	//6_bit
		uint16_t pump8		: 1;	//7_bit msb

		uint16_t motor      : 1;	//8_bit lsb
		uint16_t reserve7   : 1;	//9_bit
		uint16_t reserve6	: 1;	//10_bit
		uint16_t reserve5   : 1;	//11_bit
		uint16_t reserve4   : 1;	//12_bit
		uint16_t reserve3   : 1;	//13_bit
		uint16_t reserve2   : 1;	//14_bit
		uint16_t reserve1	: 1;	//15_bit msb
	};
} pump_s_typedef;

// Union for System flags
typedef union System_flags_bit_byte_define
{
	uint8_t Byte;   // Full word access
	struct
	{
		uint8_t ppfillf			: 1;	//0_bit lsb
		uint8_t mem_save		: 1;	//1_bit
		uint8_t comm_mode		: 1;	//2_bit//transmit - 1, receive - 0
		uint8_t reserve5		: 1;	//3_bit
		uint8_t reserve4		: 1;	//4_bit
		uint8_t reserve3		: 1;	//5_bit
		uint8_t reserve2		: 1;	//6_bit
		uint8_t reserve1		: 1;	//7_bit msb
	};
}sys_f_t;

typedef enum system_address
{
	MAGNESIUM =		0x10,
	IRON = 			0x20,
	COPPER = 		0x30,
	ZINC = 			0x40,
	BORON = 		0x50,
	SULPHUR = 		0x60,
	POTASSIUM = 	0x70,  /* Potassium system/I2C slave address = 0x70, used to identify and select potassium test subsystem */
	PHOSPHORUS = 	0x80,
	NITROGEN = 		0x90,
	ORGANIC_CARBON = 0xA0,  // this for organic carbon slave address
	MASTER_SYS = 0xFF
}sys_add_typdef;

#define NOS_STD 11
typedef union standard_variables_union_structure
{
	uint16_t strd_vars[NOS_STD][4];
	struct
	{
		uint16_t stan_0_red, stan_0_green, stan_0_blue, stan_0_clear;
		uint16_t stan_1_red, stan_1_green, stan_1_blue, stan_1_clear;
		uint16_t stan_2_red, stan_2_green, stan_2_blue, stan_2_clear;
		uint16_t stan_3_red, stan_3_green, stan_3_blue, stan_3_clear;
		uint16_t stan_4_red, stan_4_green, stan_4_blue, stan_4_clear;
		uint16_t stan_5_red, stan_5_green, stan_5_blue, stan_5_clear;
		uint16_t stan_6_red, stan_6_green, stan_6_blue, stan_6_clear;
		uint16_t stan_7_red, stan_7_green, stan_7_blue, stan_7_clear;
		uint16_t stan_8_red, stan_8_green, stan_8_blue, stan_8_clear;
		uint16_t stan_9_red, stan_9_green, stan_9_blue, stan_9_clear;
		uint16_t stan_10_red, stan_10_green, stan_10_blue, stan_10_clear;
	};

}unn_std_var_typdef;

/*typedef union actual_standard_values_in_units
{
	double act_stan_val[NOS_STD];
	struct
	{
		double act_stan_0_val, act_stan_1_val, act_stan_2_val, act_stan_3_val,
		act_stan_4_val, act_stan_5_val, act_stan_6_val, act_stan_7_val, act_stan_8_val,
		act_stan_9_val, act_stan_10_val;
	};

}union_act_stan_val_typdef;*/

typedef union current_RGBC_read_variables
{
	uint16_t curr_rgbc_var[4];
	struct
	{
		uint16_t curr_red_rcv, curr_green_rcv, curr_blue_rcv, curr_clear_rcv;
	};

}union_curr_rgbc_var_typdef;

typedef struct system_variables
{
	uint8_t led_pwm_red, led_pwm_green, led_pwm_blue;
	volatile sys_add_typdef set_sys_add;
	uint16_t chem_1a_qty, chem_1b_qty, chem_2c_qty, chem_3d_qty, chem_4e_qty,
	chem_5f_qty, wtr_g_qty, wtr_f_wsh;//chemical quantity in ml 1300 means 13 + .00 ml
	uint32_t wtr_f_wsh_in_tm, drain_tm, drain_mix_tm;//uses 1msec pump timer
	uint32_t tak_data_skp_tm, tak_data_skp_run_tm, auto_zero_skp_tm, auto_zero_skp_run_tm, auto_set_skp_tm, auto_set_skp_run_tm;//uses 100msec global timer
	sys_f_t sys_f;
	uint16_t wrm_up_tm;
	volatile byte_M_typedef Stat_M;//CHEM_1A_DISP_STAT, CHEM_1B_DISP_STAT, CHEM_2C_DISP_STAT, CHEM_3D_DISP_STAT, CHEM_4E_DISP_STAT, CHEM_5F_DISP_STAT, WATER_G_DISP_STAT, TAK_DATA_STAT
	volatile byte_L_typedef Stat_L;//DRAIN_WSH_STAT, RESERVE, RESERVE, RESERVE, RESERVE, WARM_UP_STAT, SEN_STAT, N/A
	volatile byte_M2_typedef Stat_M2;//currently reserved
	volatile byte_L2_typedef Stat_L2;//currently reserved
	pump_s_typedef pump_stat;//only for status read
	union_curr_rgbc_var_typdef curr_rgbc_vars;
	//uint16_t curr_red_rcv, curr_green_rcv, curr_blue_rcv, curr_clear_rcv;
	unn_std_var_typdef opt_std_vars, opt_std_vars2;
	uint8_t val_cal_x, val_cal_y;
	uint64_t sum_x, sum_y, sum_xy, sum_xx;
	double mslp, cshft;
	double ref_stan_x[NOS_STD];
	double ref_stan_y[NOS_STD];
	double ref_stan_dist[NOS_STD];
	double act_stan_vals[NOS_STD];
	double curr_x, curr_y, curr_dist;
	double std_absrb_val[NOS_STD];
	double curr_absrb_val;

	//double std_absrb_val_mul[NOS_STD];
	//double std_absrb_sqr[NOS_STD];
	double std_multplr;

	double curr_ResVal;
	double curr_ResVal1;
	uint8_t curr_Result_cat;
	int16_t rgbc_drift[4];  // R,G,B,C drift % scaled by 100 (e.g., -377 = -3.77%)
}sys_var_typdef;

typedef struct backup_memory_variables
{
	uint8_t start_add1;
	uint8_t start_add2;
	uint8_t maj_sw_rv_no;
	uint8_t mid_sw_rv_no;
	uint8_t min_sw_rv_no;
	sys_add_typdef curr_sys_add;
	unn_std_var_typdef hrd_std_vars, hrd_std_vars2;
	uint32_t chem_1a_in_tm, chem_1b_in_tm, chem_2c_in_tm, chem_3d_in_tm, chem_4e_in_tm,
	chem_5f_in_tm, wtr_g_in_tm;
	uint8_t end_add1;
	uint8_t end_add2;
}bk_var_typdef;

typedef union backup_memory_variables_management
{
	uint64_t bkdata[MEM_SIZE];
	uint8_t bk_data[8*MEM_SIZE];
	bk_var_typdef bk_var;
}union_bk_var_typdef;


extern sys_var_typdef sys_info;
extern union_bk_var_typdef save_sys_info;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/


/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */



#endif /* INC_MY_STRUCT_H_ */
