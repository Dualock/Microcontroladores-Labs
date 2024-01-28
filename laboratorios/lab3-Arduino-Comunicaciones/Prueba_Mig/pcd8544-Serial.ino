#include <PCD8544.h>
#include <math.h>
#include <SoftwareSerial.h>





#define SERIAL_PIN 12 // Pin de entrada a verificar (en este caso, pin 12)
// A custom glyph (a smiley)...
static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };


static PCD8544 lcd;


void setup() {
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

  // Add the smiley to position "0" of the ASCII table...
  lcd.createChar(0, glyph);




  Serial.begin(9600); // Inicializa la comunicación serial a 9600baudios
}

void printVoltageLCD(float voltage1, float voltage2, float voltage3, float voltage4){
  // Un buffer de caracteres para almacenar el flotante como string
  char voltage_char_buffer1[3];
  char voltage_char_buffer2[3];
  char voltage_char_buffer3[3];
  char voltage_char_buffer4[3];
  // 1 enteros, 1 decimales
  dtostrf(voltage1, 1, 1, voltage_char_buffer1);
  
  
  

  // Texto inicial
  lcd.setCursor(0, 0);
  lcd.print("voltmeter 4 ch");

  // Escribir voltajes 1 y 2 en Segunda linea
  lcd.setCursor(0, 1);
  lcd.print("V1");
  lcd.write(' ');
  lcd.print(voltage_char_buffer1);
  
  
  lcd.setCursor(0, 2);
  dtostrf(voltage2, 1, 1, voltage_char_buffer2);
  //lcd.write(' ');
  lcd.print("V2");
  lcd.write(' ');
  lcd.print(voltage_char_buffer2);

  // Escribir voltajes 3 y 4 en Segunda linea
  
  lcd.setCursor(0, 3);
  dtostrf(voltage3, 1, 1, voltage_char_buffer3);
  lcd.print("V3");
  lcd.write(' ');
  lcd.print(voltage_char_buffer3);
  
  lcd.setCursor(0, 4);
  dtostrf(voltage4, 1, 1, voltage_char_buffer4);
  //lcd.write(' ');
  lcd.print("V4");
  lcd.write(' ');
  lcd.print(voltage_char_buffer4);

}
  
void loop() {
  static float voltage1 = 11.1;
  float voltage2 = 22.2;
  float voltage3 = 33.3;
  float voltage4 = 44.4;
  printVoltageLCD(voltage1, voltage2, voltage3, voltage4);
  
  voltage1++;





if (digitalRead(SERIAL_PIN) == HIGH) { // Cuando el pin 12 está en alto se realiza la impresion serial
    for (int i = 0; i < 4; i++) {
      Serial.print("Voltaje");
      Serial.print(i + 1);
      Serial.print("->");
      if (i==0){
      Serial.print(voltage1);}
      if (i==1){
      Serial.print(voltage2);}
      if (i==2){
      Serial.print(voltage3);}
      if (i==3){
      Serial.print(voltage4);}
      Serial.println("V");
    }
}
delay(500);
}
