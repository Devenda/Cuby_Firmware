#include <stdio.h>
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define GPIO_M1_A1 GPIO_NUM_14 
#define GPIO_M1_A2 GPIO_NUM_13 

#define GPIO_M2_A1 GPIO_NUM_4 
#define GPIO_M2_A2 GPIO_NUM_2 

#define GPIO_MSLP GPIO_NUM_15 

void motors_init_gpio()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, GPIO_M1_A2);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, GPIO_M1_A1);

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, GPIO_M2_A2);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, GPIO_M2_A1);

    gpio_pad_select_gpio(GPIO_MSLP);
    gpio_set_direction(GPIO_MSLP, GPIO_MODE_OUTPUT);
}

static void mcpwm_example_config(void *arg)
{
    //1. mcpwm gpio initialization
    motors_init_gpio();

    //2. initialize mcpwm configuration
    printf("Configuring Initial Parameters of mcpwm...\n");

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 10000;    //frequency => lowest = 9 highest = +-250
    pwm_config.cmpr_a = 0.0;    //duty cycle of PWMxA = 0%
    pwm_config.cmpr_b = 0.0;    //duty cycle of PWMxb = 0%
    pwm_config.counter_mode = MCPWM_UP_DOWN_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);
    
    printf("Configuration Done \n");
    mcpwm_sync_enable(MCPWM_UNIT_0, MCPWM_TIMER_1, (mcpwm_sync_signal_t)1, 0);
    mcpwm_sync_enable(MCPWM_UNIT_0, MCPWM_TIMER_2, (mcpwm_sync_signal_t)1, 0);

    //Duty mode zal niet belangrijk zijn, er is maar één signaal het andere moet steeds laag zijn
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_1); //Set PWM0A to duty mode one
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1,  MCPWM_OPR_A, 0); //kleiner getal is rapper, bij 50 zijn er soms startup problemen
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_1); //Set PWM0A to duty mode one
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2,  MCPWM_OPR_A, 0); //kleiner getal is rapper, bij 50 zijn er soms startup problemen

    //Andere signaal laag zetten
    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B);
    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B);

    //Enable motors
    gpio_set_level(GPIO_MSLP, 1);

    vTaskDelete(NULL);
}

void motors_sleep(){
    //Disable motors
    gpio_set_level(GPIO_MSLP, 0);
}
