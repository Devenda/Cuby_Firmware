#pragma once

#include <stdio.h>
#include <driver/gpio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"

class TouchPad
{
private:
    touch_pad_t _pad;
    xQueueHandle _touchQueue;

    void tp_set_threshold();
    static void tp_rtc_intr(void *arg);

    void tp_init(void);

public:
    TouchPad(touch_pad_t pad, xQueueHandle touchQueue);
    ~TouchPad();

    bool tpTouched;

    void tp_touch_handler();
};