#pragma once
#include <u8g2.h>

u8g2_t oled_init();

void oled_drawString(u8g2_t u8g2, char *string);