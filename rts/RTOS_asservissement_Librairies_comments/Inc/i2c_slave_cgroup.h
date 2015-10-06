

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

#ifndef I2C_SLAVE_CGROUP_H
#define I2C_SLAVE_CGROUP_H

void i2c_cgroup_function(void);
void MX_I2C1_Init (void);
void read_i2c (uint8_t*);
void send_i2c (void);

#endif

