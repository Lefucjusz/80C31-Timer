#pragma once

#include <stdint.h>

typedef enum 
{
	HD44780_INSTRUCTION = 0,
	HD44780_CHARACTER
} HD44780_mode_t;

void hd44780_init(void);

void hd44780_write_byte(uint8_t byte, HD44780_mode_t mode);
void hd44780_write_cmd(uint8_t command);
void hd44780_write_char(char character);

void hd44780_clear(void);

void hd44780_gotoxy(uint8_t x, uint8_t y);

void hd44780_write_string(const char* str);
void hd44780_write_number(uint16_t number);
