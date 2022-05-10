#include "bsp_accelerometer.h"

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */

void Mems_Init(SPI_HandleTypeDef* hspi2)
{
	  /* Initialize mems driver interface */
	  dev_ctx.write_reg = platform_write;
	  dev_ctx.read_reg = platform_read;
	  dev_ctx.handle = hspi2;
	  /* Wait sensor boot time */
	  platform_delay(BOOT_TIME);
	  /* Check device ID */

	  lsm6dsl_reset_set(&dev_ctx, PROPERTY_ENABLE);

	  do{
		  lsm6dsl_reset_get(&dev_ctx, &rst);
	  } while(rst);

	  whoamI = 0;
	  lsm6dsl_device_id_get(&dev_ctx, &whoamI);

	  if ( whoamI != LSM6DSL_ID )
	    while (1); /*manage here device not found */

	  //power mode
	  lsm6dsl_xl_power_mode_set(&dev_ctx, LSM6DSL_XL_HIGH_PERFORMANCE);
	  lsm6dsl_gy_power_mode_set(&dev_ctx, LSM6DSL_GY_HIGH_PERFORMANCE);

	  //setting up the device
	  lsm6dsl_xl_data_rate_set(&dev_ctx, LSM6DSL_XL_ODR_12Hz5);
	  lsm6dsl_gy_data_rate_set(&dev_ctx, LSM6DSL_GY_ODR_12Hz5);
	  lsm6dsl_xl_full_scale_set(&dev_ctx, LSM6DSL_4g);
	  lsm6dsl_gy_full_scale_set(&dev_ctx, LSM6DSL_2000dps);
}

void read_angular_rate()
{
	uint8_t reg;
	lsm6dsl_gy_flag_data_ready_get(&dev_ctx, &reg);

	if(reg){
		memset(data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
		lsm6dsl_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate);
		angular_rate_mdps[0] = lsm6dsl_from_fs2000dps_to_mdps(data_raw_angular_rate[0])+280; //convert data + offset
		angular_rate_mdps[1] = lsm6dsl_from_fs2000dps_to_mdps(data_raw_angular_rate[1])+1680;
		angular_rate_mdps[2] = lsm6dsl_from_fs2000dps_to_mdps(data_raw_angular_rate[2])+280;
	}
}

void Mems_getAngles()
{
	read_angular_rate();
	float diff_in_rad = angular_rate_mdps[1] / 10000000.0 / 180.0 * M_PI;
	pitch_angle += diff_in_rad;
	diff_in_rad = angular_rate_mdps[2] / 10000000.0 / 180.0 * M_PI;
	bank_angle -= diff_in_rad;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	if(htim->Instance == TIM3){ //0.1 msec
		Mems_getAngles();
	}
}


static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_SPI_Transmit(handle, &reg, 1, 1000);
  HAL_SPI_Transmit(handle, (uint8_t*) bufp, len, 1000);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
  return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
  reg |= 0x80;
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_SPI_Transmit(handle, &reg, 1, 1000);
  HAL_SPI_Receive(handle, bufp, len, 1000);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
  return 0;
}

static void platform_delay(uint32_t ms)
{
  HAL_Delay(ms);
}
