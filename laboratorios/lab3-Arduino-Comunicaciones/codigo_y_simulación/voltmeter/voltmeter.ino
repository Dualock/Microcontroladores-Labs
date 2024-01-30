#include <PCD8544.h>
#include <math.h>
#include <SoftwareSerial.h>
static PCD8544 lcd;
// Macros

#define MAX_READ 4.73
#define MIN_READ 0.04
#define MAX_VOLT 24
#define MIN_VOLT -24
#define MAX_ARDUINOV 5
#define RESOLUTION 1023
#define DIVIDER_FACTOR 10
#define ADDER_VALUE 2.4
#define LOSS_FACTOR 1.02
#define RMS_FACTOR 0.7071
#define CANTIDAD_CANALES 4


#define SERIAL_PIN 8 // Pin de sw the inicio transmicion serial (en este caso, pin 8)


int voltage1 = A3;
int voltage2 = A2;
int voltage3 = A1;
int voltage4 = A0;
int ac_button = 2;
int led_ac = 9;
int warning_led = 10;
float ac_peak [4];
bool ac_mode = false;


//*********************** Setup Function *******************************
void setup() {
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);
  // Serial baud rate
  Serial.begin(9600);
  // Fill array with 0s
  memset(ac_peak,0,sizeof(ac_peak));
  // GPIO
  pinMode(voltage1, INPUT);
  pinMode(voltage2, INPUT);
  pinMode(voltage3, INPUT);
  pinMode(voltage4, INPUT);
  pinMode(ac_button, INPUT);
  pinMode(led_ac, OUTPUT);
  pinMode(warning_led, OUTPUT);
  // pin 2 external interrupt
  attachInterrupt(digitalPinToInterrupt(ac_button), interruptSR, FALLING);
}


//********************* Interrupt Service Routine ************************
void interruptSR(){
  if(!ac_mode){
      Serial.println("AC Mode activated!");
      digitalWrite(led_ac, HIGH);
      ac_mode = true;
    }
  else{
    Serial.println("DC Mode activated!");
    digitalWrite(led_ac, LOW);
    ac_mode = false;
  }
}


//********************* Read Analog Voltages *****************************
float readVoltage(int pin){
  float ADC_value = analogRead(pin);
  float voltage = ADC_value*MAX_ARDUINOV/RESOLUTION;
  voltage = (voltage - ADDER_VALUE)*(DIVIDER_FACTOR)*LOSS_FACTOR;
  //voltage = round(voltage);
  return voltage;

}


//********************* Find Vpeak on AC voltages ***********************
float acPeakFinder(float peak, float voltage){
  if(voltage > peak){
      return voltage;
    }
    else{
      return peak;
      }
  }

//********************* Print voltages serial mode **********************
void printVoltage(float voltage, int ch_number){
  Serial.print("voltage");
  Serial.print(ch_number);
  Serial.print(":");
  Serial.print(voltage);
  Serial.print(" \n");
  }


//********************* Display Text on PCD8544 *************************
void displayVoltageLCD(float voltage1, float voltage2, float voltage3, float voltage4){
  // Un buffer de caracteres para almacenar el flotante como string
  char voltage_char_buffer1[3];
  char voltage_char_buffer2[3];
  char voltage_char_buffer3[3];
  char voltage_char_buffer4[3];
  // 1 enteros, 1 decimales
  dtostrf(voltage1, 1, 1, voltage_char_buffer1);
  
  // Initial text
  lcd.setCursor(0, 0);
  lcd.print("voltmeter 4 ch");

  //Write V1 2nd line
  lcd.setCursor(0, 1);
  lcd.print("V1");
  lcd.write(' ');
  lcd.print(voltage_char_buffer1);
  
  // Write V2 3th line
  lcd.setCursor(0, 2);
  dtostrf(voltage2, 1, 1, voltage_char_buffer2);
  //lcd.write(' ');
  lcd.print("V2");
  lcd.write(' ');
  lcd.print(voltage_char_buffer2);

  // Write V3 4th line
  
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

//************************ Main loop ****************************
void loop() {
  static float v[] = {0,0,0,0};
  // read voltages
  v[0] = readVoltage(voltage1);
  v[1] = readVoltage(voltage2);
  v[2] = readVoltage(voltage3);
  v[3] = readVoltage(voltage4);
  
  // check for near voltage limit warning
  for (int i = 0; i < CANTIDAD_CANALES; i++){
    if(v[i] > 20 || v[i] < -20){
      digitalWrite(warning_led, HIGH);
      }
    }
  if(!ac_mode){
    //print LCD
    displayVoltageLCD(v[0], v[1], v[2], v[3]);
    
    // print serial
    if (digitalRead(SERIAL_PIN) == HIGH) { // Cuando el pin 8 está en alto se realiza la impresion serial
      for (int i = 1; i < 5; i++) {
        Serial.print("Voltaje");
        Serial.print(' ');
        Serial.print(i);
        Serial.print("=");
        if (i==1){
          Serial.print(v[0]);}
        if (i==2){
          Serial.print(v[1]);}
        if (i==3){
          Serial.print(v[2]);}
        if (i==4){
          Serial.print(v[3]);}
        Serial.print(' ');
        Serial.println("V");
    }
}
    
    delay(1000);
    }
  else if(ac_mode && millis()%1000 == 0){
    for(int i = 0; i < CANTIDAD_CANALES; i++){
        ac_peak[i]*=0.7071;
      }
      
      // print LCD
      displayVoltageLCD(ac_peak[0], ac_peak[1], ac_peak[2], ac_peak[3]);
      
      // print serial
      if (digitalRead(SERIAL_PIN) == HIGH) { // Cuando el pin 12 está en alto se realiza la impresion serial
        for (int i = 1; i < 5; i++) {
          Serial.print("Voltaje");
          Serial.print(' ');
          Serial.print(i);
          Serial.print("=");
          if (i==1){
            Serial.print(ac_peak[0]);}
          if (i==2){
            Serial.print(ac_peak[1]);}
          if (i==3){
            Serial.print(ac_peak[2]);}
          if (i==4){
            Serial.print(ac_peak[3]);}
          Serial.print(' ');
          Serial.println("V");
    }
}
  }
  else if(ac_mode){
    ac_peak[0] = acPeakFinder(ac_peak[0], v[0]);
    ac_peak[1] = acPeakFinder(ac_peak[1], v[1]);
    ac_peak[2] = acPeakFinder(ac_peak[2], v[2]);
    ac_peak[3] = acPeakFinder(ac_peak[3], v[3]);
  }
}
