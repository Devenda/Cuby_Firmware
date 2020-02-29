/* Touch Pad Interrupt Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <driver/gpio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"

#include "touchpad.h"

static const char *TAG = "Touch pad";

#define TOUCH_THRESH_NO_USE (0)
#define TOUCH_THRESH_PERCENT (80)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

TouchPad ::TouchPad(touch_pad_t pad, xQueueHandle touchQueue)
{
    _pad = pad;
    _touchQueue = touchQueue;

    tp_init();
}

void TouchPad ::tp_init(void)
{
    // Initialize touch pad peripheral, it will start a timer to run a filter
    ESP_LOGI(TAG, "Initializing touch pad %d", (int)_pad);
    touch_pad_init();
    // If use interrupt trigger mode, should set touch sensor FSM mode at 'TOUCH_FSM_MODE_TIMER'.
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    // Set reference voltage for charging/discharging
    // For most usage scenarios, we recommend using the following combination:
    // the high reference valtage will be 2.7V - 1V = 1.7V, The low reference voltage will be 0.5V.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    //init RTC IO and mode for touch pad.
    touch_pad_config(_pad, TOUCH_THRESH_PERCENT);
    // Initialize and start a software filter to detect slight change of capacitance.
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    // Set threshold
    tp_set_threshold();
    // Register touch interrupt ISR

    ESP_LOGI(TAG, "register isr for pad %d", _pad);
    touch_pad_isr_register(tp_rtc_intr, this);
    ESP_LOGI(TAG, "register isr done");

    touch_pad_intr_enable();
}

/*
  Read values sensed at all available touch pads.
  Use 2 / 3 of read value as the threshold
  to trigger interrupt when the pad is touched.
  Note: this routine demonstrates a simple way
  to configure activation threshold for the touch pads.
  Do not touch any pads when this routine
  is running (on application start).
 */
void TouchPad::tp_set_threshold()
{
    uint16_t touch_value;

    //read filtered value
    touch_pad_read_filtered(_pad, &touch_value);
    ESP_LOGI(TAG, "test init: touch pad [%d] val is %d", _pad, touch_value);
    //set interrupt threshold.
    ESP_ERROR_CHECK(touch_pad_set_thresh(_pad, touch_value * 5 / 10));
}

/*
  Handle an interrupt triggered when a pad is touched.
  Recognize what pad has been touched and save it in a table.
 */
void TouchPad ::tp_rtc_intr(void *arg)
{
    TouchPad *tp = (TouchPad *)arg;
    uint32_t pad_intr = touch_pad_get_status();
    //clear interrupt
    touch_pad_clear_status();

    if ((pad_intr >> tp->_pad) & 0x01)
    {
        if (xQueueSendFromISR(tp->_touchQueue, &tp->_pad, NULL) == pdTRUE)
        {
            // ets_printf("Write successful\n");
        }
        else
        {
            // ets_printf("Write failed\n");
        }
    }
}

void TouchPad ::tp_touch_handler()
{
    touch_pad_t touchedPad;

    if (xQueueReceive(_touchQueue, &touchedPad, 0))
    {
        ESP_LOGI(TAG, "pad %d touched", touchedPad);
    }
}