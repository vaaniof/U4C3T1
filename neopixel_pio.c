#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include "pico/bootrom.h"

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

// Matriz de Mapeamento
char MatrizMapeamento[NUM_LINHAS][NUM_COLUNAS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

char ultimaTecla = 'X';

// Protótipos das Funções
void inicializar();
char AcharDigito();
void Processar(char);
void ExecutaAcao(char);
void ModoBootsel();
void npInit(uint);
void npSetLED(const uint, const uint8_t, const uint8_t, const uint8_t);
void npClear();
void npWrite();
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
struct pixel_t
{
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t npLED_t;

npLED_t leds[LED_COUNT]; // buffer de pixels que formam a matriz.

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

int main()
{
  char tecla;
  stdio_init_all();
  inicializar();

  while (true)
  {
    tecla = AcharDigito();
    Processar(tecla);
    sleep_ms(200);
  }
}

void inicializar(){
  // inicialização dos pinos
  npInit(LED_PIN);
  npClear();

  gpio_init(BUZZER_PIN);
  gpio_set_dir(BUZZER_PIN, GPIO_OUT);
  gpio_put(BUZZER_PIN, 0);

  gpio_init(R1);
  gpio_set_dir(R1, GPIO_OUT);
  gpio_put(R1, 1); // Inicializando as linhas do teclado em nível alto

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
            case 0: 
                gpio_put(R1, 0);
                sleep_ms(50); 
                break;
            case 1: 
                gpio_put(R2, 0);
                sleep_ms(50); 
                break;
            case 2: 
                gpio_put(R3, 0);
                sleep_ms(50); 
                break;
            case 3: 
                gpio_put(R4, 0);
                sleep_ms(50); 
                break;
        }

        // Verifica as colunas
            if (!gpio_get(C1)){
      
      return MatrizMapeamento[linha][0];
      }
    if (!gpio_get(C2)){
      
      return MatrizMapeamento[linha][1];
      }
    if (!gpio_get(C3)){
      
      return MatrizMapeamento[linha][2];
      }
    if (!gpio_get(C4)){
      
      return MatrizMapeamento[linha][3];
      }
    }

    // Retorna um caractere nulo caso nenhuma tecla tenha sido pressionada
    return '\0';
}
void Processar(char tecla){

  if (tecla != '\0')
  {

    // escreve a tecla somente se for diferente da ultima pressionada, evitando leituras repetidas
    if (ultimaTecla != tecla)
      printf("Tecla %c Pressionada\n", tecla);
    ultimaTecla = tecla;
    ExecutaAcao(tecla);
  }
  sleep_ms(10);
}
void ModoBootsel(){
  printf("Entrando em modo BOOTSEL...\n");
  sleep_ms(1000);           // Aguarde um segundo antes de reiniciar
  reset_usb_boot(0,0); // Reinicia e entra no modo BOOTSEL
}
void npInit(uint pin){
  // Inicializa a máquina PIO para controle da matriz de LEDs.

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b){
  // Atribui uma cor RGB a um LED.

  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}
void npClear(){
  // Limpa o buffer de pixels.

  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}
void npWrite(){
  // Escreve os dados do buffer nos LEDs.

  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}
void ExecutaAcao(char tecla)
{
  // executa as ações de cada Tecla

  switch (tecla)
  {
  case '0':
    Animacao_0();
    break;
  case '1':
    Animacao_1();
    break;
  case '2':
    Animacao_2();
    break;
  case '3':
    Animacao_3();
    break;
  case '4':
    Animacao_4();
    break;
  case '5':
    Animacao_5();
    break;
  case '6':
    Animacao_6();
    break;
  case '7':
    Animacao_7();
    break;
  case '8':
    Animacao_8();
    break;
  case '9':
    Animacao_9();
    break;
  case 'A':
    npClear();
    npWrite();
    break;
  case 'B':
    for (uint i = 0; i < LED_COUNT; ++i)
      npSetLED(i, 0, 0, 255);
    npWrite();
    break;
  case 'C':
    for (uint i = 0; i < LED_COUNT; ++i)
      npSetLED(i, 204, 0, 0);
    npWrite();
    break;
  case 'D':
    for (uint i = 0; i < LED_COUNT; ++i)
      npSetLED(i, 0, 127, 0);
    npWrite();
    break;
  case '#':
    for (uint i = 0; i < LED_COUNT; ++i)
      npSetLED(i, 50, 50, 50);
    npWrite();
    break;
  case '*':
    ModoBootsel();
    break;
  default:
    printf("[ERRO] VALOR NAO ESPERADO RECEBIDO");
    exit(1);
    break;
  }
}
void Animacao_0(){
  npClear();
  npSetLED(0,120,0,0);
  npWrite();
  sleep_ms(200);
  for (uint i = 1,p = 0; p < LED_COUNT; ++i,++p){
    if (i<LED_COUNT)
      npSetLED(i, 120, 0, 0);
    npSetLED(p,0,0,0);
    npWrite();
    sleep_ms(200);
  }
}
void Animacao_1(){
  npClear();
  //rosto feliz
  npSetLED(1,120,0,0);
  npSetLED(2,120,0,0);
  npSetLED(3,120,0,0);
  npSetLED(5,120,0,0);
  npSetLED(9,120,0,0);
  npSetLED(16,120,0,0);
  npSetLED(18,120,0,0);
  npWrite();
  //piscar olho esquerdo
  sleep_ms(500);
  npSetLED(16,0,0,0);
  npWrite();
  sleep_ms(500);
  npSetLED(16,120,0,0);
  npWrite();
  //piscar olho direito
  sleep_ms(500);
  npSetLED(18,0,0,0);
  npWrite();
  sleep_ms(500);
  npSetLED(18,120,0,0);
  npWrite();
  //mudar expressão
  npSetLED(1,0,0,0);
  npSetLED(2,0,0,0);
  npSetLED(3,0,0,0);
  npSetLED(5,0,0,0);
  npSetLED(9,0,0,0);
  npWrite();
  sleep_ms(500);
  npSetLED(6,120,0,0);
  npSetLED(7,120,0,0);
  npSetLED(8,120,0,0);
  npWrite();
  sleep_ms(500);
  npSetLED(0,120,0,0);
  npSetLED(4,120,0,0);
  npWrite();
  //limpar a tela
  sleep_ms(500);
  npClear();
  npWrite();
}
void Animacao_2(){
  bool s = true;
  npClear();
  npSetLED(24,120,0,0);
  npWrite();
  sleep_ms(200);
  for (uint i = (LED_COUNT-1),p = LED_COUNT ;p>0 ; --i,--p,s = !s){
    if (i>=0){
      if (s)
        npSetLED(i, 120, 0, 0);
      else
        npSetLED(i,0,120,0);
    }

    if (p<LED_COUNT)
      npSetLED(p,0,0,0);
    npWrite();
    sleep_ms(200);
  }
  npSetLED(0,0,0,0);
  npWrite();
}
void Animacao_3(){
  bool s = true;
  npClear();
  npSetLED(24,0,0,120);
  npWrite();
  sleep_ms(200);
  for (uint i = (LED_COUNT-1),p = LED_COUNT ;p>0 ; --i,--p,s = !s){
    if (i>=0){
      if (s)
        npSetLED(i, 0, 0, 120);
      else
        npSetLED(i,120,120,120);
    }

    if (p<LED_COUNT)
      npSetLED(p,0,0,0);
    npWrite();
    sleep_ms(200);
  }
  npSetLED(0,0,0,0);
  npWrite();
}
void Animacao_4(){
  bool s = true;
  npClear();
  npSetLED(0,120,120,120);
  npWrite();
  sleep_ms(200);
  for (uint i = 1,p = 0; p < LED_COUNT; ++i,++p, s = !s){
    if (i<LED_COUNT){
      if (s)
        npSetLED(i, 0, 0, 120);
      else
        npSetLED(i,120,120,120);
    }
    npSetLED(p,0,0,0);
    npWrite();
    sleep_ms(200);
  }
}
void Animacao_5(){
  npClear();
}
void Animacao_6(){
  npClear();
  npSetLED(12,50,50,50);
  npSetLED(3,50,50,50);
  npSetLED(6,50,50,50);
  npSetLED(1,50,50,50);
  npSetLED(8,50,50,50);
  npSetLED(14,50,50,50);
  npSetLED(18,50,50,50);
  npSetLED(16,50,50,50);
  npSetLED(22,50,50,50);
  npSetLED(17,50,50,50);
  npSetLED(10,50,50,50);
  npWrite();
  sleep_ms(50);
  for (int i = 0;i<7;i++){
    if (i%2){
      npSetLED(14,50,50,50);
      npSetLED(24,0,0,0);
      gpio_put(BUZZER_PIN,1);
      sleep_ms(500);
      gpio_put(BUZZER_PIN,0);
    }else{
      npSetLED(24,50,50,50);
      npSetLED(14,0,0,0);
      gpio_put(BUZZER_PIN,1);
      sleep_ms(500);
      gpio_put(BUZZER_PIN,0);
    }
    npWrite();
    sleep_ms(50);
  }
  npClear();
  npWrite();
}
void Animacao_7(){
  npClear();
  npSetLED(24,120,0,0);
  npWrite();
  sleep_ms(200);
  for (uint i = (LED_COUNT-1),p = LED_COUNT;p>0 ; --i,--p){
    if (i>=0)
      npSetLED(i, 120, 0, 0);
    if (p<LED_COUNT)
      npSetLED(p,0,0,0);
    npWrite();
    sleep_ms(200);
  }
  npSetLED(0,0,0,0);
  npWrite();
}
void Animacao_8(){
  npClear();
  //L
  for (uint i = 0; i < 6; ++i)
    npSetLED(i, 120, 0, 0);
  npSetLED(14,120,0,0);
  npSetLED(15,120,0,0);
  npSetLED(24,120,0,0);
  npWrite();
  sleep_ms(500);
  //O
  npClear();
  npSetLED(1,120,0,0);
  npSetLED(2,120,0,0);
  npSetLED(3,120,0,0);
  npSetLED(5,120,0,0);
  npSetLED(9,120,0,0);
  npSetLED(10,120,0,0);
  npSetLED(14,120,0,0);
  npSetLED(15,120,0,0);
  npSetLED(19,120,0,0);
  npSetLED(21,120,0,0);
  npSetLED(22,120,0,0);
  npSetLED(23,120,0,0);
  npWrite();
  sleep_ms(500);
  //V
  npClear();
  npSetLED(2,120,0,0);
  npSetLED(6,120,0,0);
  npSetLED(8,120,0,0);
  npSetLED(10,120,0,0);
  npSetLED(14,120,0,0);
  npSetLED(15,120,0,0);
  npSetLED(19,120,0,0);
  npSetLED(20,120,0,0);
  npSetLED(24,120,0,0);
  npWrite();
  sleep_ms(500);
  //E
  npClear();
  npSetLED(20,120,0,0);
  npSetLED(21,120,0,0);
  npSetLED(22,120,0,0);
  npSetLED(23,120,0,0);
  npSetLED(24,120,0,0);
  npSetLED(15,120,0,0);
  npSetLED(14,120,0,0);
  npSetLED(5,120,0,0);
  npSetLED(0,120,0,0);
  npSetLED(1,120,0,0);
  npSetLED(2,120,0,0);
  npSetLED(3,120,0,0);
  npSetLED(4,120,0,0);
  npSetLED(10,120,0,0);
  npSetLED(11,120,0,0);
  npSetLED(12,120,0,0);
  npSetLED(13,120,0,0);
  npWrite();
  sleep_ms(500);
  //Coração
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 120, 0, 0);
  npSetLED(0,0,0,0);
  npSetLED(1,0,0,0);
  npSetLED(3,0,0,0);
  npSetLED(4,0,0,0);
  npSetLED(5,0,0,0);
  npSetLED(9,0,0,0);
  npSetLED(20,0,0,0);
  npSetLED(22,0,0,0);
  npSetLED(24,0,0,0);
  npWrite();
  sleep_ms(500);
  npClear();
  npWrite();
}
void Animacao_9(){
  npClear();
  npSetLED(12,50,50,50);
  npSetLED(3,50,50,50);
  npSetLED(6,50,50,50);
  npSetLED(1,50,50,50);
  npSetLED(8,50,50,50);
  npSetLED(14,50,50,50);
  npSetLED(18,50,50,50);
  npSetLED(16,50,50,50);
  npSetLED(22,50,50,50);
  npSetLED(17,50,50,50);
  npSetLED(10,50,50,50);
  npWrite();
  sleep_ms(50);
  for (int i = 0;i<7;i++){
    if (i%2){
      npSetLED(10,50,50,50);
      npSetLED(20,0,0,0);
      gpio_put(BUZZER_PIN,1);
      sleep_ms(500);
      gpio_put(BUZZER_PIN,0);
    }else{
      npSetLED(20,50,50,50);
      npSetLED(10,0,0,0);
      gpio_put(BUZZER_PIN,1);
      sleep_ms(500);
      gpio_put(BUZZER_PIN,0);
    }
    npWrite();
    sleep_ms(50);
  }
  npClear();
  npWrite();
}