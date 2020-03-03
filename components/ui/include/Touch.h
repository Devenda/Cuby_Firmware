#pragma once

#include <stdio.h>
#include <driver/gpio.h>
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"

class Touch
{
private:
    std::vector<touch_pad_t> _pads;
    xQueueHandle _touchQueue;

    void tp_init();
    void tp_touch_pad_init();
    void tp_set_threshold();

    static void tp_rtc_intr(void *arg);

public:
    Touch(std::vector<touch_pad_t> pads, xQueueHandle touchQueue);
    ~Touch();

    bool tpTouched;

    void tp_touch_handler();
};