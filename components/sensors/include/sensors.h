#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "driver/i2c.h"   /*!< user specific field */
#include "VL53L0X.h"

#ifdef __cplusplus
extern "C" {
#endif

/* config */
#define I2C_PORT I2C_NUM_0
#define PIN_SDA GPIO_NUM_5
#define PIN_SCL GPIO_NUM_17

/* functions */
VL53L0X vl53l0x_Init();

uint16_t vl53l0x_getDistance(VL53L0X v);


#ifdef __cplusplus
}
#endif

#endif  /* _SENSORS_H_ */
