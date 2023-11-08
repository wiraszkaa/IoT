#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define POTENTIOMETER A0

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initButtons()
{
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void initRGB()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
}

void initLCD()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.print("ADC:     V:");
}

void setup1()
{
  Serial.begin(9600);
  while (!Serial)
  {
  }
  initButtons();
}
void setup2()
{
  setup1();
  initRGB();
}
void setup3()
{
  initLCD();
}

void setup4()
{
  setup1();
}

void setup()
{
  setup1();
  // setup2();
  // setup3();
  // setup4();
}

void loop1()
{
  String str = String(digitalRead(RED_BUTTON)) + "," + String(digitalRead(GREEN_BUTTON));
  Serial.println(str);
}

boolean isRedOn = false;
boolean isGreenOn = false;
boolean isBlueOn = false;
void loop2()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    if (command == "red")
    {
      if (isGreenOn || isBlueOn)
      {
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_GREEN, LOW);
        isGreenOn = false;
        isBlueOn = false;
      }
      isRedOn = true;
      digitalWrite(LED_RED, HIGH);
      Serial.println("LED RED ON");
    }
    else if (command == "green")
    {
      if (isRedOn || isBlueOn)
      {
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_RED, LOW);
        isRedOn = false;
        isBlueOn = false;
      }
      isGreenOn = true;
      digitalWrite(LED_GREEN, HIGH);
      Serial.println("LED GREEN ON");
    }
    else if (command == "blue")
    {
      if (isRedOn || isGreenOn)
      {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
        isRedOn = false;
        isGreenOn = false;
      }
      isBlueOn = true;
      digitalWrite(LED_BLUE, HIGH);
      Serial.println("LED BLUE ON");
    }
    else
    {
      Serial.println(String("command not found: ’") + command + "’");
    }
  }
}

void loop3()
{
  int adcValue = analogRead(POTENTIOMETER);
  float voltage = (adcValue * 5.0) / 1023.0;

  lcd.setCursor(4, 0);
  lcd.print(adcValue);

  lcd.setCursor(10, 0);
  lcd.print(voltage);
  delay(100);
}

void loop4()
{
  int analog = analogRead(POTENTIOMETER);
  String result = String(analog);
  Serial.println(result);
}

void loop()
{
  loop1();
  // loop2();
  // loop3();
  // loop4();
}