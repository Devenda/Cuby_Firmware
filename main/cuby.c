#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "sensors.h"

void app_main(void)
{
    vl53l0x_Init();
    vl53l0x_getDistance();
}
