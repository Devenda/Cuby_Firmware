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
#include "touchpad.h"

static const char *TAG = "main";

static xQueueHandle button_queue = NULL;

extern "C" void app_main(void)
{
    // vl53l0x_Init();
    // u8g2_t u8g2 = oled_init();
    // uint16_t dist = 0;

    motors_init_gpio();
    motors_sleep();

    // wifi_init_sta();

    // ota_start_update(NULL);
    // xTaskCreate(&ota_start_update, "ota_start_update", 8192, NULL, 5, NULL);
    touch_pad_t touchedPad;

    xQueueHandle xTouchPadQueue = xQueueCreate(1, sizeof(int));

    TouchPad tp8(TOUCH_PAD_NUM8, xTouchPadQueue); // => This is not working, only TP9 is working
    TouchPad tp9(TOUCH_PAD_NUM9, xTouchPadQueue);

    // xTaskCreate(tp8.tp_touch_handler, "tp_touch_handler", 8192, NULL, 5, NULL);
    // xTaskCreate(tp9.tp_touch_handler, "tp_touch_handler", 8192, NULL, 5, NULL);

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

        if (xQueueReceive(xTouchPadQueue, &touchedPad, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "pad %d touched", touchedPad);
            vTaskDelay(pdMS_TO_TICKS(200));
        }
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
