#include <stdio.h>
#include <esp_log.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

//Own headers
#include "sensors.h"
#include "oled.h"
#include "motors.h"
#include "wifi.h"
#include "ota.h"

static const char *TAG = "main";

void app_main(void)
{
    vl53l0x_Init();
    u8g2_t u8g2 = oled_init();
    uint16_t dist = 0;

    motors_init_gpio();
    motors_sleep();

    wifi_init_sta();

    ota_start_update(NULL);
    // xTaskCreate(&ota_start_update, "ota_start_update", 8192, NULL, 5, NULL);


    while (1)
    {
        //test distance
        // dist = vl53l0x_getDistance();

        // char sDist[16];
        // sprintf(sDist, "%d", dist);

        // ESP_LOGI(TAG, "Distance: %d", dist);
        // oled_drawString(u8g2, sDist);

        //test bat voltage
        // int val = sensors_bat_voltage();
        // char sVal[16];
        // sprintf(sVal, "%d", val);

        // ESP_LOGI(TAG, "Bat: %d", val);
        // oled_drawString(u8g2, sDist);

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
