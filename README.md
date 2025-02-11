# PWM Servo Motor Control

Este projeto demonstra o controle de um servo motor utilizando PWM na Raspberry Pi Pico W. O programa executa os seguintes passos:

1. **Passos Iniciais (únicos):**
   - Posiciona o servo em 180° (pulso de 2400 µs) e aguarda 5 segundos.
   - Posiciona o servo em 90° (pulso de 1470 µs) e aguarda 5 segundos.
   - Posiciona o servo em 0° (pulso de 500 µs) e aguarda 5 segundos.

2. **Movimentação Contínua:**
   - Após os passos iniciais, o servo inicia uma movimentação suave entre 0° e 180°.
   - A movimentação é realizada através de um timer que atualiza o pulso a cada 10 ms, com incrementos de 5 µs.
   - Nessa fase, uma mensagem é impressa no monitor serial a cada 5 segundos.

## Estrutura do Projeto

O diretório do projeto contém os seguintes arquivos:

- **pwm_motor.c**: Código-fonte em C que implementa o controle do servo.
- **CMakeLists.txt**: Arquivo de configuração do CMake para compilar o projeto.
- **pico_sdk_import.cmake**: Script para importar o SDK da Raspberry Pi Pico.
- **diagram.json**: Arquivo para simulação do circuito utilizando a extensão Wokwi.
- **wokwi.toml**: Arquivo de configuração para simular o funcionamento do programa via Wokwi.

## Requisitos

- **Hardware:**
  - Raspberry Pi Pico W.
  - Servo motor conectado à GPIO 22.
  
- **Software:**
  - Visual Studio Code com a extensão Raspberry Pi Pico.
  - Extensão Wokwi para Visual Studio Code (para simulação).
  - Pico SDK devidamente configurado (importado via `pico_sdk_import.cmake`).

## Como Compilar e Carregar o Programa

### Compilação

1. **Abrir o Projeto:**
   - Inicie o Visual Studio Code e abra o diretório do projeto que contém todos os arquivos listados.

2. **Configurar o CMake:**
   - Abra a Paleta de Comandos (`Ctrl+Shift+P`) e selecione `CMake: Configure`.
   - Certifique-se de que o Pico SDK está corretamente configurado pelo arquivo `pico_sdk_import.cmake`.

3. **Construir o Projeto:**
   - Após a configuração, selecione `CMake: Build` na Paleta de Comandos.
   - O processo de compilação gerará um arquivo binário (tipo `.uf2`) na pasta de build.

### Carregando o Programa no Pico

1. **Entrar em Modo Bootloader:**
   - Conecte a Raspberry Pi Pico enquanto mantém pressionado o botão BOOTSEL para entrar no modo bootloader.

2. **Copiar o Arquivo UF2:**
   - Arraste e solte o arquivo `.uf2` gerado na unidade que aparece no seu sistema.

3. **Reiniciar o Dispositivo:**
   - Após a cópia, o Pico reiniciará e o programa será executado automaticamente.

## Simulação com Wokwi

1. **Abrir a Simulação:**
   - Utilize a extensão Wokwi para Visual Studio Code.
   - Abra o arquivo `diagram.json` para visualizar o diagrama do circuito.

2. **Configuração da Simulação:**
   - O arquivo `wokwi.toml` contém as configurações necessárias para simular o projeto.

3. **Executar a Simulação:**
   - Inicie a simulação pela extensão para observar o comportamento do servo motor conforme o programa.

## Comportamento do Programa

- **Inicialmente:**  
  O servo é posicionado em 180°, 90° e 0° (cada posição por 5 segundos) para demonstrar as posições iniciais.

- **Após os Passos Iniciais:**  
  Um timer de 10 ms entra em ação para realizar uma movimentação contínua e suave entre os ângulos de 0° e 180°.


## Vídeo de Demonstração

- Vídeo de demonstração disponível em <https://youtu.be/p6ZKYQe84tA>
