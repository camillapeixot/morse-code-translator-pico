# Tradutor de Código Morse para Raspberry Pi Pico

Este projeto foi desenvolvido para a **Raspberry Pi Pico** e permite que o usuário insira texto através de três botões físicos, converta esse texto em código Morse e o reproduza com um buzzer. Além disso, o texto inserido é exibido em tempo real em um display OLED SSD1306.

## Funcionalidades

- **Entrada de Texto:** O usuário pode inserir texto (letras, números e espaço) através de três botões:
  - **PINO_NEXT**: Avança para o próximo caractere.
  - **PINO_CONFIRM**: Confirma a escolha do caractere atual e adiciona à entrada.
  - **PINO_FINALIZA**: Finaliza a entrada do texto.
  
- **Exibição OLED:** O texto inserido é exibido em um display OLED SSD1306 de 128x64 pixels.
  
- **Conversão para Código Morse:** O texto inserido é convertido em código Morse, que é então transmitido através de um buzzer.

- **Buzzer:** O buzzer emite sons para representar os sinais de ponto (.) e traço (-) do código Morse.

## Componentes Utilizados

- **Raspberry Pi Pico** (ou qualquer placa compatível com RP2040)
- **Display OLED SSD1306** (usando comunicação I2C)
- **Buzzer** (para reprodução do código Morse)
- **Botões Físicos** (para entrada de texto)
  
## Como Usar

### Conexões
- **Botões:**
  - **PINO_NEXT**: Conecte a um pino GPIO configurado como entrada.
  - **PINO_CONFIRM**: Conecte a outro pino GPIO configurado como entrada.
  - **PINO_FINALIZA**: Conecte a um terceiro pino GPIO configurado como entrada.
  
- **Buzzer:** Conecte o buzzer a um pino GPIO configurado como saída.

- **Display OLED (SSD1306):**
  - Conecte os pinos **SDA** e **SCL** a um barramento I2C.
  
### Passos para Execução
1. Conecte os componentes conforme o diagrama de pinos.
2. Compile o código usando a ferramenta de sua escolha (como o Visual Studio Code com o plugin para o Raspberry Pi Pico).
3. Faça o upload para a sua Raspberry Pi Pico.
4. Pressione os botões para inserir o texto. O texto será exibido no display OLED e convertido para código Morse.
5. O buzzer emitirá os sinais de ponto e traço correspondentes ao código Morse do texto inserido.

### Exemplos de Entrada e Saída

- **Entrada de Texto:** "HELLO"
- **Código Morse:** ".... . .-.. .-.. ---"
- **Reprodução no Buzzer:** Sons de ponto e traço para "HELLO"

## Licença

Este projeto é licenciado sob a Licença MIT - veja o arquivo [LICENSE](https://mit-license.org/) para mais detalhes.
