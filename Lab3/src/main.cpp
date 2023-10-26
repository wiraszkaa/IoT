#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initRGB()
{
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void initButtons()
{
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void setup1()
{
  digitalWrite(LED_RED, HIGH);
}

void setup2()
{
  setup1();
}

void setup3()
{
  lcd.init();
  lcd.backlight();

  lcd.print(0);
  lcd.setCursor(0, 0);
}

bool isGreenButtonPressed()
{
  static int debounced_button_state = HIGH;
  static int previous_reading = HIGH;
  static unsigned long last_change_time = 0UL;
  bool isPressed = false;

  int current_reading = digitalRead(GREEN_BUTTON);

  if (previous_reading != current_reading)
  {
    last_change_time = millis();
  }

  if (millis() - last_change_time > DEBOUNCE_PERIOD)
  {
    if (current_reading != debounced_button_state)
    {
      if (debounced_button_state == HIGH && current_reading == LOW)
      {
        isPressed = true;
      }
      debounced_button_state = current_reading;
    }
  }

  previous_reading = current_reading;

  return isPressed;
}

bool isRedButtonPressed()
{
  static int debounced_button_state = HIGH;
  static int previous_reading = HIGH;
  static unsigned long last_change_time = 0UL;
  bool isPressed = false;

  int current_reading = digitalRead(RED_BUTTON);

  if (previous_reading != current_reading)
  {
    last_change_time = millis();
  }

  if (millis() - last_change_time > DEBOUNCE_PERIOD)
  {
    if (current_reading != debounced_button_state)
    {
      if (debounced_button_state == HIGH && current_reading == LOW)
      {
        isPressed = true;
      }
      debounced_button_state = current_reading;
    }
  }

  previous_reading = current_reading;

  return isPressed;
}

int led[] = {LED_RED, LED_GREEN, LED_BLUE};
int index = 0;
void loop1()
{
  if (isGreenButtonPressed())
  {
    digitalWrite(led[index], LOW);
    index = (index + 1) % 3;
    digitalWrite(led[index], HIGH);
  }
  if (isRedButtonPressed())
  {
    digitalWrite(led[index], LOW);
    index = (index + 1) % 3;
    digitalWrite(led[index], HIGH);
  }
}

void blinkRedLed()
{
  const unsigned long BlinkChangePeriod = 900UL;
  static int ledState = LOW;

  static unsigned long lastBlinkChange = 0UL;
  if (millis() - lastBlinkChange >= BlinkChangePeriod)
  {
    if (ledState == HIGH)
    {
      ledState = LOW;
    }
    else
    {
      ledState = HIGH;
    }

    digitalWrite(LED_RED, ledState);
    lastBlinkChange += BlinkChangePeriod;
  }
}

void blinkGreenLed()
{
  const unsigned long BlinkChangePeriod = 1000UL;
  static int ledState = LOW;

  static unsigned long lastBlinkChange = 0UL;
  if (millis() - lastBlinkChange >= BlinkChangePeriod)
  {
    if (ledState == HIGH)
    {
      ledState = LOW;
    }
    else
    {
      ledState = HIGH;
    }

    digitalWrite(LED_GREEN, ledState);
    lastBlinkChange += BlinkChangePeriod;
  }
}

void blinkBlueLed()
{
  const unsigned long BlinkChangePeriod = 1100UL;
  static int ledState = LOW;

  static unsigned long lastBlinkChange = 0UL;
  if (millis() - lastBlinkChange >= BlinkChangePeriod)
  {
    if (ledState == HIGH)
    {
      ledState = LOW;
    }
    else
    {
      ledState = HIGH;
    }

    digitalWrite(LED_BLUE, ledState);
    lastBlinkChange += BlinkChangePeriod;
  }
}

void loop2()
{
  blinkRedLed();
  blinkGreenLed();
  blinkBlueLed();
}

bool working = false;
int seconds = 0;
void measureTime()
{
  const unsigned long timeInterval = 1000UL;
  static unsigned long lastTime = 0UL;

  if (millis() - lastTime >= timeInterval)
  {
    if (working)
    {
      seconds++;
    }

    lastTime += timeInterval;
  }
}

void resetTimer()
{
  seconds = 0;
  working = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(0);
}

void loop3()
{
  measureTime();
  lcd.setCursor(0, 0);
  lcd.print(seconds);

  if (isGreenButtonPressed())
  {
    working = !working;
  }

  if (isRedButtonPressed())
  {
    resetTimer();
    working = false;
  }
}

void setup()
{
  initRGB();
  initButtons();
  setup1();
  // setup2();
  // setup3();
}

void loop()
{
  loop1();
  // loop2();
  // loop3();
}
