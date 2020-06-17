#pragma once

void motors_init();

void motors_sleep();
void motors_wake();

void motors_set(int angle, int speed);