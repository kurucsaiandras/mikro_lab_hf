#pragma once

#ifndef __BSP_ACC
#define __BSP_ACC

#ifdef __cplusplus
extern "C" {
#endif
#include "lsm6dsl_reg.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

/* Private macro -------------------------------------------------------------*/
#define TX_BUF_DIM          1000
#define    BOOT_TIME          15 //ms

static uint8_t whoamI, rst;
static volatile int16_t data_raw_angular_rate[3];
static volatile int16_t angular_rate_mdps[3];
static stmdev_ctx_t dev_ctx;
extern volatile float bank_angle; //oldaliranyu elfordulas
extern volatile float pitch_angle; //elore/hatra doles

void Mems_Init(SPI_HandleTypeDef* hspi2);
void read_angular_rate();
void Mems_getAngles();
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
static void platform_delay(uint32_t ms);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
