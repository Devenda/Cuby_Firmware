#pragma once

#include "driver/i2c.h"   /*!< user specific field */

#ifdef __cplusplus
extern "C" {
#endif

/* config */
#define I2C_PORT I2C_NUM_0
#define PIN_SDA GPIO_NUM_5
#define PIN_SCL GPIO_NUM_17

/* functions */
void vl53l0x_Init();

uint16_t vl53l0x_getDistance();


#ifdef __cplusplus
}
#endif
