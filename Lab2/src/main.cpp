#include <Arduino.h>

#define LED_R 6
#define LED_G 5
#define LED_B 3
#define BUTTON_R 2
#define BUTTON_G 4

void setup1()
{
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(BUTTON_G, INPUT_PULLUP);
}

bool isLedOn = false;
int currentColor = 0;
void loop1()
{
  if (digitalRead(BUTTON_R) == LOW)
  {
    if (isLedOn)
    {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
      currentColor = 0;
    }
    else
    {
      digitalWrite(LED_R, HIGH);
      currentColor = LED_R;
    }

    isLedOn = !isLedOn;
  }

  if (digitalRead(BUTTON_G) == LOW)
  {
    if (currentColor == LED_R || currentColor == 0)
    {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, HIGH);
      currentColor = LED_G;
    }
    else if (currentColor == LED_G)
    {
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, HIGH);
      currentColor = LED_B;
    }
    else
    {
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_R, HIGH);
      currentColor = LED_R;
    }
  }
}

void setup2()
{
  setup1();
  analogWrite(LED_R, 100);
}

int currentBrigthness = 100;
void loop2()
{
  if (digitalRead(BUTTON_R) == LOW)
  {
    analogWrite(LED_R, max(currentBrigthness--, 0));
    delay(20);
  }

  if (digitalRead(BUTTON_G) == LOW)
  {
    analogWrite(LED_R, min(currentBrigthness++, 255));
    delay(20);
  }
}

void setup3()
{
  setup1();
  analogWrite(LED_R, 255);
}

void increaseColor(int color, int delayTime)
{
  for (int i = 0; i < 255; i++)
  {
    analogWrite(color, i);
    delay(delayTime);
  }
}

void decreaseColor(int color, int delayTime)
{
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(color, i);
    delay(delayTime);
  }
}

int delayTime = 10;
void loop3()
{
  increaseColor(LED_G, delayTime);
  decreaseColor(LED_R, delayTime);
  increaseColor(LED_B, delayTime);
  decreaseColor(LED_G, delayTime);
  increaseColor(LED_R, delayTime);
  decreaseColor(LED_B, delayTime);
}

void setup()
{
  // setup1();
  // setup2();
  setup3();
}

void loop()
{
  // loop1();
  // loop2();
  loop3();
}