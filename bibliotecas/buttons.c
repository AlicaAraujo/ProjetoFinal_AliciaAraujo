#include "buttons.h"
#include "leds.h"
#include "joystick.h"
#include "buzzer.h"
#include "display.h"
#include "safe_mode.h"

volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
volatile bool joystick_button_pressed = false;

absolute_time_t last_button_a_press_time;
absolute_time_t last_button_b_press_time;
absolute_time_t last_joystick_press_time;

bool aprendizado_ativo = false;
bool navegacao_ativa = false;
int direcao_aprendida = -1;

#define DEBOUNCE_INTERVAL_MS 200

void desativar_todos_os_modos() {
    aprendizado_ativo = false;
    navegacao_ativa = false;
    direcao_aprendida = -1;
    display_message("Modo Inicial");
    pwm_set_gpio_level(LED_R_PIN, 255);
    pwm_set_gpio_level(LED_G_PIN, 255);
    pwm_set_gpio_level(LED_B_PIN, 255);
}

void ativar_modo_aprendizado() {
    desativar_todos_os_modos();
    aprendizado_ativo = true;
    display_message("Modo: Aprendizado");
    pwm_set_gpio_level(LED_R_PIN, 255);
    pwm_set_gpio_level(LED_G_PIN, 0);
    pwm_set_gpio_level(LED_B_PIN, 0);
}

void ativar_modo_navegacao() {
    desativar_todos_os_modos();
    navegacao_ativa = true;
    display_message("Modo: Navegacao");
    pwm_set_gpio_level(LED_R_PIN, 0);
    pwm_set_gpio_level(LED_G_PIN, 0);
    pwm_set_gpio_level(LED_B_PIN, 255);
}

void processar_botoes() {
    if (button_a_pressed) {
        button_a_pressed = false;

        // Se estiver no aprendizado e já tiver salvo a direção, sai do modo aprendizado
        if (aprendizado_ativo && direcao_aprendida != -1) {
            aprendizado_ativo = false;
            display_message("Modo Inicial");
            return;
        }

        // Alterna entre ativar e desativar o aprendizado
        if (!aprendizado_ativo && !navegacao_ativa) {
            ativar_modo_aprendizado();
        } else {
            desativar_todos_os_modos();
        }
    }

    if (button_b_pressed) {
        button_b_pressed = false;
        
        if (navegacao_ativa) {
            desativar_todos_os_modos();
        } else {
            ativar_modo_navegacao();
        }
    }

    if (joystick_button_pressed) {
        joystick_button_pressed = false;
        if (!aprendizado_ativo && !navegacao_ativa) {
            safe_mode_toggle();
        }
    }

    // ================== MODO APRENDIZADO ==================
    if (aprendizado_ativo && direcao_aprendida == -1) {  // Só executa se ainda não aprendeu
        uint16_t x = joystick_x_value;
        uint16_t y = joystick_y_value;

        if (x < 1024) {
            direcao_aprendida = 1;  // Esquerda
        } else if (x > 3072) {
            direcao_aprendida = 2;  // Direita
        } else if (y > 3072) {
            direcao_aprendida = 0;  // Frente
        }

        if (direcao_aprendida != -1) {
            display_message("Direcao Salva!");
            buzzer_play(1000, 200);
            sleep_ms(500);

            // Garante que "Pressione A para sair" realmente aparece
            display_message("Pressione A para sair");
        }
    }

    // ================== MODO NAVEGAÇÃO ==================
    if (navegacao_ativa) {
        if (direcao_aprendida == 0) {
            display_message("Siga em Frente");
            buzzer_play(1000, 200000);
        } else if (direcao_aprendida == 1) {
            display_message("Vire a Esquerda");
            buzzer_play(800, 100000);
        } else if (direcao_aprendida == 2) {
            display_message("Vire a Direita");
            buzzer_play(1200, 100000);
        } else {
            display_message("Nenhuma direcao aprendida");
        }
        sleep_ms(1000);
        desativar_todos_os_modos();
    }
}

void button_callback(uint gpio, uint32_t events) {
    absolute_time_t now = get_absolute_time();

    if (gpio == BUTTON_A_PIN && absolute_time_diff_us(last_button_a_press_time, now) > (DEBOUNCE_INTERVAL_MS * 1000)) {
        last_button_a_press_time = now;
        button_a_pressed = true;
    }
    
    if (gpio == BUTTON_B_PIN && absolute_time_diff_us(last_button_b_press_time, now) > (DEBOUNCE_INTERVAL_MS * 1000)) {
        last_button_b_press_time = now;
        button_b_pressed = true;
    }
    if (gpio == JOYSTICK_BUTTON_PIN && absolute_time_diff_us(last_joystick_press_time, now) > (DEBOUNCE_INTERVAL_MS * 1000)) {
        last_joystick_press_time = now;
        joystick_button_pressed = true;
    }    
}

void buttons_init() {
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    last_button_a_press_time = get_absolute_time();
    last_button_b_press_time = get_absolute_time();
    last_joystick_press_time = get_absolute_time();

    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
}

