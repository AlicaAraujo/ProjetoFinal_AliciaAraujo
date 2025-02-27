<h1 align="center"> 🔧 Controle Inteligente de Dispositivos com Raspberry Pi Pico W</h1>

<h2 align="center">📝 Descrição do Projeto</h2>

Este projeto implementa um sistema embarcado utilizando o **Raspberry Pi Pico W** para controle de diversos dispositivos eletrônicos. O sistema combina **botões, joystick, LEDs RGB, buzzer e display OLED SSD1306** para interação e resposta visual/auditiva ao usuário.

<h2 align="center">🎯 Objetivo</h2>

Desenvolver um sistema embarcado que permita:
- **Controle por joystick**: Ajuste de LEDs RGB e navegação entre modos.
- **Feedback visual**: Informações exibidas em um **display OLED SSD1306**.
- **Sinalização sonora**: Utilização de um **buzzer** para alertas.
- **Modos de operação**:
  - **Modo Aprendizado**: Registra uma direção e exibe no display.
  - **Modo Navegação**: Indica a direção aprendida.
  - **Modo Seguro**: Guia o usuário para um local seguro.

<h2 align="center">⚡ Funcionalidades Implementadas</h2>

- **Botões**:
  - Botão A: Ativa/desativa o **Modo Aprendizado**.
  - Botão B: Ativa/desativa o **Modo Navegação**.
  - Botão do joystick: Alterna para o **Modo Seguro**.
- **Joystick**:
  - Controla LEDs RGB via PWM com base na posição X/Y.
  - Move um quadrado no display OLED para representação visual.
- **Buzzer**:
  - Emite sons conforme a direção aprendida no modo de navegação.
- **Display OLED SSD1306**:
  - Exibe mensagens e feedbacks ao usuário.

<h2 align="center">📚 Estrutura do Projeto</h2>

### 📂 **Bibliotecas**
- **`buttons.c/h`**: Gerenciamento dos botões com debounce e interrupção.
- **`joystick.c/h`**: Leitura analógica do joystick.
- **`leds.c/h`**: Controle de LEDs RGB via PWM.
- **`buzzer.c/h`**: Emissão de sinais sonoros.
- **`display.c/h`**: Interação com o display OLED SSD1306.
- **`safe_mode.c/h`**: Implementação do Modo Seguro.
- **`ssd1306.c/h`**: Driver para o display OLED SSD1306.
- **`font.h`**: Definição de caracteres para exibição no display.

### 📂 **Arquivos principais**
- **`ProjetoFinal_AliciaAraujo.c`**: Arquivo principal do projeto.
- **`CMakeLists.txt`**: Configuração do sistema de build para compilação com o SDK do Raspberry Pi Pico.

<h2 align="center">🖥️ Como Executar</h2>

### ✅ Requisitos

- **Raspberry Pi Pico W**
- **VS Code** com extensão **Pico-W C/C++**
- **SDK do Raspberry Pi Pico** corretamente configurado

### 🛠️ Passos para Compilar e Executar

1. Clone este repositório:
   ```bash
   git clone https://github.com/SeuUsuario/ProjetoFinal.git
   ```
2. Entre no diretório do projeto:
   ```bash
   cd ProjetoFinal
   ```
3. Compile o projeto:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. Transfira o arquivo **.uf2** gerado para o Raspberry Pi Pico W.

<h2 align="center">📹 Demonstração</h2>

🎥 **Produza um vídeo de até 5 minutos**, contendo:
- Demonstração do funcionamento do projeto.
- Explicação breve das funcionalidades.
- Execução do código na placa **BitDogLab**.

<h2 align="center">📅 Informações</h2>

📌 **Autor(a)**: Alícia Oliveira Araújo  
🛠️ **Última Modificação**: 18/02/2025  

Projeto desenvolvido para estudos de sistemas embarcados com **Raspberry Pi Pico W**.

