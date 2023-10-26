#include <Arduino.h>

void blink(int pin, int delay_ms)
{
  digitalWrite(pin, HIGH);
  delay(delay_ms);
  digitalWrite(pin, LOW);
  delay(delay_ms);
}

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
  int longSig = 3000;
  int shortSig = 100;

  blink(LED_BUILTIN, longSig);
  blink(LED_BUILTIN, longSig);
  blink(LED_BUILTIN, longSig);
  blink(LED_BUILTIN, shortSig);
  blink(LED_BUILTIN, shortSig);
  blink(LED_BUILTIN, shortSig);
  blink(LED_BUILTIN, longSig);
  blink(LED_BUILTIN, longSig);
  blink(LED_BUILTIN, longSig);
}
