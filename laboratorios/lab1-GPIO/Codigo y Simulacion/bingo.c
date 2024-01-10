#include <pic14/pic12f675.h>
#include <stdlib.h>
#include <stdio.h>

//Macros
#define PUSH_B GP3
#define CONMUTADOR GP5
#define TOTAL_NUM 10

void setup(){
	TRISIO = 0x00; // Todos los pines quedan como salidas, el bit[3] siempre lee 1.
	GPIO = 0x00;
	// se desactiva el watchdog timer y el MCLR
	typedef unsigned int word;
	word __at 0x207 __CONFIG = (_WDTE_OFF & _WDT_OFF & _MCLRE_OFF);

}

void clean_array(unsigned char *array, unsigned char length){
	for (int i = 0; i < length; i++) {
		array[i] = 0;
	}
}
void delay(unsigned int time){
	unsigned int i;
	unsigned int j;
	for(i=0;i<time;(i)++)
	for(j = 0; j<320; (j)++);
}
// Implementación de un LFSR de 8 bits
unsigned char lfsr(unsigned char num) {
    // El polinomio utilizado es x^8 + x^6 + x^5 + x^4 + 1
    unsigned char feedback = (num >> 7) ^ (num >> 5) ^ (num >> 4) ^ 1;
    num = (num << 1) | feedback;
    // Se retorna el numero modulo de 100 para que el resultado este entre 0 y 100
    return num%100;
}

/** Ya que el pin GP3 siempre lee 1, entonces
 	Las combinaciones son :
	GP4	GP3(X)	GP2	GP1	GP0		BCD		hex
	0	1		0	0	0	=	0		0x08
	0	1		0	0	1	=	1		0x09
	0	1		0	1	0	=	2		0x0A
	0	1		0	1	1	=	3		0x0B
	0	1		1	0	0	=	4		0x0C
	0	1		1	0	1	=	5		0x0D
	0	1		1	1	1	=	6		0x0E
	0	1		1	1	1	=	7		0x0F
	1	1		0	0	0	=	8		0x18
	1	1		0	0	1	=	9		0x19
*/
void map_BCD(unsigned char num){
	GPIO &= 0x28; // Se limpian los bits del registro excepto GP5 y GP3
	// 0x28 = 0010 1000
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
            // Código a ejecutar si opcion no coincide con ningún caso
            break;
    }
}

/** check_num revisa si el numero ya salio, si no ha salido, lo guarda y retorna index + 1, 
	pero si ya salio, retorna index
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

void main(){
	// Configuracion e inicializacion de variables
	setup();
	unsigned char index = 0; // Index para el array de 10 numeros
	unsigned int time = 5; // con 5 obtengo 60hz de frecuencia
	unsigned int valor_rebote = 30;
	unsigned char tiros [TOTAL_NUM]; // Array donde se guardan los numeros que ya salieron
	unsigned char numero = 0x41; //seed
	unsigned char longitud = sizeof(tiros) / sizeof(tiros[0]);
	unsigned char unidades = 0;
	unsigned char decenas = 0;
	
	
	// Loop principal
	while(1){
		if(index >= TOTAL_NUM){
			unsigned char i;
			delay(time);
			for (i = 0; i<100; i++){
				map_BCD(9);
				CONMUTADOR = ~CONMUTADOR;
				delay(time);
				map_BCD(9);
				delay(time);
			}
			index = 0;
			clean_array(tiros, longitud);
		}
		else if(!PUSH_B && (valor_rebote == 0)){
			valor_rebote = 30;
			numero = lfsr(numero);
			decenas = numero/10;
			unidades = numero%10;
			index = check_num(tiros, index, numero);
		}
		else if(valor_rebote > 0){
			valor_rebote--;
		}
		if(index < TOTAL_NUM){
			// Se multiplexa la salida GP5 y las decenas y unidades para mostrar el numero completo
			map_BCD(decenas); // Se escriben las unidades
			CONMUTADOR = ~CONMUTADOR;
			delay(time);
			map_BCD(unidades); // Se escriben las decenas
			delay(time);
		}
	}
}
