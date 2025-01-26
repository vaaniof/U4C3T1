# Controle de Matriz de led 25 x 25 ws2812b <br/>Através de um Teclado Matricial 4x4 HX-543

Este projeto utiliza um microcontrolador Raspberry Pi Pico para controlar uma matriz de LEDs e um teclado matricial. O sistema permite a interação através do teclado, onde cada tecla pressionada aciona uma animação específica nos LEDs. Compatível com a plataforma Educacional Bitdoglab v6.3.

## Funcionalidades

- Controle de uma matriz de LEDs RGB.
- Leitura de entradas de um teclado matricial.
- Animações visuais baseadas nas teclas pressionadas.
- Um buzzer que emite sons em resposta a certas ações.

## Componentes

- Raspberry Pi Pico
- Matriz de LEDs (WS2818B)
- Teclado matricial 4x4
- Buzzer
- Resistores e fios de conexão

## Pinagem

| Componente | Pino |
|------------|------|
| LEDs       | 7    |
| Buzzer     | 21   |
| R1         | 18   |
| R2         | 16   |
| R3         | 19   |
| R4         | 17   |
| C1         | 20   |
| C2         | 4    |
| C3         | 9    |
| C4         | 8    |


## Configuração do Ambiente

Antes de começar, certifique-se de que você tenha o ambiente de desenvolvimento do **Raspberry Pi Pico** configurado corretamente. Siga as instruções do [Raspberry Pi Pico SDK](https://www.raspberrypi.org/documentation/rp2040/getting-started/) para configurar o SDK e as ferramentas de compilação.

## Compilação e Upload

1. Compile o código usando o ambiente de desenvolvimento configurado.
2. Após a compilação, faça o upload do código para o seu **Raspberry Pi Pico**.

## Uso

Após a inicialização, pressione as teclas do teclado matricial para acionar diferentes animações nos LEDs. As animações correspondem às teclas de **'0' a '9'**, **'A' a 'D'**, e **'*'** e **'#'**.

### Animações

- **Tecla '0'**: Animação de movimento de LEDs em vermelho.
- **Tecla '1'**: Rosto com piscadas e mudança de expressão.
- **Tecla '2'**: Animação de LEDs em vermelho e verde.
- **Tecla '3'**: Animação de LEDs em azul e branco.
- **Tecla '4'**: Diferentes animações visuais.
- **Tecla '5'**: Semelhante a animação 3 mas com sentido inverso.
- **Tecla '6'**: Aceno com a mão esquerda.
- **Tecla '7'**: Semelhante a animação 0, mas com sentido inverso.
- **Tecla '8'**: LOVE <3.
- **Tecla '9'**: Aceno com a mão direita.
- **Tecla 'A'**: Limpa a matriz de LEDs.
- **Tecla 'B'**: Acende todos os LEDs em azul com 100% da intensidade.
- **Tecla 'C'**: Acende todos os LEDs em vermelho com 80% da intensidade.
- **Tecla 'D'**: Acende todos os LEDs em verde com 50% da intensidade.
- **Tecla '#'**: Acende todos os LEDs brancos com 20% da intensidade.
- - **Tecla '*'**: Entra no modo BOOTSEL.

### Buzzer

O buzzer emite sons em resposta a certas animações, dependendo das teclas pressionadas.

## Tecnologias Utilizadas

- **C/C++**
- **Raspberry Pi Pico SDK**
- **PIO (Programmable Input/Output)** para controle de LEDs

## Desenvolvedores
- João Victor de Souza Silva
- Vanio Ferreira dos Santos Júnior
## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir um problema ou enviar um pull request.

