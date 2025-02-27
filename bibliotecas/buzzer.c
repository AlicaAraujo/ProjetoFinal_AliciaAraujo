#include "buzzer.h"
#include "pico/stdlib.h"

void buzzer_init() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

void buzzer_play(uint32_t frequencia_h, uint32_t intervalo_us) {
    // Calcula o período do som em microssegundos
    uint32_t param = (intervalo_us / frequencia_h);
    
    // Define o tempo em que o buzzer ficará ligado dentro de um ciclo (25% do período total)
    uint32_t ciclo1 = param / 4;
    
    // Define o tempo em que o buzzer ficará desligado dentro de um ciclo (restante do período)
    uint32_t ciclo2 = param - ciclo1;
    
    // Calcula o número de ciclos completos que cabem no intervalo total
    uint32_t quant = intervalo_us / param;

    // Verifica se a frequência está dentro do intervalo audível (20 Hz a 20 kHz)
    if ((frequencia_h > 20) && (frequencia_h < 20000)) {
        // Gera o som com a frequência desejada, repetindo o número de ciclos calculado
        for (int i = 0; i < quant; i++) {
            gpio_put(BUZZER_PIN, true);  // Liga o buzzer
            sleep_us(ciclo1);            // Aguarda o tempo ligado
            gpio_put(BUZZER_PIN, false); // Desliga o buzzer
            sleep_us(ciclo2);            // Aguarda o tempo desligado
        }
    } 
    else {
        // Caso a frequência esteja fora do intervalo audível, apenas espera pelo tempo total
        sleep_us(intervalo_us);
    }
}