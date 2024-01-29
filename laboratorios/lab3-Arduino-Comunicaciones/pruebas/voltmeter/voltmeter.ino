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
int voltage1 = A3;
int voltage2 = A2;
int voltage3 = A1;
int voltage4 = A0;
float ac_peak [4];
float voltage = 0;

void setup() {
  Serial.begin(9600);
  memset(ac_peak,0,sizeof(ac_peak));
  pinMode(voltage1, INPUT);
  pinMode(voltage2, INPUT);
  pinMode(voltage3, INPUT);
  pinMode(voltage4, INPUT);
}
float readVoltage(int pin){
  float ADC_value = analogRead(pin);
  float voltage = ADC_value*MAX_ARDUINOV/RESOLUTION;
  voltage = (voltage - ADDER_VALUE)*(DIVIDER_FACTOR)*LOSS_FACTOR;
  //voltage = round(voltage);
  return voltage;

}
float acPeakFinder(float peak, float voltage){
  if(voltage > peak){
      return voltage;
    }
    else{
      return peak;
      }
  }
void printVoltage(float voltage, int ch_number){
  Serial.print("voltage");
  Serial.print(ch_number);
  Serial.print(":");
  Serial.print(voltage);
  Serial.print(" \n");
  }

void loop() {
  bool ac_mode = true;
  if(!ac_mode){
    voltage = readVoltage(voltage1);
    printVoltage(voltage, 1);
    voltage = readVoltage(voltage2);
    printVoltage(voltage, 2);
    voltage = readVoltage(voltage3);
    printVoltage(voltage, 3);
    voltage = readVoltage(voltage4);
    printVoltage(voltage, 4);
    delay(1000);
    }
  else if(ac_mode && millis()%1000 == 0){
    for(int i = 0; i<4; i++){
        ac_peak[i]*=0.7071;
        printVoltage(ac_peak[i], i+1);
      }
  }
  else if(ac_mode){
    voltage = readVoltage(voltage1);
    ac_peak[0] = acPeakFinder(ac_peak[0], voltage);
    voltage = readVoltage(voltage2);
    ac_peak[1] = acPeakFinder(ac_peak[1], voltage);
    voltage = readVoltage(voltage3);
    ac_peak[2] = acPeakFinder(ac_peak[2], voltage);
    voltage = readVoltage(voltage4);
    ac_peak[3] = acPeakFinder(ac_peak[3], voltage);
  }
    
  
  
    

}
