#include "display.h"
#include "ssd1306.h"
#include "font.h"

ssd1306_t display;

void display_init() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&display, 128, 64, false, OLED_ADDRESS, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
}

void display_message(const char *message) {
    ssd1306_fill(&display, false);
    ssd1306_draw_string(&display, message, 8, 10);
    ssd1306_send_data(&display);
}