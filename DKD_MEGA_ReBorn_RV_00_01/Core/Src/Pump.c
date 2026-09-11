/*
 * Pump.c
 *
 *  Created on: Feb 20, 2026
 *      Author: Yash Choudhary
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Pump.h"
#include "my_struct.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//#define DRAIN_TM (1000*60)//millisec
//#define MIX_TM (1000*10)//millisec

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


typedef struct drn_wsh_func_structure
{
	uint32_t chem_1a_disp_tm, chem_1b_disp_tm, chem_2c_disp_tm, chem_3d_disp_tm,
	chem_4e_disp_tm, chem_5f_disp_tm, water_g_disp_tm, drn_only_tm;
	uint64_t drain_wsh_tm;
}drn_wsh_typdef;

drn_wsh_typdef pump_func;

extern drn_wsh_typdef pump_func;
/* USER CODE END EV */

/* *****************FUNCTIONS******************/
/* USER CODE BEGIN 1 */



void pump1( uint8_t flag)
{//responsible to dispense chemical 1
	switch(flag)
	{
	case 0:
		PUMP1OFF;sys_info.pump_stat.pump1 = 0;break;
	case 1:
		PUMP1ON;sys_info.pump_stat.pump1 = 1;break;
	}
}

void pump2( uint8_t flag)
{//responsible to dispense chemical 2
	switch(flag)
	{
	case 0:
		PUMP2OFF;sys_info.pump_stat.pump2 = 0;break;
	case 1:
		PUMP2ON;sys_info.pump_stat.pump2 = 1;break;
	}
}

void pump3( uint8_t flag)
{//responsible to dispense chemical 3
	switch(flag)
	{
	case 0:
		PUMP3OFF;sys_info.pump_stat.pump3 = 0;break;
	case 1:
		PUMP3ON;sys_info.pump_stat.pump3 = 1;break;
	}
}

void pump4( uint8_t flag)
{//responsible to dispense chemical 4
	switch(flag)
	{
	case 0:
		PUMP4OFF;sys_info.pump_stat.pump4 = 0;break;
	case 1:
		PUMP4ON;sys_info.pump_stat.pump4 = 1;break;
	}
}

void pump5( uint8_t flag)
{//responsible to dispense chemical 5
	switch(flag)
	{
	case 0:
		PUMP5OFF;sys_info.pump_stat.pump5 = 0;break;
	case 1:
		PUMP5ON;sys_info.pump_stat.pump5 = 1;break;
	}
}

void pump6( uint8_t flag)
{//responsible to dispense water as chemical 6
	switch(flag)
	{
	case 0:
		PUMP6OFF;sys_info.pump_stat.pump6 = 0;break;
	case 1:
		PUMP6ON;sys_info.pump_stat.pump6 = 1;break;
	}
}

void pump7( uint8_t flag)
{//responsible to dispense water for washing tube internally
	switch(flag)
	{
	case 0:
		PUMP7OFF;sys_info.pump_stat.pump7 = 0;break;
	case 1:
		PUMP7ON;sys_info.pump_stat.pump7 = 1;break;
	}
}

void pump8( uint8_t flag)
{//responsible for drain the solution and dry the tube
	switch(flag)
	{
	case 0:
		PUMP8OFF;sys_info.pump_stat.pump8 = 0;break;
	case 1:
		PUMP8ON;sys_info.pump_stat.pump8 = 1;break;
	}
}

void motor1( uint8_t flag)
 { //responsible to mix the solution
	switch (flag)
	{
	case 0:
		MOTOR1OFF;sys_info.pump_stat.motor = 0;break; // MOTOR1OFF and MOTOR1ON this is micros
	case 1:
		MOTOR1ON;sys_info.pump_stat.motor = 1;break;
	}
}

void drn_wsh_func(void)
{
	if(pump_func.drain_wsh_tm < (sys_info.drain_tm) )
	{
		pump8(1);
	}//Sample Drain complete
	else if(pump_func.drain_wsh_tm < (sys_info.drain_tm + sys_info.wtr_f_wsh_in_tm) )
	{
		pump8(0);
		pump7(1);
	}
	else if(pump_func.drain_wsh_tm < (sys_info.drain_tm +
			sys_info.wtr_f_wsh_in_tm + sys_info.drain_mix_tm) )
	{
		pump7(0);
		motor1(1);
	}
	else if(pump_func.drain_wsh_tm < (sys_info.drain_tm + sys_info.wtr_f_wsh_in_tm +
			sys_info.drain_mix_tm + sys_info.drain_tm) )
	{
		motor1(0);
		pump8(1);
	}//First clean complete
	else if(pump_func.drain_wsh_tm < (sys_info.drain_tm + sys_info.wtr_f_wsh_in_tm +
			sys_info.drain_mix_tm + sys_info.drain_tm +
			sys_info.wtr_f_wsh_in_tm) )
	{
		pump8(0);
		pump7(1);
	}
	else if(pump_func.drain_wsh_tm < (sys_info.drain_tm + sys_info.wtr_f_wsh_in_tm +
			sys_info.drain_mix_tm + sys_info.drain_tm +
			sys_info.wtr_f_wsh_in_tm + sys_info.drain_mix_tm) )
	{
		pump7(0);
		motor1(1);
	}
	else if(pump_func.drain_wsh_tm < (sys_info.drain_tm + sys_info.wtr_f_wsh_in_tm +
			sys_info.drain_mix_tm + sys_info.drain_tm +
			sys_info.wtr_f_wsh_in_tm + sys_info.drain_mix_tm +
			sys_info.drain_tm ) )
	{
		motor1(0);
		pump8(1);
	}//Second clean complete
	else if(pump_func.drain_wsh_tm >= (sys_info.drain_tm + sys_info.wtr_f_wsh_in_tm +
			sys_info.drain_mix_tm + sys_info.drain_tm +
			sys_info.wtr_f_wsh_in_tm + sys_info.drain_mix_tm +
			sys_info.drain_tm) )
	{
		motor1(0);
		pump7(0);
		pump8(0);
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 0;
		sys_info.Stat_L.drain_wsh_stat = 0;
		pump_func.drain_wsh_tm = 0;
	}
}

void pump_timer_1msec(void)
{
	if(sys_info.Stat_L.drain_wsh_stat){pump_func.drain_wsh_tm++;}
	if(sys_info.Stat_M.chem_1a_disp_stat){pump_func.chem_1a_disp_tm++;}
	if(sys_info.Stat_M.chem_1b_disp_stat){pump_func.chem_1b_disp_tm++;}
	if(sys_info.Stat_M.chem_2c_disp_stat){pump_func.chem_2c_disp_tm++;}
	if(sys_info.Stat_M.chem_3d_disp_stat){pump_func.chem_3d_disp_tm++;}
	if(sys_info.Stat_M.chem_4e_disp_stat){pump_func.chem_4e_disp_tm++;}
	if(sys_info.Stat_M.chem_5f_disp_stat){pump_func.chem_5f_disp_tm++;}
	if(sys_info.Stat_M.water_g_disp_stat){pump_func.water_g_disp_tm++;}
	if(sys_info.Stat_L.drain_only){pump_func.drn_only_tm++;}

}

void chem_1a_disp(void)
{
	if(pump_func.chem_1a_disp_tm < save_sys_info.bk_var.chem_1a_in_tm){pump1(1);}
	else if(pump_func.chem_1a_disp_tm >= save_sys_info.bk_var.chem_1a_in_tm)
	{
		pump1(0);
		pump_func.chem_1a_disp_tm = 0;
		sys_info.Stat_M.chem_1a_disp_stat = 0;
	}
}

void chem_1b_disp(void)
{
	if(pump_func.chem_1b_disp_tm < save_sys_info.bk_var.chem_1b_in_tm){pump1(1);}
	else if(pump_func.chem_1b_disp_tm >= save_sys_info.bk_var.chem_1b_in_tm)
	{
		pump1(0);
		pump_func.chem_1b_disp_tm = 0;
		sys_info.Stat_M.chem_1b_disp_stat = 0;
	}
}

void chem_2c_disp(void)
{
	if(pump_func.chem_2c_disp_tm < save_sys_info.bk_var.chem_2c_in_tm){pump2(1);}
	else if(pump_func.chem_2c_disp_tm >= save_sys_info.bk_var.chem_2c_in_tm)
	{
		pump2(0);
		pump_func.chem_2c_disp_tm = 0;
		sys_info.Stat_M.chem_2c_disp_stat = 0;
	}
}

void chem_3d_disp(void)
{
	if(pump_func.chem_3d_disp_tm < save_sys_info.bk_var.chem_3d_in_tm){pump3(1);}
	else if(pump_func.chem_3d_disp_tm >= save_sys_info.bk_var.chem_3d_in_tm)
	{
		pump3(0);
		pump_func.chem_3d_disp_tm = 0;
		sys_info.Stat_M.chem_3d_disp_stat = 0;
	}
}

void chem_4e_disp(void)
{
	if(pump_func.chem_4e_disp_tm < save_sys_info.bk_var.chem_4e_in_tm){pump4(1);}
	else if(pump_func.chem_4e_disp_tm >= save_sys_info.bk_var.chem_4e_in_tm)
	{
		pump4(0);
		pump_func.chem_4e_disp_tm = 0;
		sys_info.Stat_M.chem_4e_disp_stat = 0;
	}
}

void chem_5f_disp(void)
{
	if(pump_func.chem_5f_disp_tm < save_sys_info.bk_var.chem_5f_in_tm){pump5(1);}
	else if(pump_func.chem_5f_disp_tm >= save_sys_info.bk_var.chem_5f_in_tm)
	{
		pump5(0);
		pump_func.chem_5f_disp_tm = 0;
		sys_info.Stat_M.chem_5f_disp_stat = 0;
	}
}

void water_g_disp(void)
{
	if(pump_func.water_g_disp_tm < save_sys_info.bk_var.wtr_g_in_tm){pump6(1);}
	else if(pump_func.water_g_disp_tm >= save_sys_info.bk_var.wtr_g_in_tm)
	{
		pump6(0);
		pump_func.water_g_disp_tm = 0;
		sys_info.Stat_M.water_g_disp_stat = 0;
	}
}

void mot_tm_rst(void)
{
	pump_func.chem_1a_disp_tm = 0;
	pump_func.chem_1b_disp_tm = 0;
	pump_func.chem_2c_disp_tm = 0;
	pump_func.chem_3d_disp_tm = 0;
	pump_func.chem_4e_disp_tm = 0;
	pump_func.chem_5f_disp_tm = 0;
	pump_func.drain_wsh_tm = 0;
	pump_func.water_g_disp_tm = 0;
	pump_func.drn_only_tm = 0;
	sys_info.sys_f.ppfillf = 0;
}

void drn_only_func(void)
{
	if(pump_func.drn_only_tm < (sys_info.drain_mix_tm) )
	{
		motor1(1);
	}
	else if(pump_func.drn_only_tm < (sys_info.drain_mix_tm +
			sys_info.drain_tm) )
	{
		motor1(0);
		pump8(1);
	}//Sample Drain complete
	else if(pump_func.drn_only_tm >= (sys_info.drain_mix_tm +
			sys_info.drain_tm) )
	{
		pump8(0);
		motor1(0);
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 0;
		pump_func.drn_only_tm = 0;
		sys_info.Stat_L.drain_only = 0;
	}
}

void pump_handler(void)
{
	if(!sys_info.sys_f.ppfillf)
	{
		if(sys_info.Stat_M.chem_1a_disp_stat){chem_1a_disp();}
		if(sys_info.Stat_M.chem_1b_disp_stat){chem_1b_disp();}
		if(sys_info.Stat_M.chem_2c_disp_stat){chem_2c_disp();}
		if(sys_info.Stat_M.chem_3d_disp_stat){chem_3d_disp();}
		if(sys_info.Stat_M.chem_4e_disp_stat){chem_4e_disp();}
		if(sys_info.Stat_M.chem_5f_disp_stat){chem_5f_disp();}
		if(sys_info.Stat_M.water_g_disp_stat){water_g_disp();}//g quantity(a,b,c,d,e,f,g)

	}
	else if(sys_info.sys_f.ppfillf)
	{
		if(sys_info.Stat_M.chem_1a_disp_stat){pump1(1);}
		else if(!sys_info.Stat_M.chem_1a_disp_stat){pump1(0);}
		if(sys_info.Stat_M.chem_2c_disp_stat){pump2(1);}
		else if(!sys_info.Stat_M.chem_2c_disp_stat){pump2(0);}
		if(sys_info.Stat_M.chem_3d_disp_stat){pump3(1);}
		else if(!sys_info.Stat_M.chem_3d_disp_stat){pump3(0);}
		if(sys_info.Stat_M.chem_4e_disp_stat){pump4(1);}
		else if(!sys_info.Stat_M.chem_4e_disp_stat){pump4(0);}
		if(sys_info.Stat_M.chem_5f_disp_stat){pump5(1);}
		else if(!sys_info.Stat_M.chem_5f_disp_stat){pump5(0);}
		if(sys_info.Stat_M.water_g_disp_stat){pump6(1);}//g quantity(a,b,c,d,e,f,g)
		else if(!sys_info.Stat_M.water_g_disp_stat){pump6(0);}
	}
	if(sys_info.Stat_L.drain_wsh_stat){drn_wsh_func();}
	if(sys_info.Stat_L.drain_only){drn_only_func();}
}


/* USER CODE END 1 */

