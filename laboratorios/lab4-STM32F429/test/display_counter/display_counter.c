/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2010-2015 Piotr Esden-Tempski <piotr@esden.net>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

// Librerias
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"

//Macros para giroscopio



// Prototipos de funciones
static void gpio_setup(void);
static void button_setup(void);
void global_setup(void);
void display(int);
int print_decimal(int);

int print_decimal(int num)
{
	int	ndx = 0;
	char	buf[10];
	int	len = 0;
	char	is_signed = 0;

	if (num < 0) {
		is_signed++;
		num = 0 - num;
	}
	buf[ndx++] = '\000';
	do {
		buf[ndx++] = (num % 10) + '0';
		num = num / 10;
	} while (num != 0);
	ndx--;
	if (is_signed != 0) {
		console_putc('-');
		len++;
	}
	while (buf[ndx] != '\000') {
		console_putc(buf[ndx--]);
		len++;
	}
	return len; /* number of characters printed */
}
static void gpio_setup(void)
{
	/* Habilita el reloj GPIOG */
	rcc_periph_clock_enable(RCC_GPIOG);

	/* Configura los pines GPIO13 y GPIO14 del puerto G como salidas. */
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO13 | GPIO14);
}

static void button_setup(void)
{
	/* Habilita el reloj GPIOA */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Configura el pin GPIO0 del puerto A) como entrada */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}

// Dentro de esta funcion se inicializan todas las configuraciones
void global_setup(void){
	console_setup(115200);
	button_setup();
	gpio_setup();
	clock_setup();
	sdram_init();
	lcd_spi_init();
	gfx_init(lcd_draw_pixel, 240, 320);
	
}
void display(int counter){
	char buffer [30];
	
	// Poner pantalla en blanco
	gfx_fillScreen(LCD_WHITE);
	
	// Configura tamaño y color de letra y escribe texto
	gfx_setTextSize(1);
	gfx_setTextColor(LCD_BLACK, LCD_WHITE);
	gfx_setCursor(15, 25);
	gfx_puts("Programa contador");
	// Copia contador + int en el buffer de caracteres
	sprintf(buffer, "Contador: %d", counter);
	gfx_setCursor(25, 50);
	gfx_puts(buffer);
	lcd_show_frame();
}

int main(void){
	int i;
	int bounce_time = 3;
	int counter = 0;
	global_setup();
	gpio_clear(GPIOG, GPIO13 | GPIO14);
	/* Blink the LED (PD13) on the board. */
	while (1) {
		display(counter);

		/* Upon button press, blink more slowly. */
		if (gpio_get(GPIOA, GPIO0) && bounce_time==0) {
			gpio_toggle(GPIOG, GPIO13);
			bounce_time = 3;
			counter++;
		}

		for (i = 0; i < 3000000; i++) {		/* Wait a bit. */
			__asm__("nop");
		}
		// Blink Red LED until timeout
		if(bounce_time>0){
			bounce_time--;
			gpio_toggle(GPIOG, GPIO14);
		}
		
	}

	return 0;
}
