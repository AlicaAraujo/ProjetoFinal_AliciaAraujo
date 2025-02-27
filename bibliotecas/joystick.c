#include "joystick.h"
#include "safe_mode.h"  // Inclua o cabeçalho do modo seguro

uint16_t joystick_x_value = 0;
uint16_t joystick_y_value = 0;

// Define o intervalo de debounce como uma constante em tempo de compilação
#define DEBOUNCE_INTERVAL_MS 200

void joystick_init() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);

    // Inicializa o tempo do último pressionamento com o tempo atual
    last_joystick_press_time = get_absolute_time();
}

void joystick_read() {
    adc_select_input(0);
    joystick_x_value = adc_read();
    adc_select_input(1);
    joystick_y_value = adc_read();
}