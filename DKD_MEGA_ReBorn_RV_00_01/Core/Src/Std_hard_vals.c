/*
 * Std_hard_vals.c
 *
 *  Created on: 08-Sept-2026
 *      Author: DKD
 */

/* Includes ------------------------------------------------------------------*/
#include "Std_hard_vals.h"
#include "my_struct.h"

/* Private define ------------------------------------------------------------*/
#ifndef ARC_CAL
#define ARC_CAL
#endif

#ifndef TM_MULTIPLIER
#define TM_MULTIPLIER 20
#endif

/* USER CODE BEGIN 1 */

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
