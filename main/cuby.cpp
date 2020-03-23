#include <stdio.h>
#include <esp_log.h>
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

//Own headers
#include "sensors.h"
#include "oled.h"
#include "motors.h"
#include "wifi.h"
#include "ota.h"
#include "touch.h"

static const char *TAG = "main";

static xQueueHandle button_queue = NULL;

extern "C" void app_main(void)
{
    // Distance & OLED
    // vl53l0x_Init();
    // u8g2_t u8g2 = oled_init();
    // uint16_t dist = 0;

    // Motors
    motors_init_gpio();
    motors_sleep();

    // WiFi
    // wifi_init_sta();

    // OTA
    // ota_start_update(NULL);
    // xTaskCreate(&ota_start_update, "ota_start_update", 8192, NULL, 5, NULL);

    // Touch
    touch_pad_t touchedPad;
    std::vector<touch_pad_t> touchpads;
    touchpads.push_back(TOUCH_PAD_NUM8);
    touchpads.push_back(TOUCH_PAD_NUM9);

    // TODO: size 1: als beiden tegelijk ingedrukt blijven wordt er mss een gemist
    xQueueHandle xTouchPadQueue = xQueueCreate(1, sizeof(int));

    Touch touch(touchpads, xTouchPadQueue);

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

        // This needs to be here to reset watchdog
        // If no while is present the touch object is destroyed and ISR event cannot determine touched pad 
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// class cuby
// {
// private:
//     void init()
//     {
//         // button_queue = xQueueCreate(1,)
//     }

// public:
//     cuby()
//     {
//     }
// };

// cuby::cuby()
// {
// }