#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LedController.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define BUTTON_PIN_RED 2

#define DEBOUNCE_INTERVAL 10UL

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(A1);
DallasTemperature tempSensors(&oneWire);

void setup1()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  tempSensors.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

bool isRedBtnPressed()
{
  static int stableState = HIGH;
  static int previousState = HIGH;
  static unsigned long debounceTimer = 0UL;
  bool pressed = false;

  int currentButtonState = digitalRead(BUTTON_PIN_RED);

  if (previousState != currentButtonState)
  {
    debounceTimer = millis();
  }

  if ((millis() - debounceTimer) > DEBOUNCE_INTERVAL)
  {
    if (currentButtonState != stableState)
    {
      if (stableState == HIGH && currentButtonState == LOW)
      {
        pressed = true;
      }
      stableState = currentButtonState;
    }
  }

  previousState = currentButtonState;

  return pressed;
}

float minTemp = 10000;
float maxTemp = -10000;
bool isMinTemp = false;
void loop1()
{
  tempSensors.requestTemperatures();
  float tempIn = tempSensors.getTempCByIndex(1);
  float tempOut = tempSensors.getTempCByIndex(0);

  if (tempOut < minTemp)
  {
    minTemp = tempOut;
  }

  if (tempOut > maxTemp)
  {
    maxTemp = tempOut;
  }

  if (tempOut < 10)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
  }
  else if (tempOut < 25)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
  }
  else
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  }

  if (isRedBtnPressed())
  {
    isMinTemp = !isMinTemp;
  }

  if (isMinTemp)
  {
    char buffer[40];
    sprintf(buffer, "MIN OUT:%4sC", String(minTemp, 1).c_str());
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "MAX OUT:%4sC", String(maxTemp, 1).c_str());
    lcd.setCursor(0, 1);
    lcd.print(buffer);
  }
  else
  {
    char buffer[40];
    sprintf(buffer, "IN:%9sC", String(tempIn, 1).c_str());
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "OUT:%8sC", String(tempOut, 1).c_str());
    lcd.setCursor(0, 1);
    lcd.print(buffer);
  }
}

LedController led = LedController();
void setup2()
{
  led.init(LED_RED, LED_GREEN, LED_BLUE);
}

void loop2()
{
  led.colorOn(GREEN);
  delay(100);
  led.colorOn(BLUE);
  delay(100);
  led.colorOn(MAGENTA);
}

void setup()
{
  // setup1();
  setup2();
}

void loop()
{
  // loop1();
  loop2();
}