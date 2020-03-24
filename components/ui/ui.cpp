#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Own headers
#include "ui.h"

static const char *TAG = "UI";

UI::UI(/* args */)
{
    ESP_LOGI(TAG, "Initializing touch");

    std::vector<touch_pad_t> touchpads;
    touchpads.push_back(TOUCH_PAD_NUM8);
    touchpads.push_back(TOUCH_PAD_NUM9);

    // TODO: size 1: als beiden tegelijk ingedrukt blijven wordt er mss een gemist
    _xTouchPadQueue = xQueueCreate(1, sizeof(int));

    Touch touch(touchpads, _xTouchPadQueue);
    touch_pad_t touchedPad;

    const gpio_num_t BLINK_GPIOA = (gpio_num_t)CONFIG_BLINK_GPIOA;
    const gpio_num_t BLINK_GPIOB = (gpio_num_t)CONFIG_BLINK_GPIOB;

    gpio_pad_select_gpio(BLINK_GPIOA);
    gpio_set_direction(BLINK_GPIOA, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(BLINK_GPIOB);
    gpio_set_direction(BLINK_GPIOB, GPIO_MODE_OUTPUT);

    while (1)
    {
        if (xQueueReceive(_xTouchPadQueue, &touchedPad, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "pad %d touched", touchedPad);
            switch (touchedPad)
            {
            case TOUCH_PAD_NUM8:
                gpio_set_level(BLINK_GPIOA, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(BLINK_GPIOA, 0);
                break;

            case TOUCH_PAD_NUM9:
                gpio_set_level(BLINK_GPIOB, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(BLINK_GPIOB, 0);
                break;

            default:
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

UI::~UI()
{
    ESP_LOGI(TAG, "Destructor called");
}