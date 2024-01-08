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
	for(j = 0; j<160; (j)++);
}

void print_value(unsigned char index){
	// Ya que el pin GP3 siempre lee 1, entonces
	/* Las combinaciones son:
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
	
	switch (index) {
		GPIO &= 0x00;
		case 0:
			
			GPIO =0x08;
		    break;
		case 1:
			//GPIO &= 0x00;
			GPIO =0x09;
		    break;
        case 2:
        	//GPIO &= 0x00;
		    GPIO =0x0A;
		    break;
        case 3:
        	//GPIO &= 0x00;
		    GPIO =0x0B;
		    break;
        case 4:
		    GPIO =0x0C;
		    break;
        case 5:
		    GPIO =0x0D;
		    break;
        case 6:
		    GPIO =0x0E;
		    break;
        case 7:
		    GPIO =0x0F;
		    break;
        case 8:
		    GPIO =0x18;
		    break;
        case 9:
		    GPIO =0x19;
		    break;
        default:
            // Código a ejecutar si opcion no coincide con ningún caso
            break;
    }
}

void main(){
	setup();
	unsigned char index = 0; // Index para el array de 10 numeros
	unsigned int time = 10; // con 10 obtengo 60hz de frecuencia
	unsigned int valor_rebote = 0;
	while(1){
		if(!PUSH_B && (valor_rebote == 0)){
			print_value(index);
			valor_rebote = 100;
			index++;
		}
		if(valor_rebote > 0){
			valor_rebote--;
		}
		if(index > 20){
			GPIO = 0x1F;
			//GPIO &= 19;
		}
			GP5 = ~GP5;
		delay(time);
	}
}
