#pragma once

//Own headers
#include "touch.h"

class UI
{
private:
    xQueueHandle _xTouchPadQueue;

public:
    UI(/* args */);
    ~UI();

    void blinkLed();
};