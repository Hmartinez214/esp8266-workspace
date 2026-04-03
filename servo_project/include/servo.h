/**
 * @author Holden Martinez
 * @file servo.h
 * @date 4/2/2026
 */

#pragma once
#include "driver/mcpwm_prelude.h"

class Servo 
{
public:
    Servo(int gpio_pin);
    void set_angle(int angle);

private:
    int gpio_pin;
    mcpwm_cmpr_handle_t comparator;
 };

 void servo_task(void *pvParameters);