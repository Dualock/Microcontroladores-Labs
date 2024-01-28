#define MAX_READ 4.73
#define MIN_READ 0.04
#define MAX_VOLT 24
#define MIN_VOLT -24
#define MAX_ARDUINOV 5
#define RESOLUTION 1023
#define DIVIDER_FACTOR 10
#define ADDER_VALUE 2.4
#define LOSS_FACTOR 1.02
int voltage1 = A3;
int voltage2 = A2;
int voltage3 = A1;
int voltage4 = A0;

void setup() {
  Serial.begin(9600);
  pinMode(voltage1, INPUT);
}
float read_voltage(int pin){
  float ADC_value = analogRead(pin);
  float voltage = ADC_value*MAX_ARDUINOV/RESOLUTION;
  voltage = (voltage - ADDER_VALUE)*(DIVIDER_FACTOR)*LOSS_FACTOR;
  //voltage = round(voltage);
  return voltage;
}

void print_voltage(float voltage, int ch_number){
  Serial.print("voltage");
  Serial.print(ch_number);
  Serial.print(":");
  Serial.print(voltage);
  Serial.print(" \n");
  }

void loop() {
  float voltage = read_voltage(voltage1);
  print_voltage(voltage, 1);
  voltage = read_voltage(voltage2);
  print_voltage(voltage, 2);
  voltage = read_voltage(voltage3);
  print_voltage(voltage, 3);
  voltage = read_voltage(voltage4);
  print_voltage(voltage, 4);
  delay(10000);
    

}
