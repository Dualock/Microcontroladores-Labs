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
#include <libopencm3/stm32/spi.h>
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
#define MSB_SHIFT 8 // desplazamiento del byte mas significativo
#define OUT_X_L 0x28 // Salida eje X, nibble inferior
#define OUT_X_H 0x29 // Salida eje X, nibble superior
#define OUT_Y_L 0x2A // Salida eje Y, nibble inferior
#define OUT_Y_H 0x2B // Salida eje Y, nibble superior
#define OUT_Z_L 0x2C // Salida eje Z, nibble inferior
#define OUT_Z_H 0x2D // Salida eje Z, nibble superior
#define READ_BIT_MASk 0x80 // Poner en 1 este bit permite lectura del giroscopio
#define SENSITIVITY_FACTOR_500DPS 0.0175F // Sensitividad 500 grados por segundo 17.5mdps/digit
#define SENSITIVITY_FACTOR_2000DPS 0.07F

// Macros para pantalla LCD

#define BATTERY_ICON_WIDTH 10
#define BATTERY_ICON_HEIGHT 10
#define BATTERY_POS_X 180
#define BATTERY_POS_Y 25
// Macros letura de tension
#define BATTERY_MAX 9
#define CLOSE_MAX_WARNING 6.5F

// Macros random
#define OUT_TEMP		0x26
#define STATUS_REG		0x27
#define CTRL_REG1		0x20
#define CTRL_REG1_PD	(1 << 3)
#define CTRL_REG1_XEN	(1 << 1)
#define CTRL_REG1_YEN	(1 << 0)
#define CTRL_REG1_ZEN	(1 << 2)
#define CTRL_REG1_BW_SHIFT 	4
#define CTRL_REG4		0x23
#define CTRL_REG4_FS_SHIFT	4

// Prototipos de funciones
static void gpio_setup(void);
static void button_setup(void);
static void spi_setup(void);
void gyroscope_setup(void);
void global_setup(void);
void display(int16_t *, float);
int print_decimal(int);
void usart_setup(void);
void read_xyz(int16_t *);
void fill_battery_rectangles(int);
uint8_t read_register_8bits(uint8_t);

// Banderas globales
static bool flag_low_battery = false; // bool para saber si la bateria está en estado critico
static uint8_t flag_usart_active = 1; // entero sin signo para anunciar cuando está activa la transmisión
static uint8_t limit_reaching_warning = 0; // bool para detectar si la tension de entrada es muy alta

 /* MEMS usa SPI5 - SCK (PF7), MISO (PF8), MOSI (PF9),
 * MEMS CS* (PC1)  -- GPIO
 * MEMS INT1 = PA1, MEMS INT2 = PA2
 */
 
static void gpio_setup(void){
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

uint8_t read_register_8bits(uint8_t reg){
	int8_t data;
	gpio_clear(GPIOC, GPIO1); // Se baja Chip Select
	spi_send(SPI5, reg); // Envia la solicitud de lectura
	spi_read(SPI5); // Recibe respuesta de SPI5
	spi_send(SPI5, 0); // Envia un byte de ceros
	data = spi_read(SPI5); // Lee la respuesta y la guarda en data
	gpio_set(GPIOC, GPIO1); // Se sube el Chip Select
	return data;
}


static void spi_setup(void) {
	rcc_periph_clock_enable(RCC_SPI5); // Reloj habilitado para SPI5
	/* Para señales SPI */
	rcc_periph_clock_enable(RCC_GPIOC); // Reloj habilitado para puerto C
	/* Para seleccionar el modo SPI en l3gd20 */
	rcc_periph_clock_enable(RCC_GPIOF); // Reloj habilitado para puerto F

	/* Setup GPIOc1 pin for spi mode l3gd20 select. */
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
	/* Start with spi communication disabled */
	gpio_set(GPIOC, GPIO1);

	/* Setup GPIO pins for AF5 for SPI5 signals. */
	gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7 | GPIO8 | GPIO9);
	gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);

	//Inicialización de SPI
	spi_set_master_mode(SPI5);
	spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);
	spi_set_clock_polarity_0(SPI5);
	spi_set_clock_phase_0(SPI5);
	spi_set_full_duplex_mode(SPI5);
	spi_set_unidirectional_mode(SPI5);
	spi_enable_software_slave_management(SPI5);
	spi_send_msb_first(SPI5);
	spi_set_nss_high(SPI5);
	SPI_I2SCFGR(SPI5) &= ~SPI_I2SCFGR_I2SMOD;
	spi_enable(SPI5);
}


void gyroscope_setup(void){
	spi_setup();
	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, CTRL_REG1);
	spi_read(SPI5);
	spi_send(SPI5, CTRL_REG1_PD | CTRL_REG1_XEN | CTRL_REG1_YEN | CTRL_REG1_ZEN | (3 << 
	CTRL_REG1_BW_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);

	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, CTRL_REG4);
	spi_read(SPI5);
	spi_send(SPI5, (3 << CTRL_REG4_FS_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);
}


// Dentro de esta funcion se inicializan todas las configuraciones
void global_setup(void){
	console_setup(115200);
	button_setup();
	gpio_setup();
	clock_setup();
	sdram_init();
	spi_setup();
	gyroscope_setup();
	lcd_spi_init();
	gfx_init(lcd_draw_pixel, 240, 320);
}


/** Funcion fill_battery_rectangles rellena con colores los rectangulos del indicador de bateria
*   @param: battery_levet: int que indica el nivel de bateria en porcentaje
*/
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
		flag_low_battery = false;
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
		flag_low_battery = true;
	}
}

/** Funcion display para graficar datos en la pantalla LCD
*   @param: axis_matriz: matriz de enteros con los datos x,y,z leidos del giroscopio
*   @param: battery_voltage: float con el valor leido de tension
*/
void display(int16_t * axis_matriz, float battery_voltage){
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
	sprintf(buffer, "Eje x: %d", axis_matriz[0]);
	gfx_setCursor(25, 50);
	gfx_puts(buffer);
	
	// Eje y
	sprintf(buffer, "Eje y: %d", axis_matriz[1]);
	gfx_setCursor(25, 75);
	gfx_puts(buffer);
	
	// Eje z
	sprintf(buffer, "Eje z: %d", axis_matriz[2]);
	gfx_setCursor(25, 100);
	gfx_puts(buffer);
	
	// USART
	if(flag_usart_active){
		sprintf(buffer, "Usart: On");
	}
	else{
		sprintf(buffer, "Usart: Off");
	}
	
	gfx_setCursor(25, 125);
	gfx_puts(buffer);
	
	// Porcentaje de bateria
	gfx_setTextSize(1);
	sprintf(buffer, "%d %%", battery_level);
	gfx_setCursor(BATTERY_POS_X+BATTERY_ICON_WIDTH/2, BATTERY_POS_Y + BATTERY_ICON_HEIGHT+1);
	gfx_puts(buffer);
	
	if(flag_low_battery){
		gfx_setTextSize(1);
		gfx_setTextColor(LCD_RED, LCD_WHITE);
		gfx_setCursor(BATTERY_POS_X, BATTERY_POS_Y + BATTERY_ICON_HEIGHT+10);
		sprintf(buffer, "Low");
		gfx_puts(buffer);
	}
	
	// Mostrar datos
	lcd_show_frame();
}

void read_xyz(int16_t * axis_matriz){
	int16_t current_read;
	/*axis_matriz[0] = 1;
	axis_matriz[1] = 2;
	axis_matriz[2] = 3;
	*/	
	read_register_8bits(WHO_AM_I | 0x80);                 // Lee el registro WHO_AM_I
	read_register_8bits(STATUS_REG | READ_BIT_MASk);            // Lee el registro STATUS_REG
    read_register_8bits(OUT_TEMP | READ_BIT_MASk);              // Lee el registro OUT_TEMP
    
	//read X axis
	current_read = read_register_8bits(OUT_X_L | READ_BIT_MASk);
	current_read |= read_register_8bits(OUT_X_H | READ_BIT_MASk) << MSB_SHIFT;
	axis_matriz[0] = current_read*SENSITIVITY_FACTOR_500DPS;
	
	//read Y axis
	current_read = read_register_8bits(OUT_Y_L | READ_BIT_MASk);
	current_read |= read_register_8bits(OUT_Y_H | READ_BIT_MASk) << MSB_SHIFT;
	axis_matriz[1] = current_read*SENSITIVITY_FACTOR_500DPS;
	
	//read Z axis
	current_read = read_register_8bits(OUT_Z_L | READ_BIT_MASk);
	current_read |= read_register_8bits(OUT_Z_H | READ_BIT_MASk) << MSB_SHIFT;
	axis_matriz[2] = current_read*SENSITIVITY_FACTOR_500DPS;
}

int main(void){
	int i;
	int bounce_time = 3; // tiempo de rebote del boton
	int16_t axis_matriz [3]; // Matriz de enteros con los datos leidos por el giroscopio
	float battery_voltage = 0; // voltaje medido
	global_setup();
	gpio_clear(GPIOG, GPIO13 | GPIO14);
	/* Loop principal*/
	while (1) {
		read_xyz(axis_matriz);
		display(axis_matriz, battery_voltage);
		
		// Falta agregar funcion que lea el voltaje
		/* Al presionar el boton, activa el modo USART y aumenta el voltaje para probar */
		if (gpio_get(GPIOA, GPIO0) && bounce_time==0) {
			flag_usart_active = !flag_usart_active; // toggle usart mode
			bounce_time = 3;
			battery_voltage+=0.9; // Nivel de voltaje provisional
		}

		for (i = 0; i < 3000000; i++) {		/* Delay */
			__asm__("nop");
		}
		// Blink Red LED until timeout
		if(bounce_time>0){
			bounce_time--;
		}
		// El warning debe activarse al hacer una lectura > a 7V
		if(limit_reaching_warning){
			gpio_toggle(GPIOG, GPIO14);
		}
		// Si esta activo el modo USART, parpadea el LED verde
		if(flag_usart_active){
			gpio_toggle(GPIOG, GPIO13);
		}
		
	}

	return 0;
}
