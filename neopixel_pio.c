#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"

// Definição do número dos pinos.
#define LED_COUNT 25
#define LED_PIN 7
#define NUM_LINHAS 4
#define NUM_COLUNAS 4
#define BUZZER_PIN 21
#define R1 18
#define R2 16
#define R3 19
#define R4 17
#define C1 20
#define C2 4
#define C3 9
#define C4 8

//Matriz de Mapeamento
char MatrizMapeamento [NUM_LINHAS][NUM_COLUNAS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

char ultimaTecla = 'X';

//Protótipos das Funções
void inicializar();
char AcharDigito();
void Processar(char);
void Animacao_0();
void Animacao_1();
void Animacao_2();
void Animacao_3();
void Animacao_4();
void Animacao_5();
void Animacao_6();
void Animacao_7();
void Animacao_8();
void Animacao_9();

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.


npLED_t leds[LED_COUNT]; //buffer de pixels que formam a matriz.

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

//Inicializa a máquina PIO para controle da matriz de LEDs.
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

//Atribui uma cor RGB a um LED.
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

//Limpa o buffer de pixels.
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

//Escreve os dados do buffer nos LEDs.
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

int main() {
  char tecla;
  stdio_init_all();

  while (true) {
    tecla = AcharDigito();
    Processar(tecla);
    sleep_ms(1000);
  }
}
void inicializar(){
    // inicialização dos pinos
    npInit(LED_PIN);
    npClear();

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN,0);

    gpio_init(R1);
    gpio_set_dir(R1, GPIO_OUT);
    gpio_put(R1, 1);  // Inicializando as linhas do teclado em nível alto

    gpio_init(R2);
    gpio_set_dir(R2, GPIO_OUT);
    gpio_put(R2, 1);

    gpio_init(R3);
    gpio_set_dir(R3, GPIO_OUT);
    gpio_put(R3, 1);

    gpio_init(R4);
    gpio_set_dir(R4, GPIO_OUT);
    gpio_put(R4, 1);

    // Inicializando os pinos das colunas como entrada com pull up
    gpio_init(C1);
    gpio_set_dir(C1, GPIO_IN);
    gpio_pull_up(C1);

    gpio_init(C2);
    gpio_set_dir(C2, GPIO_IN);
    gpio_pull_up(C2);

    gpio_init(C3);
    gpio_set_dir(C3, GPIO_IN);
    gpio_pull_up(C3);

    gpio_init(C4);
    gpio_set_dir(C4, GPIO_IN);
    gpio_pull_up(C4);
}
char AcharDigito() {
    for (int linha = 0; linha < 4; linha++) {
        // Coloca todas as linhas em nível alto
        gpio_put(R1, 1);
        gpio_put(R2, 1);
        gpio_put(R3, 1);
        gpio_put(R4, 1);

        // Coloca a linha atual em nível baixo
        switch (linha) {
            case 0: gpio_put(R1, 0); break;
            case 1: gpio_put(R2, 0); break;
            case 2: gpio_put(R3, 0); break;
            case 3: gpio_put(R4, 0); break;
        }

        // Verifica as colunas
        if (!gpio_get(C1)) return MatrizMapeamento[linha][0];
        if (!gpio_get(C2)) return MatrizMapeamento[linha][1];
        if (!gpio_get(C3)) return MatrizMapeamento[linha][2];
        if (!gpio_get(C4)) return MatrizMapeamento[linha][3];
    }

    // Retorna um caractere nulo caso nenhuma tecla tenha sido pressionada
    return '\0';
}
void Processar(char tecla){
    
    if (tecla != '\0'){

        //escreve a tecla somente se for diferente da ultima pressionada, evitando leituras repetidas 
        if (ultimaTecla != tecla)
            printf("Tecla %c Pressionada\n",tecla);
        ultimaTecla = tecla;

        //caso seja uma das teclas especiais executa as ações
        switch (tecla){
        case 'A':
          //
          break;
        case 'B':
          //
          break;
        case 'C':
          //
          break;
        case 'D':
          //
          break;
        case '#':
          //
          break;
        case '*':
          //
          break;
        default:
            break;
            }
        }

        //se nenhuma tecla estiver sendo pressionada 
        else
        {

        
        }
        sleep_ms(100);
}

