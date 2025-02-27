#include "leds.h"
#include "joystick.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

uint16_t led_r_level = 0, led_g_level = 0, led_b_level = 0;

// Definição das variáveis
bool led_g_status = false;
bool led_b_status = false;

void leds_init() {
    // Configura os pinos como saídas PWM
    gpio_set_function(LED_R_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_G_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_B_PIN, GPIO_FUNC_PWM);

    // Obtém as fatias (slices) de PWM para cada pino
    uint slice_r = pwm_gpio_to_slice_num(LED_R_PIN);
    uint slice_g = pwm_gpio_to_slice_num(LED_G_PIN);
    uint slice_b = pwm_gpio_to_slice_num(LED_B_PIN);

    // Configura o PWM para cada fatia
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 255); // Define o intervalo do PWM (0 a 255)
    pwm_init(slice_r, &config, true);
    pwm_init(slice_g, &config, true);
    pwm_init(slice_b, &config, true);
}

void leds_update() {
    led_r_level = joystick_x_value / 16;      
    led_g_level = joystick_y_value / 16;     
    led_b_level = (joystick_x_value + joystick_y_value) / 32; 

    // Define o nível do PWM para cada LED
    pwm_set_gpio_level(LED_R_PIN, led_r_level);
    pwm_set_gpio_level(LED_G_PIN, led_g_level);
    pwm_set_gpio_level(LED_B_PIN, led_b_level);
}
