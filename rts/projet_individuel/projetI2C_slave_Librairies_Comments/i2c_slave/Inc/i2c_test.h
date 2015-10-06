

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

#ifndef I2C_SLAVE_CGROUP_H
#define I2C_SLAVE_CGROUP_H

void MX_I2C1_Init (void);
void read_i2c (void);
void send_i2c (void);

#endif

