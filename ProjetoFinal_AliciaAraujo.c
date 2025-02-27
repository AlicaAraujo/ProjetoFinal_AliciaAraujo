#include <stdio.h>
#include "pico/stdlib.h"
#include "bibliotecas/joystick.h"
#include "bibliotecas/buttons.h"
#include "bibliotecas/leds.h"
#include "bibliotecas/buzzer.h"
#include "bibliotecas/display.h"
#include "bibliotecas/safe_mode.h"

extern bool aprendizado_ativo, navegacao_ativa;

int main() {
    stdio_init_all();

    // Inicializa a semente do gerador de números aleatórios
    srand(to_us_since_boot(get_absolute_time()));

    // Inicializações
    joystick_init();
    buttons_init();
    leds_init();
    buzzer_init();
    display_init();

    // Inicia no Modo Inicial
    desativar_todos_os_modos();

    // Loop principal
    while (true) {
        // Lê os valores do joystick
        joystick_read();
        // Processa os botões para atualizar os estados
        processar_botoes();

        // Verifica se o botão A foi pressionado
        if (button_a_pressed) {
            printf("Botão A pressionado! \n");
            button_a_pressed = false;  // Reseta o estado do botão
        }

        // Verifica se o botão B foi pressionado
        if (button_b_pressed) {
            printf("Botão B pressionado! \n");
            button_b_pressed = false;  // Reseta o estado do botão
        }

        // Verifica se o botão do joystick foi pressionado
        if (joystick_button_pressed) {
            joystick_button_pressed = false;  // Reseta o estado do botão
        }

        // Atualizar LEDs com base no joystick (se não estiver em nenhum modo)
        if (!aprendizado_ativo && !navegacao_ativa && !safe_mode_active) {
            leds_update();
        }

        // Pequeno delay para evitar sobrecarga da CPU
        sleep_ms(100);
    }

    return 0;
}
