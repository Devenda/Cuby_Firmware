#include <stdio.h>
#include <math.h>

#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include <esp_log.h>

#include "motors.h"

static const char *TAG = "motors";

//Motor Left
#define MOTOR_LEFT 1
#define GPIO_M1_A1 GPIO_NUM_14
#define GPIO_M1_A2 GPIO_NUM_13

//Motor Right
#define MOTOR_RIGHT 2
#define GPIO_M2_A1 GPIO_NUM_4
#define GPIO_M2_A2 GPIO_NUM_2

#define GPIO_MSLP GPIO_NUM_15

static void motors_init_gpio()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, GPIO_M1_A2);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, GPIO_M1_A1);

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, GPIO_M2_A2);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, GPIO_M2_A1);

    gpio_pad_select_gpio(GPIO_MSLP);
    gpio_set_direction(GPIO_MSLP, GPIO_MODE_OUTPUT);
}

void motors_init()
{
    //1. mcpwm gpio initialization
    motors_init_gpio();

    //2. initialize mcpwm configuration
    printf("Configuring Initial Parameters of mcpwm...\n");

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 10000; //PWM Stepper: frequency => lowest = 9 highest = +-250
    pwm_config.cmpr_a = 0.0;      //duty cycle of PWMxA = 0%
    pwm_config.cmpr_b = 0.0;      //duty cycle of PWMxb = 0%
    pwm_config.counter_mode = MCPWM_UP_DOWN_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);

    mcpwm_sync_enable(MCPWM_UNIT_0, MCPWM_TIMER_1, (mcpwm_sync_signal_t)1, 0);
    mcpwm_sync_enable(MCPWM_UNIT_0, MCPWM_TIMER_2, (mcpwm_sync_signal_t)1, 0);

    //Set Left motor
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_1);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, MCPWM_DUTY_MODE_1);

    //Set Right motor
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_1);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B, MCPWM_DUTY_MODE_1);

    //Enable motors
    gpio_set_level(GPIO_MSLP, 1);

    //Don't move after init
    motors_set(270, 0);

    printf("Configuration Done \n");
}

void motors_sleep()
{
    //Disable motors
    gpio_set_level(GPIO_MSLP, 0);
}

void motors_wake()
{
    //Disable motors
    gpio_set_level(GPIO_MSLP, 1);
}

// Set duty of one output, the other must sink
void motors_set_reverse_duty(int motor, int duty)
{
    if (motor == MOTOR_LEFT)
    {
        mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, MCPWM_DUTY_MODE_1);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, duty);
        mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A);
    }
    if (motor == MOTOR_RIGHT)
    {
        mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B, MCPWM_DUTY_MODE_1);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B, duty);
        mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A);
    }
}

void motors_set_forward_duty(int motor, int duty)
{
    if (motor == MOTOR_LEFT)
    {
        mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_1);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, duty);
        mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B);
    }
    if (motor == MOTOR_RIGHT)
    {
        mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_1);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, duty);
        mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B);
    }
}

//Set motors, use angle (degrees) and speed (0 -> 100) to determine direction and duty
void motors_set(int angle, int speed)
{
    double sine = sin(angle * 3.1415/180);
    int duty_full = 100 - speed;
    int duty_sine = 100 - abs(sine * speed);

    ESP_LOGI(TAG, "Angle: %d, Sine: %f, Duty_Full: %d, Duty_Sine: %d", angle, sine, duty_full, duty_sine);

    if (angle >= 0 && angle < 90)
    {
        motors_set_forward_duty(MOTOR_LEFT, duty_full);
        motors_set_forward_duty(MOTOR_RIGHT, duty_sine);
    }
    else if (angle >= 90 && angle < 180)
    {
        motors_set_forward_duty(MOTOR_LEFT, duty_sine);
        motors_set_forward_duty(MOTOR_RIGHT, duty_full);
    }
    else if (angle >= 180 && angle < 270)
    {
        motors_set_reverse_duty(MOTOR_LEFT, duty_sine);
        motors_set_reverse_duty(MOTOR_RIGHT, duty_full);
    }
    else if (angle >= 270 && angle < 360)
    {
        motors_set_reverse_duty(MOTOR_LEFT, duty_full);
        motors_set_reverse_duty(MOTOR_RIGHT, duty_sine);
    }
    else
    {
        motors_set_forward_duty(MOTOR_LEFT, 100);
        motors_set_forward_duty(MOTOR_RIGHT, 100);
    }
}
