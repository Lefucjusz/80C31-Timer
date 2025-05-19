#pragma once

#include <stdint.h>
#include <stdbool.h>

void encoder_init(void);
void encoder_reset(void);

uint16_t encoder_get_count(void);
bool encoder_get_button_state(void);

void encoder_isr(void) __interrupt(0);