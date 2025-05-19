#include "hd44780.h"
#include <mcs51/8051.h>
#include <system.h>
#include <stdbool.h>

#define HD44780_PORT P1
#define HD44780_RS P1_2
#define HD44780_E P1_3

#define HD44780_CLEAR_DISPLAY_CMD 0x01
#define HD44780_ENTRY_MODE_SET_CMD 0x04
#define HD44780_DISPLAY_ON_OFF_CMD 0x08
#define HD44780_FUNCTION_SET_CMD 0x20
#define HD44780_SET_CGRAM_ADDR_CMD 0x40
#define HD44780_SET_DDRAM_ADDR_CMD 0x80

#define HD44780_COLS_COUNT 16
#define HD44780_ROWS_COUNT 2

#define HD44780_FIRST_ROW_ADDR 0x00
#define HD44780_SECOND_ROW_ADDR 0x40

#define HD44780_TWO_LINES 0x08

#define HD44780_DISPLAY_SCROLL_ON 0x01
#define HD44780_INCREASE_CURSOR_ON 0x02

#define HD44780_CURSOR_BLINK_ON 0x01
#define HD44780_CURSOR_ON 0x02
#define HD44780_DISPLAY_ON 0x04

#define HD44780_MAX_DIGITS 5
#define HD44780_MAX_DIVISOR 10000

void hd44780_init(void)
{
    /* This is required to set display to a known mode */
    hd44780_write_cmd(0x03);
    system_delay_ms(5);
    hd44780_write_cmd(0x03);
    system_delay_ms(1);
    hd44780_write_cmd(0x03);
    system_delay_ms(1);
    hd44780_write_cmd(0x02);

    /* Initialize as 2 lines, 5x8 matrix, 4-bit interface */
    hd44780_write_cmd(HD44780_FUNCTION_SET_CMD | HD44780_TWO_LINES);

    /* Enable display and enable cursor position increasing */
    hd44780_write_cmd(HD44780_DISPLAY_ON_OFF_CMD | HD44780_DISPLAY_ON);
    hd44780_write_cmd(HD44780_ENTRY_MODE_SET_CMD | HD44780_INCREASE_CURSOR_ON);

    /* Clear display */
    hd44780_clear();
}

void hd44780_write_byte(uint8_t byte, HD44780_mode_t mode)
{
    /* Set RS line state */
    HD44780_RS = mode;

    /* Write upper nibble */
    HD44780_PORT &= 0x0F;
    HD44780_PORT |= byte & 0xF0;

    /* Pulse enable signal */
    HD44780_E = 1;
    HD44780_E = 0;

    /* Write lower nibble */
    HD44780_PORT &= 0x0F;
    HD44780_PORT |= (byte << 4) & 0xF0;

    /* Pulse enable signal */
    HD44780_E = 1;
    HD44780_E = 0;
}

void hd44780_write_cmd(uint8_t command)
{
    hd44780_write_byte(command, HD44780_INSTRUCTION);
}

void hd44780_write_char(char character)
{
    hd44780_write_byte(character, HD44780_CHARACTER);
}

void hd44780_clear(void)
{
    /* Clear display and set cursor to the beginning */
    hd44780_write_cmd(HD44780_CLEAR_DISPLAY_CMD);
    system_delay_ms(5);
    hd44780_write_cmd(HD44780_SET_DDRAM_ADDR_CMD | HD44780_FIRST_ROW_ADDR);
    system_delay_ms(1);
}

void hd44780_gotoxy(uint8_t x, uint8_t y)
{
    uint8_t address;

    /* Sanity checks */
    if (x >= HD44780_ROWS_COUNT) {
        x = HD44780_ROWS_COUNT - 1;
    }
    if (y >= HD44780_COLS_COUNT) {
        y = HD44780_COLS_COUNT - 1;
    }

    /* Set proper row */
    if (x == 0) {
        address = HD44780_FIRST_ROW_ADDR;
    }
    else {
        address = HD44780_SECOND_ROW_ADDR;
    }

    /* Set proper column */
    address += y;

    /* Write to display */
    hd44780_write_cmd(HD44780_SET_DDRAM_ADDR_CMD | address);
}

void hd44780_write_string(const char* str)
{
    while (*str != '\0') {
        hd44780_write_char(*str++);
    }
}

void hd44780_write_number(uint16_t number)
{
    uint16_t divisor = HD44780_MAX_DIVISOR;
    uint8_t digit;
    bool blank_zeros = true;

    for (uint8_t i = 0; i < HD44780_MAX_DIGITS; ++i) {
        digit = (number / divisor) % 10;
        divisor /= 10;

        if (digit != 0) {
            blank_zeros = false;
        }

        if (!blank_zeros || (i == (HD44780_MAX_DIGITS - 1))) {
            hd44780_write_char(digit + '0');
        }
    }
}
