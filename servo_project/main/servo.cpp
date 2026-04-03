/**
 * @author Holden Martinez
 * @file Servo.cpp
 * @date 4/2/2026
 */
#include "Servo.h"
#include "driver/mcpwm_prelude.h"

#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000
#define SERVO_TIMEBASE_PERIOD        20000
#define SERVO_MIN_PULSEWIDTH         500
#define SERVO_MAX_PULSEWIDTH         2500

Servo::Servo(int gpio_pin)
{
    this->gpio_pin = gpio_pin;

    // creating timer
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {};
    timer_config.group_id      = 0;
    timer_config.clk_src       = MCPWM_TIMER_CLK_SRC_DEFAULT;
    timer_config.resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ;
    timer_config.count_mode    = MCPWM_TIMER_COUNT_MODE_UP;
    timer_config.period_ticks  = SERVO_TIMEBASE_PERIOD;
    mcpwm_new_timer(&timer_config, &timer);

    // creating operator
    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {};
    operator_config.group_id = 0;
    mcpwm_new_operator(&operator_config, &oper);
    mcpwm_operator_connect_timer(oper, timer);

    // comparator
    mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {};
    comparator_config.flags.update_cmp_on_tez = true;
    mcpwm_new_comparator(oper, &comparator_config, &comparator);
    this->comparator = comparator;

    // create generator
    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {};
    generator_config.gen_gpio_num = gpio_pin;
    mcpwm_new_generator(oper, &generator_config, &generator);

    // generator actions
    mcpwm_generator_set_action_on_timer_event(generator,
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
        MCPWM_TIMER_EVENT_EMPTY,
        MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(generator,
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
        comparator,
        MCPWM_GEN_ACTION_LOW));

    // enable + start timer
    mcpwm_timer_enable(timer);
    mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP);
}

void Servo::set_angle(int angle)
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    uint32_t pulse_width = SERVO_MIN_PULSEWIDTH + (angle * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / 180);
    mcpwm_comparator_set_compare_value(comparator, pulse_width);
}