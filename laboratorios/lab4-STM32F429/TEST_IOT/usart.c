#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void clock_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA); // Habilita el reloj para el puerto GPIOA
    rcc_periph_clock_enable(RCC_USART1); // Habilita el reloj para USART1
}

static void usart_setup(void)
{
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9); // Configura el pin GPIO9 como alternativo
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9); // Configura el pin GPIO9 para la función alternativa 7 (USART1)

    usart_set_baudrate(USART1, 115200); // Configura la velocidad de baudios en 115200
    usart_set_databits(USART1, 8); // Configura 8 bits de datos
    usart_set_stopbits(USART1, USART_STOPBITS_1); // Configura 1 bit de parada
    usart_set_mode(USART1, USART_MODE_TX); // Configura USART1 en modo de transmisión y recepción
    usart_set_parity(USART1, USART_PARITY_NONE); // Sin paridad
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE); // Sin control de flujo

    usart_enable(USART1); // Habilita USART1
}

static void gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA); // Habilita el reloj para el puerto GPIOA

    // Configura el pin GPIO0 de GPIOA como entrada con resistencia de pull-up
    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO0);
}

int main(void)
{
    clock_setup();
    gpio_setup();

    while (1)
    {   lectura = read_xyz(); 
        if (gpio_get(GPIOA, GPIO0)) // Si el botón está presionado
        {enviar = !enviar;} // Invertir el valor del flag "enviar"
        if (enviar==True){
            usart_setup(); // Habilita la comunicación USART
            printf("USART habilitado por botón.\n");
            send_data(lectura, bateria_lvl);  // Si 'enviar' es verdadero, envía los datos.
        }
    }

    return 0;
}
