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
    while (1)
    {
        /* measurement */
        uint16_t result_mm = 0;
        TickType_t tick_start = xTaskGetTickCount();
        bool res = v.read(&result_mm);
        TickType_t tick_end = xTaskGetTickCount();
        int took_ms = ((int)tick_end - tick_start) / portTICK_PERIOD_MS;
        if (res)
            ESP_LOGI(TAG, "Range: %d [mm] took %d [ms]", (int)result_mm, took_ms);
        else
            ESP_LOGE(TAG, "Failed to measure :(");

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}