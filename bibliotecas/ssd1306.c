#include "ssd1306.h"
#include "font.h"

/*
* Função para inicializar o display SSD1306.
*
* Configura as dimensões do display, o endereço I2C e cria os buffers necessários para 
* o envio de dados ao display.
*
* Parâmetros: 
* - ssd: Ponteiro para a estrutura do display.
* - width: Largura do display em pixels.
* - height: Altura do display em pixels.
* - external_vcc: Indica se o display utiliza alimentação externa (true) ou interna (false).
* - address: Endereço I2C do display.
* - i2c: Ponteiro para a instância I2C utilizada.
*/
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8U;
    ssd->address = address;
    ssd->i2c_port = i2c;
    ssd->bufsize = ssd->pages * ssd->width + 1;
    ssd->ram_buffer = calloc(ssd->bufsize, sizeof(uint8_t));
    ssd->ram_buffer[0] = 0x40;
    ssd->port_buffer[0] = 0x80;
}

/*
* Função para configurar o display SSD1306 com os parâmetros necessários.
*
* Envia uma sequência de comandos ao display para configurar a memória de exibição, 
* contraste, orientação e outras configurações essenciais para o funcionamento adequado.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
*/
void ssd1306_config(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_DISP | 0x00);
    ssd1306_command(ssd, SET_MEM_ADDR);
    ssd1306_command(ssd, 0x01);
    ssd1306_command(ssd, SET_DISP_START_LINE | 0x00);
    ssd1306_command(ssd, SET_SEG_REMAP | 0x01);
    ssd1306_command(ssd, SET_MUX_RATIO);
    ssd1306_command(ssd, HEIGHT - 1);
    ssd1306_command(ssd, SET_COM_OUT_DIR | 0x08);
    ssd1306_command(ssd, SET_DISP_OFFSET);
    ssd1306_command(ssd, 0x00);
    ssd1306_command(ssd, SET_COM_PIN_CFG);
    ssd1306_command(ssd, 0x12);
    ssd1306_command(ssd, SET_DISP_CLK_DIV);
    ssd1306_command(ssd, 0x80);
    ssd1306_command(ssd, SET_PRECHARGE);
    ssd1306_command(ssd, 0xF1);
    ssd1306_command(ssd, SET_VCOM_DESEL);
    ssd1306_command(ssd, 0x30);
    ssd1306_command(ssd, SET_CONTRAST);
    ssd1306_command(ssd, 0xFF);
    ssd1306_command(ssd, SET_ENTIRE_ON);
    ssd1306_command(ssd, SET_NORM_INV);
    ssd1306_command(ssd, SET_CHARGE_PUMP);
    ssd1306_command(ssd, 0x14);
    ssd1306_command(ssd, SET_DISP | 0x01);
}

/*
* Função para enviar um comando ao display SSD1306 via I2C.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - command: Comando a ser enviado ao display.
*/
void ssd1306_command(ssd1306_t *ssd, uint8_t command) {
    ssd->port_buffer[1] = command;
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->port_buffer,
        2,
        false
    );
}

/*
* Função para enviar os dados do buffer RAM para o display SSD1306.
*
* Atualiza o display enviando os dados armazenados no buffer interno via I2C.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
*/
void ssd1306_send_data(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_COL_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->width - 1);
    ssd1306_command(ssd, SET_PAGE_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->pages - 1);
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->ram_buffer,
        ssd->bufsize,
        false
    );
}

/*
* Função para definir o estado de um pixel no buffer do display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - x: Coordenada X do pixel.
* - y: Coordenada Y do pixel.
* - value: Estado do pixel (true para ligado, false para desligado).
*/
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
    uint16_t index = (y >> 3) + (x << 3) + 1;
    uint8_t pixel = (y & 0b111);
    if (value)
        ssd->ram_buffer[index] |= (1 << pixel);
    else
        ssd->ram_buffer[index] &= ~(1 << pixel);
}

/*
* Função para preencher todo o display com um único valor (ligado ou desligado).
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - value: Estado para preencher o display (true para ligado, false para desligado).
*/
void ssd1306_fill(ssd1306_t *ssd, bool value) {
    for (uint8_t y = 0; y < ssd->height; ++y) {
        for (uint8_t x = 0; x < ssd->width; ++x) {
            ssd1306_pixel(ssd, x, y, value);
        }
    }
}

/*
* Função para desenhar um caractere no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - c: Caractere a ser desenhado.
* - x: Coordenada X do canto superior esquerdo do caractere.
* - y: Coordenada Y do canto superior esquerdo do caractere.
*/
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y) {
    uint16_t index = 0;

    if (c >= '0' && c <= '9') {
        index = (c - '0' + 1) * 8;
    } 
    else if (c >= 'A' && c <= 'Z') {
        index = (c - 'A' + 11) * 8;
    } 
    else if (c >= 'a' && c <= 'z') {
        index = (c - 'a' + 37) * 8;
    } 
    else {
        switch (c) {
            case '!': index = 63 * 8; break; // Exclamação
            case ':': index = 64 * 8; break; // Dois pontos
            case '?': index = 65 * 8; break; // Interrogação
            case '.': index = 66 * 8; break; // Ponto
            case ',': index = 67 * 8; break; // Vírgula
            case ' ': index = 68 * 8; break; // Espaço
            case '@': index = 69 * 8; break; // Arroba
            case '#': index = 70 * 8; break; // Cerquilha
            case '+': index = 71 * 8; break; // Mais
            case '/': index = 72 * 8; break; // Barra
            case '*': index = 73 * 8; break; // Asterisco
            case '-': index = 74 * 8; break; // Hífen
            case '=': index = 75 * 8; break; // Igual
            default: return;  // Ignora caracteres desconhecidos
        }
    }

    for (uint8_t i = 0; i < 8; ++i) {
        uint8_t line = font[index + i];  // Obtém o byte correto da fonte
        for (uint8_t j = 0; j < 8; ++j) {
            ssd1306_pixel(ssd, x + i, y + j, (line >> j) & 1);
        }
    }
}

/*
* Função para desenhar uma string no display.
*
* Parâmetros:
* - ssd: Ponteiro para a estrutura do display.
* - str: String a ser desenhada.
* - x: Coordenada X do canto superior esquerdo da string.
* - y: Coordenada Y do canto superior esquerdo da string.
*/
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y) {
    while (*str) {
        ssd1306_draw_char(ssd, *str++, x, y);
        x += 8; // Ajuste no espaçamento dos caracteres

        // Se atingir a borda da tela, faz a quebra de linha corretamente
        if (x + 8 >= ssd->width) {
            x = 0;  // Reinicia na posição inicial da linha
            y += 8; // Avança para a próxima linha
        }

        // Impede que o texto ultrapasse a altura do display
        if (y + 8 >= ssd->height) {
            break;
        }
    }
}