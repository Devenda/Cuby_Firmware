#include <stdio.h>
#include <esp_log.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "sensors.h"
#include "oled.h"

static const char *TAG = "main";

void app_main(void)
{
    vl53l0x_Init();
    u8g2_t u8g2 = oled_init();
    uint16_t dist = 0;
    while (1)
    {

        dist = vl53l0x_getDistance();

        char sDist[16];
        // dist = dist + 1;
        sprintf(sDist, "%d", dist);

        // ESP_LOGI(TAG, "Distance: %d", dist);
        oled_drawString(u8g2, sDist);

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
