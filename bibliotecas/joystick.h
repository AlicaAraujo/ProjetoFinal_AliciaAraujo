#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"
#include "buzzer.h"
#include "buttons.h"
#include "display.h"
#include "safe_mode.h"

// Definições dos pinos
#define JOYSTICK_X_PIN 27
#define JOYSTICK_Y_PIN 26
#define JOYSTICK_BUTTON_PIN 22

// Variáveis globais
extern uint16_t joystick_x_value;
extern uint16_t joystick_y_value;

// Protótipos das funções
void joystick_init();
void joystick_read();

#endif // JOYSTICK_H