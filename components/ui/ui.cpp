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
    xQueueHandle xTouchPadQueue = xQueueCreate(1, sizeof(int));

    Touch touch(touchpads, xTouchPadQueue);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

UI::~UI()
{
    ESP_LOGI(TAG, "Destructor called");
}