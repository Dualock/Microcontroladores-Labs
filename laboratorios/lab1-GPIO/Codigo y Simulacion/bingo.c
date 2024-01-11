#include <pic14/pic12f675.h>
#include <stdlib.h>
#include <stdio.h>

//Macros
#define PUSH_B GP3
#define CONMUTADOR GP5
#define TOTAL_NUM 10
#define PANTALLA_FINAL 9
#define APAGAR 15


/** Funcion setup: Establece los valores de inicio de los registros
*/
void setup(){
	TRISIO = 0x00; // Todos los pines quedan como salidas, el bit[3] siempre lee 1.
	GPIO = 0x00;
	// se desactiva el watchdog timer y el MCLR
	typedef unsigned int word;
	word __at 0x207 __CONFIG = (_WDTE_OFF & _WDT_OFF & _MCLRE_OFF);

}

/** Funcion clean_array:
*	Limpia el arreglo donde se guardan los tiros de la gondola
*	Parametros:
*		- *array: Arreglo con los 10 tiros de la gondola
*		- length: Longitud del arreglo
*/
void clean_array(unsigned char *array, unsigned char length){
	for (int i = 0; i < length; i++) {
		array[i] = 0;
	}
}

/** Funcion delay: Agrega tiempo de retardo al sistema
*	Parametros:
*		- time: numero de repeticiones del ciclo externo
*/
void delay(unsigned int time){
	unsigned int i;
	unsigned int j;
	for(i=0;i<time;(i)++)
	for(j = 0; j<320; (j)++);
}
/* 	Funcion lfsr: Implementación de un linear feedback shift register de 8 bits
*	para generar numeros semi aleatorios a partir de una semilla
*	Parametros:
*		- num: numero decimal, manejado como binario
*	Retorna: num%100
*/
unsigned char lfsr(unsigned char num) {
    // El polinomio utilizado es x^7 + x^5 + x^4 + 1
    unsigned char feedback = (num >> 7) ^ (num >> 5) ^ (num >> 4) ^ 1;
    num = (num << 1) | feedback; //corrimiento hacia la derecha
    // Se retorna el numero modulo de 100 para que el resultado este entre 0 y 100
    return num%100;
}


/**	Funcion map_BCD:
*	Mapea los numeros a sus respectivas combinaciones de GPIO en BCD
*	Parametros:
*		-num: numero decimal
*/
void map_BCD(unsigned char num){
	GPIO &= 0x28; // Se limpian los bits del registro excepto GP5 y GP3

/* Las combinaciones son :
	GP4	GP3(X)	GP2	GP1	GP0		BCD		hex
	0	1		0	0	0	=	0		0x28
	0	1		0	0	1	=	1		0x29
	0	1		0	1	0	=	2		0x2A
	0	1		0	1	1	=	3		0x2B
	0	1		1	0	0	=	4		0x2C
	0	1		1	0	1	=	5		0x2D
	0	1		1	1	1	=	6		0x2E
	0	1		1	1	1	=	7		0x3F
	1	1		0	0	0	=	8		0x38
	1	1		0	0	1	=	9		0x39
*/
	switch (num) {
		case 0:
			GPIO |=0x28;
		    break;
		case 1:
			GPIO |=0x29;
		    break;
        case 2:
		    GPIO |=0x2A;
		    break;
        case 3:
		    GPIO |=0x2B;
		    break;
        case 4:
		    GPIO |=0x2C;
		    break;
        case 5:
		    GPIO |=0x2D;
		    break;
        case 6:
		    GPIO |=0x2E;
		    break;
        case 7:
		    GPIO |=0x2F;
		    break;
        case 8:
		    GPIO |=0x38;
		    break;
        case 9:
		    GPIO |=0x39;
		    break;
        default:
        	// Una F en la salida por la forma en que esta conectada la logica, apaga los LED
            GPIO |=0x3F; // En caso de no haber entradas validas, pondra una F en la salida
            break;
    }
}

/** Funcion check_num: revisa si el numero ya salio y guarda los numeros que no estan repetidos
*	Parametros:
		- *array: Arreglo con los 10 tiros de la gondola
*		- num: numero decimal
*		- index: indice actual del array
*	Retorna: index
*/	
unsigned char check_num(unsigned char* array, unsigned char index, unsigned char numero){
	for (int i = 0; i <= index; i++) {
        if(array[i] == numero){
        
		// El numero existe en el array, se retorna el mismo indice
        	return index;
        }
	}
	/** Si sale del loop, entonces el numero no esta en el array
    Se guarda y se incrementa el indice en 1
    */
	array[index] = numero;
	index+=1;
	return index;
}

/** Funcion multiplexar: multiplexa los LEDs
*	Parametros:
		- decenas: Numero izquierdo
*		- unidades: Numero derecho
*		- time: Numero de repeticiones del ciclo externo
*/	
void multiplexar(unsigned char decenas, unsigned char unidades, unsigned int time){

	// Se multiplexa la salida GP5 y las decenas y unidades para mostrar el numero completo
	map_BCD(decenas); // Se escriben las unidades
	CONMUTADOR = ~CONMUTADOR;
	delay(time);
	map_BCD(unidades); // Se escriben las decenas
	delay(time);
	
}

/** Funcion main: funcion principal
*/
void main(){
	// Configuracion e inicializacion de variables
	setup();
	unsigned char index = 0; // Index para el array de 10 numeros
	unsigned int time = 5; // con 5 obtengo 60hz de frecuencia
	unsigned int valor_rebote = 30; // Valor inicial de rebote alto para darle tiempo de encender
	unsigned char tiros [TOTAL_NUM]; // Array donde se guardan los numeros que ya salieron
	unsigned char numero = 0x41; //seed
	unsigned char longitud = sizeof(tiros) / sizeof(tiros[0]);
	
	/* La idea es que al inicio se mapeen numeros no validos, y por defecto se pondra un FF
	El FF por la logica combinacional implementada, va a apagar los LEDs*/ 
	unsigned char unidades = APAGAR;
	unsigned char decenas = APAGAR;
	
	// Loop principal
	while(1){
		if(index > TOTAL_NUM){
		// Aqui se maneja la logica de la pantalla de final de juego
			unsigned char i;
			delay(time);
			for (i = 0; i<3; i++){
			// Los numeros magicos de los tiempos se calcularon de forma empirica
				multiplexar(PANTALLA_FINAL, PANTALLA_FINAL, 4*time);
				multiplexar(APAGAR, APAGAR, time);
				delay(50*time);
			}
			// Se resetean los valores para volver a empezar
			index = 0;
			clean_array(tiros, longitud);
			unidades = APAGAR;
			decenas = APAGAR;
		}
		/*	Entra en este else if cuando se pulsa el boton, esta configurado con resistencia
			de pull up
		*/
		else if(!PUSH_B && (valor_rebote == 0)){
			valor_rebote = 30;
			numero = lfsr(numero);
			decenas = numero/10;
			unidades = numero%10;
			index = check_num(tiros, index, numero);
		}
		// logica para el debouncing
		else if(valor_rebote > 0){
			valor_rebote--;
		}
		// logica para mantener los LED mostrando el numero actual
		if(index <= TOTAL_NUM){
			multiplexar(decenas, unidades, time);
		}
	}
}
