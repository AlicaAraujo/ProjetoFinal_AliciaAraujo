#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "pico/stdlib.h"

#define BUZZER_PIN 21

void buzzer_init();
void buzzer_play(uint32_t frequencia_h, uint32_t intervalo_us);
void buzzer_play_async(int freq, int duration_ms);
void buzzer_set_frequency(int freq);

#endif