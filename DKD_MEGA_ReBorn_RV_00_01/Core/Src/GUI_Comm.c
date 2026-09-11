/*
 * GUI_Comm.c
 *
 *  Created on: Nov 09, 2026
 *      Author: Yash_Choudhary
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "my_struct.h"
#include "GUI_Comm.h"
#include "Pump.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define COM_RX_ARR_SZ	30
#define RX_TMOUT		200
#define TX_DELAY		5//160
#define COM_STRT_ID1	0xAA
#define COM_STRT_ID2	0x99
#define COM_END_ID1		COM_STRT_ID2
#define COM_END_ID2		COM_STRT_ID1
#define COM_END_ID3		0x0D
#define COM_END_ID4		0x0A

#define COM_STRT_ID1_POS	0
#define COM_STRT_ID2_POS	1
#define COM_CMD_LEN_POS		2
#define COM_SRCS_ADD_POS	3
#define COM_DEST_ADD_POS	4
#define COM_CMD_TYP_POS		5
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
typedef struct GUI_Comm_variables
{
	UART_HandleTypeDef* uart_handler;
	HAL_StatusTypeDef handler_rx_stat;//HAL_OK = 0x00U, HAL_ERROR = 0x01U, HAL_BUSY = 0x02U, HAL_TIMEOUT = 0x03U//
	uint8_t rx_data;
	uint8_t rx_data_arr[COM_RX_ARR_SZ];
	uint8_t rx_data_arr_indx;
	uint16_t rx_tmout;
	uint8_t rx_cmd_exe;
	uint8_t tx_data_arr[COM_RX_ARR_SZ];
	uint8_t tx_delay;
	//HAL_StatusTypeDef tx_stat;
}GUI_Comm_typdef;

GUI_Comm_typdef GUI_Comm_var;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* *****************FUNCTIONS******************/
/* USER CODE BEGIN 1 */
void Comm_mode(uint8_t mode)
{
	if(mode)
	{
		TMT;
		sys_info.sys_f.comm_mode = TMT_MODE;
	}
	else if(!mode)
	{
		RCV;
		sys_info.sys_f.comm_mode = RCV_MODE;
	}
}
void GUI_Comm_init(UART_HandleTypeDef* uart_handler)
{
	Comm_mode(RCV_MODE);
	GUI_Comm_var.uart_handler = uart_handler;
	//GUI_Comm_var.tx_stat = HAL_ERROR;
	GUI_Comm_var.handler_rx_stat = HAL_ERROR;
	if(GUI_Comm_var.handler_rx_stat){ GUI_Comm_var.handler_rx_stat = HAL_UART_Receive_IT( GUI_Comm_var.uart_handler , &GUI_Comm_var.rx_data, 1);}

	/*GUI_Comm_var.rx_data = 0;
	for(uint8_t i = 0; i < COM_RX_ARR_SZ; i++)
	{
		GUI_Comm_var.rx_data_arr[i] = 0;
		GUI_Comm_var.tx_data_arr[i] = 0;
	}
	GUI_Comm_var.rx_data_arr_indx = 0;
	GUI_Comm_var.rx_tmout = 0;
	GUI_Comm_var.rx_cmd_exe = 0;
	GUI_Comm_var.tx_delay = 0;*/
}

void part_rx_reset(void)
{
	GUI_Comm_var.rx_data_arr_indx = 0;
	//for(uint8_t i = 0; i < COM_RX_ARR_SZ; i++){ GUI_Comm_var.rx_data_arr[i] = 0;}
	GUI_Comm_var.rx_cmd_exe = 0;
	GUI_Comm_var.rx_tmout = 0;
	GUI_Comm_var.tx_delay = 0;
	//GUI_Comm_var.tx_stat = HAL_ERROR;
}

void rx_reset(void)
{
	part_rx_reset();
	Comm_mode(RCV_MODE);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  //UNUSED(huart);
	if( (huart->Instance) == USART2)
	{
		GUI_Comm_var.rx_data_arr[GUI_Comm_var.rx_data_arr_indx] = GUI_Comm_var.rx_data;
		GUI_Comm_var.rx_data_arr_indx++; GUI_Comm_var.rx_tmout = 0;//GUI_Comm_var.rx_data = 0;
		GUI_Comm_var.handler_rx_stat = HAL_ERROR;
		if(GUI_Comm_var.handler_rx_stat){ GUI_Comm_var.handler_rx_stat = HAL_UART_Receive_IT( GUI_Comm_var.uart_handler , &GUI_Comm_var.rx_data, 1);}
		//HAL_UART_Receive_IT( GUI_Comm_var.uart_handler , &GUI_Comm_var.rx_data, 1);
	}

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  //UNUSED(huart);

	if( (huart->Instance) == USART2)
	{
		for(uint8_t i = 0; i < COM_RX_ARR_SZ; i++)
		{
			GUI_Comm_var.rx_data_arr[i] = 0;
			GUI_Comm_var.tx_data_arr[i] = 0;
		}
		rx_reset();
	}

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_TxCpltCallback can be implemented in the user file.
   */
}

uint8_t check_crc(void)
{
	uint8_t y, ChkStat, ChkByt;
	ChkStat = 0; ChkByt = 0;
	y = GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS];
	y = y - (6+2);
	if( ( (y%2) == 0 ) && ( ( y / 2) >= 3 ) )
	{
		y = y/2;
		for(uint8_t i = 0; i < y; i++){ ChkByt = ChkByt | GUI_Comm_var.rx_data_arr[(i*2) + 2];}
		if(ChkByt == GUI_Comm_var.rx_data_arr[(y*2) + 2]){ ChkStat = 1;}
		else
		{
			for(uint8_t i = 0; i < COM_RX_ARR_SZ; i++)
			{GUI_Comm_var.rx_data_arr[i] = 0;}
			return 0;
		}

		ChkByt = 0;
		for(uint8_t i = 0; i < y; i++){ ChkByt = ChkByt | GUI_Comm_var.rx_data_arr[(i*2) + 3];}
		if( ( ChkByt == GUI_Comm_var.rx_data_arr[(y*2) + 3] ) && (ChkStat == 1) ){ ChkStat = 1;}
		else
		{
			for(uint8_t i = 0; i < COM_RX_ARR_SZ; i++)
			{GUI_Comm_var.rx_data_arr[i] = 0;}
			return 0;
		}
	}
	else{return 0;}
	return ChkStat;
}

uint32_t updtval( uint16_t rcv_ml , uint16_t act_ml, uint32_t ref_tm)
{//calculate the correct motor run time
	uint16_t diff_ml;
	double persnt;
	uint32_t crction, crct_tm;

	if( act_ml > rcv_ml)
	{
		diff_ml = act_ml - rcv_ml;
		persnt = ( (double)diff_ml * 100.00 ) / (double)rcv_ml;
		crction = (uint32_t)( ( persnt * (double)ref_tm ) / 100.00);
		uint32_t x, y;
		x = ref_tm;
		y = crction;
		if( ( x + y ) >= 65535000 ){crct_tm = 65535000;}
		else{crct_tm = ref_tm + crction;}
	}
	else if( rcv_ml > act_ml)
	{
		diff_ml = rcv_ml - act_ml;
		persnt = (float)( ( (float)diff_ml * 100.00 ) / (float)rcv_ml);
		crction = (uint32_t)( persnt * (float)ref_tm ) / 100.00;
		int32_t x, y;
		x = (int32_t)ref_tm;
		y = (int32_t)crction;
		if( ( x - y ) <= 0 ){crct_tm = 0;}
		else{crct_tm = ref_tm - crction;}
	}
	else if( act_ml == rcv_ml){crct_tm = ref_tm;}
	return crct_tm;
}

void calpump(void)
{//update the motor run time according to saved qty and received qty
	uint16_t rcv_qty;
	rcv_qty = (uint16_t)( ( (uint16_t)GUI_Comm_var.rx_data_arr[10] * 100) +
			(uint16_t)GUI_Comm_var.rx_data_arr[11]);                          // received qty in ml from GUI
	if( GUI_Comm_var.rx_data_arr[6] == 0x80 )
	{
		save_sys_info.bk_var.chem_1a_in_tm =
			updtval( rcv_qty, sys_info.chem_1a_qty , save_sys_info.bk_var.chem_1a_in_tm);
	}
	else if( GUI_Comm_var.rx_data_arr[6] == 0x40 )
	{
		save_sys_info.bk_var.chem_1b_in_tm =
			updtval( rcv_qty, sys_info.chem_1b_qty , save_sys_info.bk_var.chem_1b_in_tm);
	}
	else if( GUI_Comm_var.rx_data_arr[6] == 0x20 )
	{
		save_sys_info.bk_var.chem_2c_in_tm =
			updtval( rcv_qty, sys_info.chem_2c_qty , save_sys_info.bk_var.chem_2c_in_tm);
	}
	else if( GUI_Comm_var.rx_data_arr[6] == 0x10 )
	{
		save_sys_info.bk_var.chem_3d_in_tm =
			updtval( rcv_qty, sys_info.chem_3d_qty , save_sys_info.bk_var.chem_3d_in_tm);
	}
	else if( GUI_Comm_var.rx_data_arr[6] == 0x08 )
	{
		save_sys_info.bk_var.chem_4e_in_tm =
			updtval( rcv_qty, sys_info.chem_4e_qty , save_sys_info.bk_var.chem_4e_in_tm);
	}
	else if( GUI_Comm_var.rx_data_arr[6] == 0x04 )
	{
		save_sys_info.bk_var.chem_5f_in_tm =
			updtval( rcv_qty, sys_info.chem_5f_qty , save_sys_info.bk_var.chem_5f_in_tm);
	}
	else if( GUI_Comm_var.rx_data_arr[6] == 0x02 )
	{
		save_sys_info.bk_var.wtr_g_in_tm =
			updtval( rcv_qty, sys_info.wtr_g_qty , save_sys_info.bk_var.wtr_g_in_tm);
	}
	/*if( GUI_Comm_var.rx_data_arr[7] == 0x80 )
	{
		WFWSHFCNTL = UpdtVal( Rx_Data[8] , Rx_Data[9], WFWMl, WFWDMl , WFWSHFCNTL);
	}*/

	sys_info.sys_f.mem_save = 1;
}

uint16_t SendActVal(void)
{
	double val;
	val = 0.00;
	if( (save_sys_info.bk_var.curr_sys_add == MAGNESIUM) ||
		(save_sys_info.bk_var.curr_sys_add == IRON) ||
		(save_sys_info.bk_var.curr_sys_add == SULPHUR) ||
		(save_sys_info.bk_var.curr_sys_add == PHOSPHORUS) ||
		(save_sys_info.bk_var.curr_sys_add == NITROGEN) )               //changed from val = val
	{
		val = sys_info.curr_ResVal * 100.00;
	}
	else if( (save_sys_info.bk_var.curr_sys_add == COPPER) ||
			(save_sys_info.bk_var.curr_sys_add == ZINC) ||
			(save_sys_info.bk_var.curr_sys_add == BORON) )
	{
		val = sys_info.curr_ResVal * 1000.00;
	}

	else if( (save_sys_info.bk_var.curr_sys_add == POTASSIUM) )
	{
		val = sys_info.curr_ResVal *100.00;        /* POTASSIUM GUI scaling: multiply result by 100 to preserve 2 decimal places (e.g., 12.45 ppm --> 1245 for transmission) */
	}

	else if( (save_sys_info.bk_var.curr_sys_add == ORGANIC_CARBON) )
	{
		val = sys_info.curr_ResVal * 10000;
	}
	if(val >= 65534)
	{
		val = 65534;
	}
	return (uint16_t)val;
}

void ExecuteCMD(void)
{
	if( GUI_Comm_var.rx_data_arr[COM_CMD_TYP_POS] == 0x10)
	{
		uint8_t y;
		y = GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS];
		GUI_Comm_var.rx_data_arr[0] = 0xAA;
		GUI_Comm_var.rx_data_arr[1] = 0x99;
		GUI_Comm_var.rx_data_arr[2] = 0x18;
		GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS] = GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS];
		GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS] = y;
		GUI_Comm_var.rx_data_arr[5] = 0x10;
		GUI_Comm_var.rx_data_arr[6] = sys_info.Stat_M.Byte;
		GUI_Comm_var.rx_data_arr[7] = sys_info.Stat_L.Byte | 0x01;
		GUI_Comm_var.rx_data_arr[8] = sys_info.Stat_M2.Byte;
		GUI_Comm_var.rx_data_arr[9] = sys_info.Stat_L2.Byte;
		GUI_Comm_var.rx_data_arr[10] = (uint8_t)((sys_info.curr_rgbc_vars.curr_red_rcv & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[11] = (uint8_t)((sys_info.curr_rgbc_vars.curr_red_rcv & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[12] = (uint8_t)((sys_info.curr_rgbc_vars.curr_green_rcv & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[13] = (uint8_t)((sys_info.curr_rgbc_vars.curr_green_rcv & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[14] = (uint8_t)((sys_info.curr_rgbc_vars.curr_blue_rcv & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[15] = (uint8_t)((sys_info.curr_rgbc_vars.curr_blue_rcv & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[16] = (uint8_t)((sys_info.curr_rgbc_vars.curr_clear_rcv & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[17] = (uint8_t)((sys_info.curr_rgbc_vars.curr_clear_rcv & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[18] = 0x00;
		GUI_Comm_var.rx_data_arr[19] = 0x00;
		GUI_Comm_var.rx_data_arr[20] = 0x99;
		GUI_Comm_var.rx_data_arr[21] = 0xAA;
		GUI_Comm_var.rx_data_arr[22] = 0x0D;
		GUI_Comm_var.rx_data_arr[23] = 0x0A;
	}
	else if( GUI_Comm_var.rx_data_arr[COM_CMD_TYP_POS] == 0x20)
	{
		if(sys_info.sys_f.ppfillf == 1)
		{
			pump1(0);
			pump2(0);
			pump3(0);
			pump4(0);
			pump5(0);
			pump6(0);
			pump7(0);
			pump8(0);
			motor1(0);
			mot_tm_rst();
		}
		uint8_t y;
		y = GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS];
		sys_info.Stat_M.Byte = sys_info.Stat_M.Byte | ( GUI_Comm_var.rx_data_arr[6] & 0xFF );
		sys_info.Stat_L.Byte = sys_info.Stat_L.Byte | ( GUI_Comm_var.rx_data_arr[7] & 0xF8 );
		sys_info.Stat_M2.Byte = sys_info.Stat_M2.Byte | ( GUI_Comm_var.rx_data_arr[8] & 0xFF );
		sys_info.Stat_L2.Byte = sys_info.Stat_L2.Byte | ( GUI_Comm_var.rx_data_arr[9] & 0xFF );
		GUI_Comm_var.rx_data_arr[0] = 0xAA;
		GUI_Comm_var.rx_data_arr[1] = 0x99;
		GUI_Comm_var.rx_data_arr[2] = 0x10;
		GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS] = GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS];
		GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS] = y;
		GUI_Comm_var.rx_data_arr[5] = 0x20;
		GUI_Comm_var.rx_data_arr[6] = GUI_Comm_var.rx_data_arr[6];
		GUI_Comm_var.rx_data_arr[7] = GUI_Comm_var.rx_data_arr[7] | 0x01;
		GUI_Comm_var.rx_data_arr[8] = GUI_Comm_var.rx_data_arr[8];
		GUI_Comm_var.rx_data_arr[9] = GUI_Comm_var.rx_data_arr[9];
		GUI_Comm_var.rx_data_arr[10] = 0x00;
		GUI_Comm_var.rx_data_arr[11] = 0x00;
		GUI_Comm_var.rx_data_arr[12] = 0x99;
		GUI_Comm_var.rx_data_arr[13] = 0xAA;
		GUI_Comm_var.rx_data_arr[14] = 0x0D;
		GUI_Comm_var.rx_data_arr[15] = 0x0A;
	}
	else if( GUI_Comm_var.rx_data_arr[COM_CMD_TYP_POS] == 0x30) // get command in GUI

	{
		
		uint8_t y;
		uint16_t actval;
		actval = SendActVal();
		y = GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS];
		GUI_Comm_var.rx_data_arr[0] = 0xAA;
		GUI_Comm_var.rx_data_arr[1] = 0x99;
		GUI_Comm_var.rx_data_arr[2] = 0x10;
		GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS] = GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS];
		GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS] = y;
		GUI_Comm_var.rx_data_arr[5] = 0x30;
		GUI_Comm_var.rx_data_arr[6] = sys_info.curr_Result_cat;
		GUI_Comm_var.rx_data_arr[7] = GUI_Comm_var.rx_data_arr[7] | 0x01;
		GUI_Comm_var.rx_data_arr[8] = ((actval & 0xFF00) >> 8);              //curr_ResVal (PPM value)
		GUI_Comm_var.rx_data_arr[9] = ((actval & 0x00FF) >> 0);              //curr_ResVal (PPM value)
		GUI_Comm_var.rx_data_arr[10] = 0x00;
		GUI_Comm_var.rx_data_arr[11] = 0x00;
		GUI_Comm_var.rx_data_arr[12] = 0x99;
		GUI_Comm_var.rx_data_arr[13] = 0xAA;
		GUI_Comm_var.rx_data_arr[14] = 0x0D;
		GUI_Comm_var.rx_data_arr[15] = 0x0A;
	}

	//aa991870ff10001300003cd43560370d9bd2ffff99aa0d0a


	else if( GUI_Comm_var.rx_data_arr[COM_CMD_TYP_POS] == 0x40) // Pip fill command
	{
		uint8_t y;
		y = GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS];
		sys_info.sys_f.ppfillf = 1;
		if( (GUI_Comm_var.rx_data_arr[7] & 0x08) == 0x08)
		{sys_info.Stat_M.Byte = sys_info.Stat_M.Byte | (GUI_Comm_var.rx_data_arr[6] & 0xBE);}
		else if( (GUI_Comm_var.rx_data_arr[7] & 0x08) != 0x08)
		{sys_info.Stat_M.Byte = sys_info.Stat_M.Byte & (~(GUI_Comm_var.rx_data_arr[6] & 0xBE));}
		//sys_info.Stat_L.Byte = GUI_Comm_var.rx_data_arr[7] | (sys_info.Stat_L.Byte & 0x06);
		GUI_Comm_var.rx_data_arr[0] = 0xAA;
		GUI_Comm_var.rx_data_arr[1] = 0x99;
		GUI_Comm_var.rx_data_arr[2] = 0x10;
		GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS] = GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS];
		GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS] = y;
		GUI_Comm_var.rx_data_arr[5] = 0x40;
		GUI_Comm_var.rx_data_arr[6] = GUI_Comm_var.rx_data_arr[6];
		GUI_Comm_var.rx_data_arr[7] = GUI_Comm_var.rx_data_arr[7] | 0x01;
		GUI_Comm_var.rx_data_arr[8] = 0x00;
		GUI_Comm_var.rx_data_arr[9] = 0x00;
		GUI_Comm_var.rx_data_arr[10] = 0x00;
		GUI_Comm_var.rx_data_arr[11] = 0x00;
		GUI_Comm_var.rx_data_arr[12] = 0x99;
		GUI_Comm_var.rx_data_arr[13] = 0xAA;
		GUI_Comm_var.rx_data_arr[14] = 0x0D;
		GUI_Comm_var.rx_data_arr[15] = 0x0A;
	}
	else if( GUI_Comm_var.rx_data_arr[COM_CMD_TYP_POS] == 0x50) // Pump Calibration
	{
		uint8_t y;
		y = GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS];
		calpump();
		GUI_Comm_var.rx_data_arr[0] = 0xAA;
		GUI_Comm_var.rx_data_arr[1] = 0x99;
		GUI_Comm_var.rx_data_arr[2] = 0x12;
		GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS] = GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS];
		GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS] = y;
		GUI_Comm_var.rx_data_arr[5] = 0x50;
		GUI_Comm_var.rx_data_arr[6] = GUI_Comm_var.rx_data_arr[6];
		GUI_Comm_var.rx_data_arr[7] = GUI_Comm_var.rx_data_arr[7] | 0x01;
		GUI_Comm_var.rx_data_arr[8] = 0x00;
		GUI_Comm_var.rx_data_arr[9] = 0x00;
		GUI_Comm_var.rx_data_arr[10] = GUI_Comm_var.rx_data_arr[10];
		GUI_Comm_var.rx_data_arr[11] = GUI_Comm_var.rx_data_arr[11];
		GUI_Comm_var.rx_data_arr[12] = 0x00;
		GUI_Comm_var.rx_data_arr[13] = 0x00;
		GUI_Comm_var.rx_data_arr[14] = 0x99;
		GUI_Comm_var.rx_data_arr[15] = 0xAA;
		GUI_Comm_var.rx_data_arr[16] = 0x0D;
		GUI_Comm_var.rx_data_arr[17] = 0x0A;
	}

	else if( GUI_Comm_var.rx_data_arr[COM_CMD_TYP_POS] == 0x6A) // sends RGBC auto-zero drift % values (scaled by 100, e.g., -377 = -3.77%)
	{
		//24 total frame length
		uint8_t y;
		y = GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS];
		GUI_Comm_var.rx_data_arr[0] = 0xAA;
		GUI_Comm_var.rx_data_arr[1] = 0x99;
		GUI_Comm_var.rx_data_arr[2] = 0x18;   // CMD length
		GUI_Comm_var.rx_data_arr[COM_SRCS_ADD_POS] = GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS];
		GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS] = y;
		GUI_Comm_var.rx_data_arr[5] = 0x6A;
		GUI_Comm_var.rx_data_arr[6] = sys_info.Stat_M.Byte;
		GUI_Comm_var.rx_data_arr[7] = sys_info.Stat_L.Byte | 0x01;
		GUI_Comm_var.rx_data_arr[8] = 0x00;
		GUI_Comm_var.rx_data_arr[9] = 0x00;
		GUI_Comm_var.rx_data_arr[10] = (uint8_t)((sys_info.rgbc_drift[0] & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[11] = (uint8_t)((sys_info.rgbc_drift[0] & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[12] = (uint8_t)((sys_info.rgbc_drift[1] & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[13] = (uint8_t)((sys_info.rgbc_drift[1] & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[14] = (uint8_t)((sys_info.rgbc_drift[2] & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[15] = (uint8_t)((sys_info.rgbc_drift[2] & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[16] = (uint8_t)((sys_info.rgbc_drift[3] & 0xFF00) >> 8);
		GUI_Comm_var.rx_data_arr[17] = (uint8_t)((sys_info.rgbc_drift[3] & 0x00FF) >> 0);
		GUI_Comm_var.rx_data_arr[18] = 0x00;
		GUI_Comm_var.rx_data_arr[19] = 0x00;
		GUI_Comm_var.rx_data_arr[20] = 0x99;
		GUI_Comm_var.rx_data_arr[21] = 0xAA;
		GUI_Comm_var.rx_data_arr[22] = 0x0D;
		GUI_Comm_var.rx_data_arr[23] = 0x0A;
	}
	uint8_t a, b, ChkByt; //x
	ChkByt = 0;
	a = GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS];
	a = a - (6+2);
	b = a / 2;
	for(uint8_t i = 0; i < b; i++)
	{ChkByt = ChkByt | GUI_Comm_var.rx_data_arr[ ( i * 2 ) + 2 ];}
	GUI_Comm_var.rx_data_arr[ ( b * 2 ) + 2 ] = ChkByt;
	ChkByt = 0;
	for(uint8_t i = 0; i < b; i++)
	{
		ChkByt = ChkByt | GUI_Comm_var.rx_data_arr[ ( i * 2 ) + 3 ];
	}
	GUI_Comm_var.rx_data_arr[ ( b * 2 ) + 3 ] = ChkByt;
	b = GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS];
	for(uint8_t i = 0; i < b; i++)
	{
		GUI_Comm_var.tx_data_arr[i] = GUI_Comm_var.rx_data_arr[i];
		GUI_Comm_var.rx_data_arr[i] = 0;
	}
	part_rx_reset();
	//HAL_UART_Transmit(GUI_Comm_var.uart_handler, (uint8_t*)GUI_Comm_var.tx_data_arr, 12, 100);//(uint16_t)GUI_Comm_var.tx_data_arr[COM_CMD_LEN_POS]
	HAL_UART_Transmit_IT(GUI_Comm_var.uart_handler, GUI_Comm_var.tx_data_arr, GUI_Comm_var.tx_data_arr[COM_CMD_LEN_POS]);
	//rx_reset();
	//here i m sending packet to GUI , rx data copy to in tx arr
}

void ProcessCMD(void)
{
	if(check_crc())
//	if(1)
	{
		if( GUI_Comm_var.rx_data_arr[COM_DEST_ADD_POS] == save_sys_info.bk_var.curr_sys_add)
		{
			ExecuteCMD();
		}else{
			rx_reset();
		}
	}
	else{rx_reset();}
}

void GUI_timer_1msec(void) // called in 1msec timer interrupt and frame assembly is done in this function
{
	if(!GUI_Comm_var.rx_cmd_exe)
	{
		if( ( GUI_Comm_var.rx_data_arr[COM_STRT_ID1_POS] == COM_STRT_ID1 )  && ( GUI_Comm_var.rx_data_arr_indx > COM_STRT_ID1_POS ) )
		{
			if( ( GUI_Comm_var.rx_data_arr[COM_STRT_ID2_POS] == COM_STRT_ID2 ) && ( GUI_Comm_var.rx_data_arr_indx > COM_STRT_ID2_POS ) )
			{
				if( ( ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] != 0 ) && ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] <= COM_RX_ARR_SZ ) ) && ( GUI_Comm_var.rx_data_arr_indx > COM_CMD_LEN_POS ) )
				{
					if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 4 ] ) == COM_END_ID1 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 4 ) ) )
					{
						if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 3 ] ) == COM_END_ID2 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 3 ) ) )
						{
							if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 2 ] ) == COM_END_ID3 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 2 ) ) )
							{
								if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 1 ] ) == COM_END_ID4 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 1 ) ) )
								{
									GUI_Comm_var.rx_cmd_exe = 1; Comm_mode(TMT_MODE);
									//for(uint8_t i = 0; i < COM_RX_ARR_SZ; i++){GUI_Comm_var.tx_data_arr[i] = 0;}//////
								}
								else if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 1 ] ) != COM_END_ID4 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 1 ) ) ){rx_reset();}
							}
							else if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 2 ] ) != COM_END_ID3 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 2 ) ) ){rx_reset();}
						}
						else if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 3 ] ) != COM_END_ID2 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 3 ) ) ){rx_reset();}
					}
					else if( ( ( GUI_Comm_var.rx_data_arr[ GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 4 ] ) != COM_END_ID1 ) && ( GUI_Comm_var.rx_data_arr_indx > ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] - 4 ) ) ){rx_reset();}
				}
				else if( ( ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] == 0) || ( GUI_Comm_var.rx_data_arr[COM_CMD_LEN_POS] > COM_RX_ARR_SZ ) ) && ( GUI_Comm_var.rx_data_arr_indx > COM_CMD_LEN_POS ) ){rx_reset();}
			}
			else if( ( GUI_Comm_var.rx_data_arr[COM_STRT_ID2_POS] != COM_STRT_ID2 ) && ( GUI_Comm_var.rx_data_arr_indx > COM_STRT_ID2_POS ) ){rx_reset();}
		}
		else if( ( GUI_Comm_var.rx_data_arr[COM_STRT_ID1_POS] != COM_STRT_ID1 )  && ( GUI_Comm_var.rx_data_arr_indx > COM_STRT_ID1_POS ) ){rx_reset();}
	}
	else if(GUI_Comm_var.rx_cmd_exe)
	{ if(GUI_Comm_var.tx_delay < TX_DELAY){GUI_Comm_var.tx_delay++;} }

	if( (GUI_Comm_var.rx_data_arr_indx > 0) && (GUI_Comm_var.rx_tmout < RX_TMOUT) ){GUI_Comm_var.rx_tmout++;}
	if( (GUI_Comm_var.rx_tmout >= RX_TMOUT) && (!GUI_Comm_var.rx_cmd_exe) ){rx_reset();}
}

void GUI_Comm_handler(void)
{
	if(GUI_Comm_var.handler_rx_stat){ GUI_Comm_var.handler_rx_stat = HAL_UART_Receive_IT( GUI_Comm_var.uart_handler , &GUI_Comm_var.rx_data, 1);}
	if( ( GUI_Comm_var.rx_cmd_exe == 1) && ( GUI_Comm_var.tx_delay >= TX_DELAY) )
	{
		ProcessCMD();
	}

	/*else if( ( GUI_Comm_var.rx_cmd_exe == 1) && ( GUI_Comm_var.tx_delay < TX_DELAY) )
	{for(uint8_t i = 0; i < COM_RX_ARR_SZ; i++){GUI_Comm_var.tx_data_arr[i] = 0;}}*/
}

/* USER CODE END 1 */

