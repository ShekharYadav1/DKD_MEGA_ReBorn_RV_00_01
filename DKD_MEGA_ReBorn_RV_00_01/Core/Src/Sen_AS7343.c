/*
 * Sen_TCS34725.C
 *
 *  Created on: Mar 23, 2026
 *      Author: Yash.Choudhary
 */

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "Sen_AS7343.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/*
typedef enum Sensor_stage
{
	SEN_OK = 0,
	SEN_ERROR = 1,
	SEN_UNKNWN = 2,

}enum_Sen_stg;

typedef enum I2C_stage
{
	I2C_IDEAL = 0,
	I2C_READ = 1,
	I2C_WRITE = 2,
}enum_i2c_stg;

typedef enum Sen_Command
{
	SEN_DO_N = 0,
	SEN_MEM_READ = 1,
	SEN_MEM_WRITE = 2,
}enum_sen_comm;

typedef enum Sensor_running_mode
{
	SEN_IDEAL = 0,
	SEN_DATA_READ = 1,
}enum_sen_mode;

static const uint8_t sen_ind_add[MAX_SEN_DATA] = {TCS34725_ENABLE, TCS34725_ATIME, TCS34725_WTIME, TCS34725_AILTL, TCS34725_AILTH, TCS34725_AIHTL, TCS34725_AIHTH, TCS34725_PERS, TCS34725_CONFIG, TCS34725_CONTROL, TCS34725_ID, TCS34725_STATUS, TCS34725_CDATAL, TCS34725_CDATAH, TCS34725_RDATAL, TCS34725_RDATAH, TCS34725_GDATAL, TCS34725_GDATAH, TCS34725_BDATAL, TCS34725_BDATAH};

typedef struct Sensor_variables
{
	uint16_t rd_dly_cnt;//after how much time sensor value will be taken
	enum_Sen_stg sen_stat;//sensor is running ok or not
	enum_sen_mode sen_mode;//in which mode sensor is running
	uint8_t rd_wrt_flg;
	I2C_HandleTypeDef* sen_i2cHandle;//sensor i2c handler.
	uint8_t sen_read_Data[MAX_SEN_DATA];//This array contains current data of all the resister in the sensor
	uint8_t sen_write_Data[MAX_SEN_CNTRL_DATA];//contains current running mode of the sensor if sensor resistor differs from these then it will write these data to sensor.
	uint8_t sen_indx; //this the index of resister on which operation will be performed.
	enum_sen_comm sen_comm;
	enum_i2c_stg I2C_stg;
	uint16_t err_dly_cnt;//if nothing is happening for some time then it will reset the sensor.
	uint16_t cur_red_val, cur_green_val, cur_blue_val, cur_clear_val;
	uint16_t mavg_cur_red_val, mavg_cur_green_val, mavg_cur_blue_val, mavg_cur_clear_val;
	//uint16_t data_cnt,curr_max_val,curr_min_val,avg_max_val,avg_min_val;
}sen_var_typdef;
*/
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
//static
//sen_var_typdef sen_var;
/* USER CODE END EV */

/* *****************FUNCTIONS******************/
/* USER CODE BEGIN 1 */

/*
void TCS3472X_init(I2C_HandleTypeDef* i2cHandle)
{
	sen_var.rd_dly_cnt = 0;
	sen_var.sen_stat = SEN_UNKNWN;
	sen_var.sen_mode = SEN_IDEAL;
	sen_var.sen_comm = SEN_DO_N;
	sen_var.rd_wrt_flg = 0;
	sen_var.sen_i2cHandle = i2cHandle;
	for(uint8_t i = 0; i < MAX_SEN_DATA; i++){sen_var.sen_read_Data[i] = 0;}
	for(uint8_t i = 0; i < MAX_SEN_CNTRL_DATA; i++){sen_var.sen_write_Data[i] = 0;}
	sen_var.sen_indx = 0;
	sen_var.I2C_stg = I2C_IDEAL;
	sen_var.err_dly_cnt = 0;
	sen_var.cur_red_val = 0; sen_var.cur_green_val = 0; sen_var.cur_blue_val = 0; sen_var.cur_clear_val = 0;
	sen_var.mavg_cur_red_val = 0; sen_var.mavg_cur_green_val = 0; sen_var.mavg_cur_blue_val = 0; sen_var.mavg_cur_clear_val = 0;
	//sen_var.data_cnt = 0;sen_var.curr_max_val = 0;sen_var.curr_min_val = 0;sen_var.avg_max_val = 0;sen_var.avg_min_val = 0;
}

void TCS3472X_reset(void)
{
	sen_var.rd_dly_cnt = 0;
	sen_var.sen_stat = SEN_UNKNWN;
	sen_var.sen_mode = SEN_DATA_READ;
	sen_var.sen_comm = SEN_DO_N;
	sen_var.rd_wrt_flg = 0;
	for(uint8_t i = 0; i < MAX_SEN_DATA; i++){sen_var.sen_read_Data[i] = 0;}
	for(uint8_t i = 0; i < MAX_SEN_CNTRL_DATA; i++){sen_var.sen_write_Data[i] = 0;}
	sen_var.sen_indx = 0;
	sen_var.I2C_stg = I2C_IDEAL;
	sen_var.err_dly_cnt = 0;
	sen_var.cur_red_val = 0; sen_var.cur_green_val = 0; sen_var.cur_blue_val = 0; sen_var.cur_clear_val = 0;
	//sen_var.mavg_cur_red_val = 0; sen_var.mavg_cur_green_val = 0; sen_var.mavg_cur_blue_val = 0; sen_var.mavg_cur_clear_val = 0;
}

void sensor_timer_1msec(void)
{
	if(sen_var.rd_dly_cnt < RD_DELAY_CNT){sen_var.rd_dly_cnt++; sen_var.err_dly_cnt = 0;}
	else//if(sen_var.rd_dly_cnt >= RD_DELAY_CNT)
	{
		sen_var.err_dly_cnt++;
		if(sen_var.err_dly_cnt >= RD_DELAY_CNT){TCS3472X_reset();}
	}
}


void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  // Prevent unused argument(s) compilation warning
  //UNUSED(hi2c);
	if (hi2c->Instance == I2C1)
	{
		sen_var.sen_indx += 1;
		if(sen_var.sen_indx >= MAX_SEN_CNTRL_DATA){sen_var.sen_indx = 0; sen_var.sen_comm = SEN_DO_N; sen_var.rd_wrt_flg = 0;}
		sen_var.I2C_stg = I2C_IDEAL;
	}
  // NOTE : This function should not be modified, when the callback is needed,
  //          the HAL_I2C_MemTxCpltCallback could be implemented in the user file
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  // Prevent unused argument(s) compilation warning
  //UNUSED(hi2c);
  if (hi2c->Instance == I2C1)
  {
	sen_var.sen_indx += 1;
	if(sen_var.sen_indx >= MAX_SEN_DATA)
	{
		sen_var.mavg_cur_clear_val = 	(uint16_t)( ( (float)sen_var.mavg_cur_clear_val * 0.70) + ( (float)( sen_var.sen_read_Data[13] << 8 | sen_var.sen_read_Data[12] ) * 0.30) );
		sen_var.mavg_cur_red_val = 		(uint16_t)( ( (float)sen_var.mavg_cur_red_val   * 0.70) + ( (float)( sen_var.sen_read_Data[15] << 8 | sen_var.sen_read_Data[14] ) * 0.30) );
		sen_var.mavg_cur_green_val = 	(uint16_t)( ( (float)sen_var.mavg_cur_green_val * 0.70) + ( (float)( sen_var.sen_read_Data[17] << 8 | sen_var.sen_read_Data[16] ) * 0.30) );
		sen_var.mavg_cur_blue_val = 	(uint16_t)( ( (float)sen_var.mavg_cur_blue_val  * 0.70) + ( (float)( sen_var.sen_read_Data[19] << 8 | sen_var.sen_read_Data[18] ) * 0.30) );
		sen_var.sen_indx = 0; sen_var.rd_dly_cnt = 0; sen_var.sen_comm = SEN_DO_N; sen_var.rd_wrt_flg = 1;
	}
	sen_var.I2C_stg = I2C_IDEAL;
  }
  // NOTE : This function should not be modified, when the callback is needed,
  //          the HAL_I2C_MemRxCpltCallback could be implemented in the user file
}

void memory_read(void)
{
		HAL_I2C_Mem_Read_IT(sen_var.sen_i2cHandle,(uint16_t)(TCS34725_ADD<<1), 0x80 | sen_ind_add[sen_var.sen_indx], I2C_MEMADD_SIZE_8BIT, &sen_var.sen_read_Data[sen_var.sen_indx], 1);
		sen_var.I2C_stg = I2C_READ;
		sen_var.sen_comm = SEN_MEM_READ;
}

void set_mode(enum_sen_mode sen_mode)
{
	sen_var.sen_mode = sen_mode;
	if(sen_var.sen_mode == SEN_DATA_READ)
	{
		sen_var.sen_write_Data[0] = 0x03;//ENABLE
		sen_var.sen_write_Data[1] = 0x00;//ATIME
		sen_var.sen_write_Data[2] = 0xFF;//WTIME
		sen_var.sen_write_Data[3] = 0x00;//AILTL
		sen_var.sen_write_Data[4] = 0x00;//AILTH
		sen_var.sen_write_Data[5] = 0xFF;//AIHTL
		sen_var.sen_write_Data[6] = 0xFF;//AIHTH
		sen_var.sen_write_Data[7] = 0x0F;//PERS
		sen_var.sen_write_Data[8] = 0x00;//CONFIG
		sen_var.sen_write_Data[9] = 0x00;//CONTROL
	}

	//sen_var.sen_comm = SEN_MEM_WRITE;
}

enum_Sen_stg check_mode(void)
{
	sen_var.cur_clear_val = ( sen_var.sen_read_Data[13] << 8 | sen_var.sen_read_Data[12] );
	sen_var.cur_red_val = ( sen_var.sen_read_Data[15] << 8 | sen_var.sen_read_Data[14] );
	sen_var.cur_green_val = ( sen_var.sen_read_Data[17] << 8 | sen_var.sen_read_Data[16] );
	sen_var.cur_blue_val = ( sen_var.sen_read_Data[19] << 8 | sen_var.sen_read_Data[18] );

	if(sen_var.sen_mode == SEN_DATA_READ)
	{
		if(sen_var.sen_read_Data[0] != sen_var.sen_write_Data[0]){return SEN_ERROR;}//ENABLE
		if(sen_var.sen_read_Data[1] != sen_var.sen_write_Data[1]){return SEN_ERROR;}//ATIME
		if(sen_var.sen_read_Data[2] != sen_var.sen_write_Data[2]){return SEN_ERROR;}//WTIME
		if(sen_var.sen_read_Data[3] != sen_var.sen_write_Data[3]){return SEN_ERROR;}//AILTL
		if(sen_var.sen_read_Data[4] != sen_var.sen_write_Data[4]){return SEN_ERROR;}//AILTH
		if(sen_var.sen_read_Data[5] != sen_var.sen_write_Data[5]){return SEN_ERROR;}//AIHTL
		if(sen_var.sen_read_Data[6] != sen_var.sen_write_Data[6]){return SEN_ERROR;}//AIHTH
		if(sen_var.sen_read_Data[7] != sen_var.sen_write_Data[7]){return SEN_ERROR;}//PERS
		if(sen_var.sen_read_Data[8] != sen_var.sen_write_Data[8]){return SEN_ERROR;}//CONFIG
		if(sen_var.sen_read_Data[9] != sen_var.sen_write_Data[9]){return SEN_ERROR;}//CONTROL
		if( (sen_var.sen_read_Data[10] != 0x44) && (sen_var.sen_read_Data[10] != 0x4D) ){return SEN_ERROR;}
	}
	return SEN_OK;
}

void Chk_n_set_mode(void)
{
	//SET MODE
	if(sen_var.sen_stat == SEN_UNKNWN)
	{
		set_mode(SEN_DATA_READ);
	}
	else if(sen_var.sen_mode == SEN_DATA_READ)
	{
		set_mode(SEN_DATA_READ);
	}

	//Check mode
	sen_var.sen_stat = check_mode();
}

void TCS3472X_handler(void)
{
	if( ( ( (sen_var.rd_dly_cnt >= RD_DELAY_CNT) && (sen_var.sen_indx == 0) ) || ( sen_var.sen_comm == SEN_MEM_READ ) ) && (sen_var.I2C_stg == I2C_IDEAL) && (sen_var.rd_wrt_flg == 0) ){memory_read();}

	Chk_n_set_mode();

	if( ( ( (sen_var.sen_stat == SEN_ERROR) && (sen_var.sen_indx == 0) ) || ( sen_var.sen_comm == SEN_MEM_WRITE ) ) && (sen_var.I2C_stg == I2C_IDEAL )  && (sen_var.rd_wrt_flg == 1) )
	{
		HAL_I2C_Mem_Write_IT(sen_var.sen_i2cHandle, (uint16_t)(TCS34725_ADD<<1),  0x80 | sen_ind_add[sen_var.sen_indx], I2C_MEMADD_SIZE_8BIT, &sen_var.sen_write_Data[sen_var.sen_indx], 1);
		sen_var.I2C_stg = I2C_WRITE;
		sen_var.sen_comm = SEN_MEM_WRITE;
	}
	else if( ( ( (sen_var.sen_stat == SEN_OK) && (sen_var.sen_indx == 0) ) || ( sen_var.sen_comm == SEN_MEM_WRITE ) ) && (sen_var.I2C_stg == I2C_IDEAL )  && (sen_var.rd_wrt_flg == 1) )
	{
		sen_var.rd_wrt_flg = 0;
	}
}

int get_sen_stat(void)
{
	if(sen_var.sen_stat == SEN_OK){ return 1;}
	return 0;
}

void get_rgbc_val(uint16_t* Red_var, uint16_t* Green_var, uint16_t* Blue_var, uint16_t* Clear_var)
{
	*Red_var = sen_var.cur_red_val;
	*Green_var = sen_var.cur_green_val;
	*Blue_var = sen_var.cur_blue_val;
	*Clear_var = sen_var.cur_clear_val;
}

void get_mavg_rgbc_val(uint16_t* Red_var, uint16_t* Green_var, uint16_t* Blue_var, uint16_t* Clear_var)
{
	*Red_var = sen_var.mavg_cur_red_val;
	*Green_var = sen_var.mavg_cur_green_val;
	*Blue_var = sen_var.mavg_cur_blue_val;
	*Clear_var = sen_var.mavg_cur_clear_val;
}
*/

/* USER CODE END 1 */
