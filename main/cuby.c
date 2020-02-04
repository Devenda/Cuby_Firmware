#include <stdio.h>
#include <esp_log.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

//Own headers
#include "sensors.h"
#include "oled.h"
#include "motors.h"

static const char *TAG = "main";

void app_main(void)
{
    vl53l0x_Init();
    u8g2_t u8g2 = oled_init();
    uint16_t dist = 0;

    motors_init_gpio();
    motors_sleep();

    while (1)
    {

        dist = vl53l0x_getDistance();

        char sDist[16];
        sprintf(sDist, "%d", dist);

        ESP_LOGI(TAG, "Distance: %d", dist);
        // oled_drawString(u8g2, sDist);

        int val = sensors_bat_voltage();
        char sVal[16];
        sprintf(sVal, "%d", val);

        ESP_LOGI(TAG, "Bat: %d", val);
        oled_drawString(u8g2, sVal);

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
