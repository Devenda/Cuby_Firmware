#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"
#include "driver/i2c.h"

#include "VL53L0X.h"

#include "esp_log.h"
#define TAG "app_main"

#include "sensors.h"

VL53L0X v;

void vl53l0x_Init()
{
    VL53L0X vl(I2C_PORT);
    vl.i2cMasterInit(PIN_SDA, PIN_SCL);
    if (!vl.init())
    {
        ESP_LOGE(TAG, "Failed to initialize VL53L0X :(");
        vTaskDelay(portMAX_DELAY);
    }

    v = vl;
}

uint16_t vl53l0x_getDistance()
{
    /* measurement */
    uint16_t result_mm = 0;
    bool res = v.read(&result_mm);

    return (int)result_mm;
}