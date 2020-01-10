idf_component_register(SRCS "oled.c"
                            "u8g2_esp32_hal.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES u8g2)
                