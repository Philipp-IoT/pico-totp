/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "hd44780.pio.h"

#define HD44780_CLEAR_DISPLAY 0b00000001
#define HD44780_RETURN_HOME   0b00000010
#define HD44780_DISPLAY_ON    0b00001100
#define HD44780_DISPLAY_OFF   0b00001000
#define HD44780_FUNCTION_SET  0b00101000

// Write `period` to the input shift register
void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

// Write `level` to TX FIFO. State machine will copy this into X.
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
    pio_sm_put_blocking(pio, sm, level);
}

void hd44780_set_rs(PIO pio, uint sm, uint offset, uint rs) {
    // Wait for PIO state machine to reach stable state based on its PC
    while((!pio_sm_is_tx_fifo_empty(pio, sm)) || (pio_sm_get_pc(pio, sm) != offset + 0)) {
    }
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, rs);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_mov(pio_y, pio_osr));
    pio_sm_set_enabled(pio, sm, true);
    sleep_ms(1);
}

int main() {
    stdio_init_all();
#ifndef PICO_DEFAULT_LED_PIN
#warning pio/pwm example requires a board with a regular LED
    puts("Default LED pin was not defined");
#else

    // todo get free sm
    PIO pio = pio0;
    uint offset = 0;
    int sm = 0;

    offset = pio_add_program(pio, &hd44780_4bit_program);
    hd44780_4bit_init_program_init(pio, sm, offset, 2);

    const uint8_t init_sequence[] = {0b0011, 0b0010};

    sleep_ms(20);
    pio_sm_put_blocking(pio, sm, init_sequence[0]);
    sleep_ms(10);
    pio_sm_put_blocking(pio, sm, init_sequence[0]);
    sleep_ms(1);
    pio_sm_put_blocking(pio, sm, init_sequence[0]);
    sleep_ms(1);
    pio_sm_put_blocking(pio, sm, init_sequence[1]);
    sleep_ms(10);
    pio_sm_set_enabled(pio, sm, false);
    pio_remove_program_and_unclaim_sm(&hd44780_4bit_program, pio, sm, offset);
    sleep_ms(10);
    
    offset = pio_add_program(pio, &hd44780_program);
    hd44780_program_init(pio, sm, offset, 2);

    pio_sm_put_blocking(pio, sm, HD44780_FUNCTION_SET);
    sleep_ms(10);
    pio_sm_put_blocking(pio, sm, HD44780_DISPLAY_OFF);
    sleep_ms(10);
    pio_sm_put_blocking(pio, sm, HD44780_CLEAR_DISPLAY);
    sleep_ms(10);
    pio_sm_put_blocking(pio, sm, HD44780_RETURN_HOME);
    sleep_ms(10);

    // Set RS to HIGH
    hd44780_set_rs(pio, sm, offset, 1);

    const char HelloWorld[] = "Hello, World!";
    for (int i = 0; i < sizeof(HelloWorld) - 1; i++) {
        pio_sm_put_blocking(pio, sm, HelloWorld[i]);
        sleep_ms(1);
    }

    // Set RS to LOW again
    hd44780_set_rs(pio, sm, offset, 0);

    pio_sm_put_blocking(pio, sm, HD44780_DISPLAY_ON);
    sleep_ms(10);

    uint8_t i = 0;
    char buf[16];
    int len = 0;
    while(1)
    {
        i++;
        len = snprintf(buf, 16, "Counter: %d", i);

        // Clear display
        pio_sm_put_blocking(pio, sm, HD44780_CLEAR_DISPLAY);
        sleep_ms(1);

        // Move cursor to start
        pio_sm_put_blocking(pio, sm, HD44780_RETURN_HOME);
        sleep_ms(1);

        // Set RS to HIGH
        hd44780_set_rs(pio, sm, offset, 1);

        // Write to display
        for (int j = 0; j < len; j++) 
        {
            pio_sm_put_blocking(pio, sm, buf[j]);
            
        }

        //sleep_ms(1);
        // Set RS to LOW again
        hd44780_set_rs(pio, sm, offset, 0);

        sleep_ms(1000);
    }
#endif
}
