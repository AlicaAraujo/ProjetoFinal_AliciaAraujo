#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define I2C_SDA 14         // Pino SDA do I2C
#define I2C_SCL 15         // Pino SCL do I2C
#define OLED_ADDRESS 0x3C  // Endereço I2C do display OLED
#define I2C_PORT i2c1      // Porta I2C utilizada

void display_init();
void display_message(const char *message);

#endif