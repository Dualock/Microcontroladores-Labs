#include "Arduino.h"
#include "state_machine.h"

void blink(int RED_LED)
{
 digitalWrite(RED_LED, HIGH);
 delay(500);
 digitalWrite(RED_LED, LOW);
 delay(500);
}
