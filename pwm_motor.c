#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/time.h"  
#include "hardware/clocks.h"

#define SERVO_PIN 22    // Pino conectado ao servo motor
#define PWM_FREQ 50     // Frequência do PWM: 50Hz (período de 20ms)

// Limites de pulso para o servo (em microssegundos)
#define PULSE_MIN 500   // Aproximadamente 0° 
#define PULSE_MID 1470  // Aproximadamente 90°
#define PULSE_MAX 2400  // Aproximadamente 180°

// Incremento do pulso para a movimentação suave (em microssegundos)
#define PULSE_STEP 5
// Intervalo de tempo entre os incrementos (em milissegundos)
#define STEP_DELAY_MS 10

// Variáveis globais para o controle do pulso na movimentação contínua
volatile uint16_t target_pulse = PULSE_MIN; // Pulso atual (inicialmente em 0°)
volatile bool increasing = true;            // Flag para indicar direção do movimento

uint32_t period_counts = 0; // Armazena o número de contagens correspondente ao período PWM

// Função para atualizar o nível de PWM com base no pulso desejado (em µs)
// Calcula o valor correspondente considerando o período total (20ms)
void update_servo(uint16_t pulse_width_us) {
    // O duty cycle é dado por: (pulse_width_us / 20000) * (wrap+1)
    uint32_t level = (pulse_width_us * period_counts) / 20000;
    pwm_set_gpio_level(SERVO_PIN, level);
}

// Configuração do PWM para obter um período de 20ms (50Hz)
void pwm_setup() {
    // Configura o pino como saída PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    
    // Definindo o divisor de clock; 64.0 é uma escolha que permite um wrap adequado
    float clkdiv = 64.0;
    pwm_set_clkdiv(slice_num, clkdiv);
    
    // Obtem a frequência do clock do sistema
    uint32_t clock_freq = clock_get_hz(clk_sys);
    
    // Calcula o valor de wrap para obter um período de 20ms:
    // período (s) = (wrap + 1) * clkdiv / clock_freq  =>  wrap = (20e-3 * clock_freq / clkdiv) - 1
    uint32_t wrap = (uint32_t)((20e-3 * clock_freq) / clkdiv) - 1;
    pwm_set_wrap(slice_num, wrap);
    
    // Calcula o número total de contagens do período (wrap + 1)
    period_counts = wrap + 1;
    
    // Habilita o PWM no slice correspondente
    pwm_set_enabled(slice_num, true);
}

// Callback do timer repetitivo: chamado a cada 10ms para atualizar a posição do servo
bool servo_timer_callback(repeating_timer_t *rt) {
    // Atualiza o pulso conforme a direção
    if (increasing) {
        target_pulse += PULSE_STEP;
        if (target_pulse >= PULSE_MAX) {
            target_pulse = PULSE_MAX; // Limita ao máximo (180°)
            increasing = false;       // Inverte a direção para reduzir o pulso
        }
    } else {
        target_pulse -= PULSE_STEP;
        if (target_pulse <= PULSE_MIN) {
            target_pulse = PULSE_MIN; // Limita ao mínimo (0°)
            increasing = true;        // Inverte a direção para aumentar o pulso
        }
    }
    
    // Atualiza o PWM com o novo pulso
    update_servo(target_pulse);
    
    return true; // Retorna true para manter o timer repetitivo ativo
}

int main() {
    stdio_init_all();   
    pwm_setup();        // Configura o PWM na GPIO do servo
    
    // Executa os passos iniciais da tarefa
    // Passo 2: Posiciona o servo em 180° (pulso de 2400µs) e aguarda 5 segundos
    update_servo(PULSE_MAX);
    sleep_ms(5000);
    
    // Passo 3: Posiciona o servo em 90° (pulso de 1470µs) e aguarda 5 segundos
    update_servo(PULSE_MID);
    sleep_ms(5000);
    
    // Passo 4: Posiciona o servo em 0° (pulso de 500µs) e aguarda 5 segundos
    update_servo(PULSE_MIN);
    sleep_ms(5000);
    
    // Define os valores iniciais para a movimentação contínua (Passo 5)
    target_pulse = PULSE_MIN;
    increasing = true;
    
    // Cria um timer repetitivo que chama 'servo_timer_callback' a cada 10ms
    repeating_timer_t timer;
    // add_repeating_timer_ms retorna um timer que dispara a cada STEP_DELAY_MS (10ms)
    add_repeating_timer_ms(STEP_DELAY_MS, servo_timer_callback, NULL, &timer);
    
    while (true) {
        printf("loop rodando, servo motor mexendo...\n");
        sleep_ms(5000);
    }
    
    return 0;
}
