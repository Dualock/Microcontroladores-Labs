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
// libopencm3
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"

//Macros para giroscopio
#define WHO_AM_I 0x0F // Identificador
#define CTRL_REG1 0x20 // Registro 1: Control de ejes
#define CTRL_REG2 0x21	// Registro 2: Filtro paso alto
#define CTRL_REG4 0x23 // Registro 4: DPS y SPI config
#define MSB_SHIFT 8
// Macros para pantalla LCD

#define BATTERY_ICON_WIDTH 10
#define BATTERY_ICON_HEIGHT 40
#define BATTERY_POS_X 70
#define BATTERY_POS_Y 10
// Macros otros
#define BATTERY_MAX 9

// Prototipos de funciones
static void gpio_setup(void);
static void button_setup(void);
void global_setup(void);
void display(int *);
int print_decimal(int);
void usart_setup(void);
void read_xyz(int *);
uint16_t battery_color(float, bool);


 /* MEMS usa SPI5 - SCK (PF7), MISO (PF8), MOSI (PF9),
 * MEMS CS* (PC1)  -- GPIO
 * MEMS INT1 = PA1, MEMS INT2 = PA2
 */
 
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
uint16_t battery_color(float battery_voltage, bool critical){
	uint16_t color;
	int battery_level = battery_voltage*100/BATTERY_MAX;
	if(battery_level >= 75){
		color = LCD_GREEN;
		critical = false;
	}
	else if(battery_level >= 25 && battery_level < 75){
		color = LCD_YELLOW;
		critical = false;
	}
	else{
		color = LCD_RED;
		critical = true;
	}
	return color;
}
void display(int * ejes_matriz){
	char buffer [30];
	// Dibujar rectangulo de pantalla
	
	// Poner pantalla en blanco
	gfx_fillScreen(LCD_WHITE);
	
	// Configura tamaño y color de letra y escribe texto
	gfx_setTextSize(2);
	gfx_setTextColor(LCD_BLACK, LCD_WHITE);
	gfx_setCursor(15, 25);
	gfx_puts("Sismografo");
	// Eje x
	sprintf(buffer, "Eje x: %d", ejes_matriz[0]);
	gfx_setCursor(25, 50);
	gfx_puts(buffer);
	
	// Eje y
	sprintf(buffer, "Eje y: %d", ejes_matriz[1]);
	gfx_setCursor(25, 75);
	gfx_puts(buffer);
	
	// Eje z
	sprintf(buffer, "Eje z: %d", ejes_matriz[2]);
	gfx_setCursor(25, 100);
	gfx_puts(buffer);
	lcd_show_frame();
}

void read_xyz(int * ejes_matriz){
	ejes_matriz[0] = 1;
	ejes_matriz[1] = 2;
	ejes_matriz[2] = 3;
	//gpio_clear(GPIOC, GPIO1); // Se baja Chip Select
}


int main(void){
	int i;
	int bounce_time = 3; // tiempo de rebote del boton
	static bool critical = false; // bool para saber si la bateria está en estado critico
	int ejes_matriz [3];
	global_setup();
	gpio_clear(GPIOG, GPIO13 | GPIO14);
	/* Blink the LED (PD13) on the board. */
	while (1) {
		read_xyz(ejes_matriz);
		display(ejes_matriz);

		/* Al presionar el boton, pone a blinkear el LED G13 */
		if (gpio_get(GPIOA, GPIO0) && bounce_time==0) {
			gpio_toggle(GPIOG, GPIO13);
			bounce_time = 3;
		}

		for (i = 0; i < 3000000; i++) {		/* Delay */
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
