#include "buttons.h"

// Variáveis globais
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
volatile bool joystick_button_pressed = false;

absolute_time_t last_button_a_press_time;
absolute_time_t last_button_b_press_time;
absolute_time_t last_joystick_press_time;

bool aprendizado_ativo = false;
bool navegacao_ativa = false;
bool modo_seguro_ativo = false;
int direcao_aprendida = -1; // -1 = nenhuma direção, 0 = frente, 1 = esquerda, 2 = direita, 3 = trás

#define DEBOUNCE_INTERVAL_MS 200

// Função para desativar todos os modos
void desativar_todos_os_modos() {
    aprendizado_ativo = false;
    navegacao_ativa = false;
    modo_seguro_ativo = false;
    display_message("Modo Inicial");
    pwm_set_gpio_level(LED_R_PIN, 255);
    pwm_set_gpio_level(LED_G_PIN, 255);
    pwm_set_gpio_level(LED_B_PIN, 255);
}

// Função para ativar o modo de aprendizado
void ativar_modo_aprendizado() {
    if (aprendizado_ativo) {
        desativar_todos_os_modos();
        return;
    }
    aprendizado_ativo = true;
    display_message("Modo: Aprendizado");
    pwm_set_gpio_level(LED_R_PIN, 255);
    pwm_set_gpio_level(LED_G_PIN, 0);
    pwm_set_gpio_level(LED_B_PIN, 0);
    buzzer_play(500, 300000); // Som curto ao entrar no modo aprendizado
}

// Função para ativar o modo de navegação
void ativar_modo_navegacao() {
    if (direcao_aprendida == -1) {
        display_message("Nenhuma direcao aprendida");
        pwm_set_gpio_level(LED_R_PIN, 255);
        pwm_set_gpio_level(LED_G_PIN, 0);
        pwm_set_gpio_level(LED_B_PIN, 165);
        buzzer_play(400, 500000); // Som de erro quando não há direção aprendida
        desativar_todos_os_modos(); // Voltar ao modo inicial se não houver direção
        return;
    }
    navegacao_ativa = true;
    display_message("Modo: Navegacao");
    pwm_set_gpio_level(LED_R_PIN, 0);
    pwm_set_gpio_level(LED_G_PIN, 0);
    pwm_set_gpio_level(LED_B_PIN, 255);
    
    switch (direcao_aprendida) {
        case 0:
            display_message("Siga em Frente");
            pwm_set_gpio_level(LED_R_PIN, 0);
            pwm_set_gpio_level(LED_G_PIN, 255);
            pwm_set_gpio_level(LED_B_PIN, 0);
            buzzer_play(1000, 500000);
            break;
        case 1:
            display_message("Vire a Esquerda");
            pwm_set_gpio_level(LED_R_PIN, 255);
            pwm_set_gpio_level(LED_G_PIN, 255);
            pwm_set_gpio_level(LED_B_PIN, 0);
            buzzer_play(800, 500000);
            break;
        case 2:
            display_message("Vire a Direita");
            pwm_set_gpio_level(LED_R_PIN, 0);
            pwm_set_gpio_level(LED_G_PIN, 0);
            pwm_set_gpio_level(LED_B_PIN, 255);
            buzzer_play(1200, 500000);
            break;
        case 3:
            display_message("Va para tras");
            pwm_set_gpio_level(LED_R_PIN, 255);
            pwm_set_gpio_level(LED_G_PIN, 0);
            pwm_set_gpio_level(LED_B_PIN, 255);
            buzzer_play(600, 500000);
            break;
    }
    direcao_aprendida = -1;
    sleep_ms(1000); // Aguardar antes de voltar ao modo inicial
    desativar_todos_os_modos();
}

// Função para ativar/desativar o modo seguro
void toggle_modo_seguro() {
    safe_mode_toggle();
}

// Função para processar os botões
void processar_botoes() {
    // Botão A: Modo de Aprendizado
    if (button_a_pressed) {
        button_a_pressed = false;
        if (aprendizado_ativo) {
            desativar_todos_os_modos();
            return;
        }
        ativar_modo_aprendizado();
    }

    if (aprendizado_ativo && direcao_aprendida == -1) {
        joystick_read();
        if (joystick_y_value < 1024) direcao_aprendida = 1;
        else if (joystick_y_value > 3072) direcao_aprendida = 2;
        else if (joystick_x_value > 3072) direcao_aprendida = 0;
        else if (joystick_x_value < 1024) direcao_aprendida = 3;
        if (direcao_aprendida != -1) {
            display_message("Direcao Aprendida");
            pwm_set_gpio_level(LED_R_PIN, 255);
            pwm_set_gpio_level(LED_G_PIN, 10);
            pwm_set_gpio_level(LED_B_PIN, 80);
        }
    }

    // Botão B: Modo de Navegação
    if (button_b_pressed) {
        button_b_pressed = false;
        if (navegacao_ativa) {
            desativar_todos_os_modos();
            return;
        }
        ativar_modo_navegacao();
    }

    // Botão do Joystick: Modo Seguro
    if (joystick_button_pressed) {
        joystick_button_pressed = false;
        toggle_modo_seguro();
    }
}

// Callback para interrupções dos botões
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

// Inicialização dos botões
void buttons_init() {
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_init(JOYSTICK_BUTTON_PIN);

    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);

    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
}