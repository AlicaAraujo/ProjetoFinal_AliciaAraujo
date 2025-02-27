#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "leds.h"
#include "buzzer.h"
#include "joystick.h"
#include "safe_mode.h"
#include "display.h"
#include "pico/time.h"

// Definições dos pinos
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define JOYSTICK_BUTTON_PIN 22

// Variáveis globais
extern volatile bool button_a_pressed;
extern volatile bool button_b_pressed;
extern absolute_time_t last_joystick_press_time;

extern volatile bool joystick_button_pressed;

// Protótipos das funções
void buttons_init();
void button_callback(uint gpio, uint32_t events);
void processar_botoes();

#endif // BUTTONS_H