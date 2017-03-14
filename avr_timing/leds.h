//
// Created by robert on 3/14/17.
//

#ifndef AVR_TIMING_LEDS_H
#define AVR_TIMING_LEDS_H

#include <stdint.h>

void init_leds();

void led_tx();
void led_rx();

void led_console(uint8_t console);

#endif //AVR_TIMING_LEDS_H
