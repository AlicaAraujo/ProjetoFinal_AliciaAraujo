#ifndef SAFE_MODE_H
#define SAFE_MODE_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "buzzer.h"
#include "leds.h"
#include "buttons.h"
#include "display.h"
#include <stdlib.h>
#include "hardware/pwm.h"

// Variáveis globais
extern bool safe_mode_active;
extern int direcao_segura;

// Protótipos das funções
void safe_mode_toggle();
void safe_mode_guide();
void desativar_todos_os_modos();

#endif // SAFE_MODE_H