#include <stdio.h>
#include <stdlib.h>
#define TOTAL_BOLAS 10
// Implementación de un LFSR de 8 bits
unsigned char lfsr(unsigned char num) {
    // El polinomio utilizado es x^8 + x^6 + x^5 + x^4 + 1
    unsigned char feedback = (num >> 7) ^ (num >> 5) ^ (num >> 4) ^ 1;
    num = (num << 1) | feedback;
    return num%100;
}
unsigned char check_num(unsigned char* array, unsigned char counter, unsigned char numero){
	for (int i = 0; i <= counter; i++) {
		printf("iterando %d \n", i);
        if(array[i] == numero){
        	printf("el numero: %d ya existe \n", numero);
        	return counter;
        }
	}
    // Si sale del loop, entonces el numero no esta en el array
	printf("Insertando: %d", numero);
	array[counter] = numero;
	counter+=1;
	return counter;
}

void recorrer_array(unsigned char* array, unsigned char longitud){
	for (int i = 0; i < longitud; i++) {
        printf("Elemento %d: %d\n", i, array[i]);
    }
}

int main(){
	unsigned char tiros [TOTAL_BOLAS];
	unsigned char numero = 0x41; //seed
	unsigned char counter = 0;
	unsigned char longitud = sizeof(tiros) / sizeof(tiros[0]);
	
    while(1){
    	if (TOTAL_BOLAS == counter){
    		break;
    	}
		numero = lfsr(numero);
		counter = check_num(tiros, counter, numero);
		printf("counter %d \n", counter);
    }
    recorrer_array(tiros, longitud);
    return 0;
}
