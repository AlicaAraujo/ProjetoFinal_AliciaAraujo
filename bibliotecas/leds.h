#ifndef LEDS_H
#define LEDS_H

#include <stdio.h>
#include "pico/stdlib.h"

#define LED_R_PIN 13
#define LED_G_PIN 11
#define LED_B_PIN 12

extern uint16_t led_r_level, led_g_level, led_b_level;
extern bool led_verde_status, led_azul_status;

void leds_init();
void leds_update();

#endif