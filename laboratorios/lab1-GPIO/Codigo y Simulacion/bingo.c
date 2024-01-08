#include <pic14/pic12f675.h>
#include <stdlib.h>
#include <stdio.h>

//Macros
#define PUSH_B GP3
#define TOTAL_NUM 10

void setup(){
	TRISIO = 0x00; // Todos los pines quedan como salidas, el bit[3] siempre lee 1.
	GPIO = 0x00;
	typedef unsigned int word;
	word __at 0x207 __CONFIG = (_WDTE_OFF & _WDT_OFF & _MCLRE_OFF);

}

void delay(unsigned int time){
	unsigned int i;
	unsigned int j;
	for(i=0;i<time;(i)++)
	for(j = 0; j<200; (j)++);
}

void main(){
	setup();
	unsigned char index = 0;
	unsigned int time = 10; // con 10 obtengo 60hz de frecuencia
	while(1){
		GP0 = ~GP0;
		//index++;
		delay(time);
	}
}
