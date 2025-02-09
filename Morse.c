#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include <unistd.h>
#include "hardware/gpio.h"
#include "ssd1306.h"

#define PINO_NEXT 5
#define PINO_CONFIRM 6
#define PINO_FINALIZA 22
#define BUZZER 10
#define SDA_PIN 14
#define SCL_PIN 15

i2c_inst_t *i2c = i2c0;

ssd1306_t display;

void setup_oled() {
    i2c_init(i2c, 400000);  
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    
    // Inicializa o display SSD1306
    ssd1306_init(&display, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_I2C_ADDRESS, i2c);
}

// Mapeamento do código Morse
const char *morse_code[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",  // A-J
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",   // K-T
    "..-", "...-", ".--", "-..-", "-.--", "--..",                         // U-Z
    ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", // 0-9
    "/"
};

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

// Função para capturar entrada do usuário
void get_user_input(char *input) {
    int index = 0, length = 0;

    while (1) {
        if (!gpio_get(PINO_NEXT)) {
            sleep_ms(100);  // Debounce
            while (!gpio_get(PINO_NEXT));
            index = (index + 1) % strlen(alphabet);
            printf("Letra atual: %c\n", alphabet[index]);
        }
        if (!gpio_get(PINO_CONFIRM)) {
            sleep_ms(100);  // Debounce
            while (!gpio_get(PINO_CONFIRM));
            if (length < 99) {
                input[length++] = alphabet[index];
                input[length] = '\0';
                printf("Adicionado: %c\n", alphabet[index]);

                // Atualiza o display OLED
                ssd1306_clear(&display);
                ssd1306_draw_string(&display, 0, 0, 1, input);
                ssd1306_show(&display);
            }
        }
        if (!gpio_get(PINO_FINALIZA)) { 
            sleep_ms(100);
            while (!gpio_get(PINO_FINALIZA));
            break;
        }
    }
}

// Conversão de texto para código Morse
void text_to_morse(const char *text, char *morse_output) {
    morse_output[0] = '\0';
    for (int i = 0; i < strlen(text); i++) {
        char c = text[i];
        if (c >= 'A' && c <= 'Z') {
            strcat(morse_output, morse_code[c - 'A']);
        } else if (c >= '0' && c <= '9') {
            strcat(morse_output, morse_code[c - '0' + 26]);
        } else if (c == ' ') {
            strcat(morse_output, morse_code[36]);
        }
        strcat(morse_output, " ");
    }
}

// Reprodução do código Morse no buzzer
void play_morse(const char *morse_text) {
    for (int i = 0; i < strlen(morse_text); i++) {
        char symbol = morse_text[i];
        if (symbol == '.') {
            gpio_put(BUZZER, 1);
            sleep_ms(200);
        } else if (symbol == '-') {
            gpio_put(BUZZER, 1);
            sleep_ms(600);
        } else if (symbol == ' ') {
            sleep_ms(600);
        } else if (symbol == '/') {
            sleep_ms(1200);
        }
        gpio_put(BUZZER, 0);
        sleep_ms(200);
    }
}

int main() {
    stdio_init_all(); 
    
    // Inicializa o display OLED
    setup_oled();

    // Configura o LED embutido da Pico
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // LED piscar 5 vezes para indicar que esta funcionando
    for (int i = 0; i < 5; i++) {
        gpio_put(LED_PIN, 1);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }
   
    gpio_init(PINO_NEXT);
    gpio_set_dir(PINO_NEXT, GPIO_IN);
    gpio_pull_up(PINO_NEXT);

    gpio_init(PINO_CONFIRM);
    gpio_set_dir(PINO_CONFIRM, GPIO_IN);
    gpio_pull_up(PINO_CONFIRM);

    gpio_init(PINO_FINALIZA);
    gpio_set_dir(PINO_FINALIZA, GPIO_IN);
    gpio_pull_up(PINO_FINALIZA);

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    char user_text[100];
    char morse_output[500];

    while (1) {
        printf("Pressione os botões para inserir texto.\n");
        get_user_input(user_text);

        printf("Convertendo...\n");
        text_to_morse(user_text, morse_output);
        printf("Saída em Morse: %s\n", morse_output);

        play_morse(morse_output);

        printf("Reiniciando em 3 segundos...\n");
        sleep_ms(3000);
    }

    return 0;
}
