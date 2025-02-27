#include "safe_mode.h"
#include "leds.h" 
#include "buzzer.h"
#include "display.h"

bool safe_mode_active = false;
int direcao_segura = 0;

void safe_mode_toggle() {
    if (safe_mode_active) {
        // Se o modo já estiver ativo, desativa e retorna ao estado inicial
        safe_mode_active = false;
        display_message("Modo Inicial");

        // Define todos os LEDs para brilho máximo (branco)
        pwm_set_gpio_level(LED_R_PIN, 255);  // LED vermelho ligado
        pwm_set_gpio_level(LED_G_PIN, 255);  // LED verde ligado
        pwm_set_gpio_level(LED_B_PIN, 255);  // LED azul ligado

        return;
    }

    // Se o modo não estiver ativo, ativa o modo seguro
    desativar_todos_os_modos();  // Desativa os outros modos antes de ativar
    safe_mode_active = true;

    display_message("Modo: Localização Segura");

    // Gera uma direção segura aleatória (0, 1 ou 2)
    direcao_segura = rand() % 3;
    printf("Direção Segura: %d\n", direcao_segura);  // Depuração

    safe_mode_guide();  // Chama o guia de orientação segura
}

void safe_mode_guide() {
    if (!safe_mode_active) {
        return; // Se não estiver ativo, não faz nada
    }

    switch (direcao_segura) {
        case 0:
            display_message("Siga em Frente");
            buzzer_play(5000, 2500000);  // Som contínuo para indicar "siga em frente" 
            pwm_set_gpio_level(LED_R_PIN, 0);    // Vermelho desligado
            pwm_set_gpio_level(LED_G_PIN, 255);  // Verde ligado
            pwm_set_gpio_level(LED_B_PIN, 0);    // Azul desligado
            break;
        
        case 1:
            display_message("Vire a Esquerda");
            buzzer_play(2500, 2500000);  // Som intermitente para indicar "vire à esquerda" 
            pwm_set_gpio_level(LED_R_PIN, 0);    // Vermelho desligado
            pwm_set_gpio_level(LED_G_PIN, 0);    // Verde desligado
            pwm_set_gpio_level(LED_B_PIN, 255);  // Azul ligado
            break;
        
        case 2:
            display_message("Vire a Direita");
            buzzer_play(1500, 2500000);  // Som intermitente para indicar "vire à direita" 
            pwm_set_gpio_level(LED_R_PIN, 255);  // Vermelho ligado
            pwm_set_gpio_level(LED_G_PIN, 0);    // Verde desligado
            pwm_set_gpio_level(LED_B_PIN, 0);    // Azul desligado
            break;
    }
}
