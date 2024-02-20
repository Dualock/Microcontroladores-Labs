#ifndef COMPUERTA_H
#define COMPUERTA_H
#pragma once

#include <Servo.h>
#include "Arduino.h"

#define OPEN_VALUE 0
#define CLOSED_VALUE 180

// Prototipos de funciones
int closeGate(Servo &myservo, int current_pos);
int openGate(Servo &myservo, int current_pos);
int hardWriteValue(Servo &myservo, int current_pos, int val);

int closeGate(Servo &myservo, int current_pos)
{
  while(current_pos < CLOSED_VALUE){
    //Serial.print("Cerrando: ");
    //Serial.println(current_pos);
    myservo.write(current_pos);
    current_pos++;
    delay(10);
    }
    return current_pos;
}

int openGate(Servo &myservo, int current_pos)
{
  while(current_pos > OPEN_VALUE){
    //Serial.print("Abriendo: ");
    //Serial.println(current_pos);
    myservo.write(current_pos);
    current_pos--;
    delay(10);
    }
    return current_pos;
}

int hardWriteValue(Servo &myservo, int current_pos, int val){
    while(current_pos != val){
    myservo.write(current_pos);
    if(val > current_pos){
      current_pos++;
      }
    else if(val < current_pos){
      current_pos--;
      }
    
    delay(20);
    }
    return current_pos;
}
#endif
