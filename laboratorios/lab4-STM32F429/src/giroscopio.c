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
#define BATTERY_ICON_HEIGHT 10
#define BATTERY_POS_X 180
#define BATTERY_POS_Y 25
// Macros otros
#define BATTERY_MAX 9

// Prototipos de funciones
static void gpio_setup(void);
static void button_setup(void);
void global_setup(void);
void display(int *, float);
int print_decimal(int);
void usart_setup(void);
void read_xyz(int *);
void fill_battery_rectangles(int);

// Banderas globales
static bool flag_critical = false; // bool para saber si la bateria está en estado critico

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

void fill_battery_rectangles(int battery_level){
	uint16_t color; // Color de relleno de bateria
	int current_pos_x = 0;
	int i; // iterador
	
	// Bateria al maximo, llena 4 cuadros con verde
	if (battery_level >= 95){
		color = LCD_GREEN;
		for(i = 0; i < 4; i++){
			current_pos_x = BATTERY_POS_X+i*BATTERY_ICON_WIDTH;
			gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, color);
		}
	
	}
	// Rellena 3 rectangulos con verde y 1 con blanco
	else if(battery_level >= 75 && battery_level < 95){
		color = LCD_GREEN;
		for(i = 0; i < 4; i++){
			current_pos_x = BATTERY_POS_X+i*BATTERY_ICON_WIDTH;
			if(i < 3){
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, color);
			}
			else{
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, LCD_WHITE);
			}	
		}
	}
	// Rellena 3 rectangulos con naranja y 1 con blanco
	else if(battery_level >= 50 && battery_level < 75){
		color = LCD_ORANGE;
		for(i = 0; i < 4; i++){
			current_pos_x = BATTERY_POS_X+i*BATTERY_ICON_WIDTH;
			
			if(i < 3){
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, color);
			}
			else{
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, LCD_WHITE);
			}
			
		}
	}
	// Rellena 2 rect con amarillo y 2 con blanco
	else if(battery_level >= 25 && battery_level < 50){
		color = LCD_YELLOW;
		for(i = 0; i < 4; i++){
			current_pos_x = BATTERY_POS_X+i*BATTERY_ICON_WIDTH;
			
			if(i < 2){
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, color);
			}
			else{
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, LCD_WHITE);
			}
			
		}
		flag_critical = false;
	}
	else{
		color = LCD_RED;
			for(i = 0; i < 4; i++){
			current_pos_x = BATTERY_POS_X+i*BATTERY_ICON_WIDTH;
			
			if(i < 1){
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, color);
			}
			else{
				gfx_fillRect(current_pos_x+1, BATTERY_POS_Y+1, BATTERY_ICON_WIDTH-2, BATTERY_ICON_HEIGHT-2, LCD_WHITE);
			}
			
		}
		flag_critical = true;
	}
}
/** Funcion display para graficar datos en la pantalla LCD
*   @param: ejes_matriz: matriz de enteros con los datos x,y,z leidos del giroscopio
*   @param: battery_voltage: float con el valor leido de tension
*   @param: color: int de 16 bits con el color del indicador de bateria 
*/
void display(int * ejes_matriz, float battery_voltage){
	// Poner pantalla en blanco
	gfx_fillScreen(LCD_WHITE);
	int current_pos_x = 0;
	char buffer [30]; // Buffer para concatenar string + numero
	int battery_level = battery_voltage*100/BATTERY_MAX; // Nivel de bateria en porcentaje %
	// Dibujar 4 rectangulos de bateria
	for (int i = 0; i<4; i++){
		current_pos_x = BATTERY_POS_X+i*BATTERY_ICON_WIDTH;
		gfx_drawRect(current_pos_x, BATTERY_POS_Y, BATTERY_ICON_WIDTH, BATTERY_ICON_HEIGHT, LCD_BLACK);
	}
	fill_battery_rectangles(battery_level);
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
	
	// Porcentaje de bateria
	gfx_setTextSize(1);
	sprintf(buffer, "%d", battery_level);
	gfx_setCursor(BATTERY_POS_X+BATTERY_ICON_WIDTH/2, BATTERY_POS_Y + BATTERY_ICON_HEIGHT+1);
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
	int ejes_matriz [3]; // Matriz de enteros con los datos leidos por el giroscopio
	float battery_voltage = 0; // voltaje medido
	global_setup();
	gpio_clear(GPIOG, GPIO13 | GPIO14);
	/* Loop principal*/
	while (1) {
		read_xyz(ejes_matriz);
		display(ejes_matriz, battery_voltage);
		
		/* Al presionar el boton, pone a blinkear el LED G13 */
		if (gpio_get(GPIOA, GPIO0) && bounce_time==0) {
			gpio_toggle(GPIOG, GPIO13);
			bounce_time = 3;
			battery_voltage+=0.9;
		}

		for (i = 0; i < 3000000; i++) {		/* Delay */
			__asm__("nop");
		}
		// Blink Red LED until timeout
		if(bounce_time>0){
			bounce_time--;
		}
		if(flag_critical){
			gpio_toggle(GPIOG, GPIO14);
		}
		
	}

	return 0;
}
