int led_pin = 13;
int in_pin = 7;
int val = 0;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(in_pin, INPUT);
}

void loop() {
  val = digitalRead(inPin);
  digitalWrite(led_pin, val);
}
