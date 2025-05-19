#pragma once

#include <stdint.h>

void system_init(void);

uint32_t system_get_ticks(void);
void system_delay_ms(uint16_t ms);

void system_timer_isr(void) __interrupt(1);
