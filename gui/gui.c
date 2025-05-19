#include "gui.h"
#include <stdint.h>
#include <system.h>
#include <encoder.h>
#include <hd44780.h>
#include <relay.h>

#define GUI_REFRESH_PERIOD_MS 250

#define GUI_MILLISECONDS_PER_MINUTE 60000
#define GUI_MILLISECONDS_PER_SECOND 1000

#define GUI_DEFAULT_TIMER_TIME_MIN 5
#define GUI_DEFAULT_DELAY_TIME_S 10

#define GUI_MIN_TIMER_VALUE_MIN 1
#define GUI_MAX_TIMER_VALUE_MIN 99

#define GUI_MIN_DELAY_VALUE_S 1
#define GUI_MAX_DELAY_VALUE_S 99

typedef enum 
{
    GUI_SHOW_SET_TIME = 0,
    GUI_SET_TIME,
    GUI_SET_DELAY,
    GUI_DELAY,
    GUI_RUNNING
} gui_state_t;

static gui_state_t state;

static uint32_t last_task_tick;
static uint32_t last_count_tick;
static uint16_t last_encoder_count;

static uint8_t timer_time_min;
static uint8_t delay_time_s;
static uint8_t counter;

void gui_init(void)
{
    timer_time_min = GUI_DEFAULT_TIMER_TIME_MIN;
    delay_time_s = GUI_DEFAULT_DELAY_TIME_S;
}

void gui_task(void)
{
    const uint32_t current_tick = system_get_ticks();
    if ((current_tick - last_task_tick) < GUI_REFRESH_PERIOD_MS) {
        return;
    }

    const uint32_t encoder_count = encoder_get_count();

    switch (state) {
        case GUI_SHOW_SET_TIME:
            hd44780_gotoxy(0, 0);
            hd44780_write_string("Time: ");
            hd44780_write_number(timer_time_min);
            hd44780_write_string("min ");
            hd44780_gotoxy(1, 0);
            hd44780_write_string("STOPPED ");

            relay_enable(false);

            last_encoder_count = encoder_get_count();
            state = GUI_SET_TIME;

            break;

        case GUI_SET_TIME:
            if (!encoder_get_button_state()) {
                hd44780_gotoxy(0, 0);
                hd44780_write_string("Delay: ");
                hd44780_write_number(delay_time_s);
                hd44780_write_string("s ");

                state = GUI_SET_DELAY;

                break;
            }

            if (encoder_count != last_encoder_count) {
                timer_time_min += (encoder_count - last_encoder_count);
                if (timer_time_min < GUI_MIN_TIMER_VALUE_MIN) {
                    timer_time_min = GUI_MIN_TIMER_VALUE_MIN;
                }
                if (timer_time_min > GUI_MAX_TIMER_VALUE_MIN) {
                    timer_time_min = GUI_MAX_TIMER_VALUE_MIN;
                }

                hd44780_gotoxy(0, 6);
                hd44780_write_number(timer_time_min);
                hd44780_write_string("min ");

                last_encoder_count = encoder_count;
            }
            break;

        case GUI_SET_DELAY:
            if (!encoder_get_button_state()) {
                hd44780_gotoxy(1, 0);
                hd44780_write_string("STARTING");

                last_count_tick = system_get_ticks();
                counter = delay_time_s;
                state = GUI_DELAY;

                break;
            }

            if (encoder_count != last_encoder_count) {
                delay_time_s += (encoder_count - last_encoder_count);
                if (delay_time_s < GUI_MIN_DELAY_VALUE_S) {
                    delay_time_s = GUI_MIN_DELAY_VALUE_S;
                }
                if (delay_time_s > GUI_MAX_DELAY_VALUE_S) {
                    delay_time_s = GUI_MAX_DELAY_VALUE_S;
                }

                hd44780_gotoxy(0, 7);
                hd44780_write_number(delay_time_s);
                hd44780_write_string("s ");

                last_encoder_count = encoder_count;
            }
            break;

        case GUI_DELAY:
            if (!encoder_get_button_state()) {
                state = GUI_SHOW_SET_TIME;
                break;
            }

            if ((current_tick - last_count_tick) < GUI_MILLISECONDS_PER_SECOND) {
                break;
            }

            last_count_tick = current_tick;
            --counter; 
            if (counter == 0) {
                hd44780_gotoxy(0, 0);
                hd44780_write_string("Left: ");
                hd44780_write_number(timer_time_min);
                hd44780_write_string("min ");
                hd44780_gotoxy(1, 0);
                hd44780_write_string("RUNNING ");

                relay_enable(true);

                last_count_tick = system_get_ticks();
                counter = timer_time_min;
                state = GUI_RUNNING;

                break;
            }
            hd44780_gotoxy(0, 7);
            hd44780_write_number(counter);
            hd44780_write_string("s ");
        
            break;

        case GUI_RUNNING:
            if (!encoder_get_button_state()) {
                state = GUI_SHOW_SET_TIME;
                break;
            }

            if ((current_tick - last_count_tick) < GUI_MILLISECONDS_PER_MINUTE) {
                break;
            }

            last_count_tick = current_tick;
            --counter; 
            if (counter == 0) {
                state = GUI_SHOW_SET_TIME;
                break;
            }
            hd44780_gotoxy(0, 6);
            hd44780_write_number(counter);
            hd44780_write_string("min ");

            break;

        default:
            break;
    }

    last_task_tick = current_tick;
}
