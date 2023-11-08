#include <Arduino.h>
#include <util/atomic.h>
#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define ENCODER1 A2
#define ENCODER2 A3

#define BUTTON_PIN_RED 2

#define ENCODER_PIN_A A2
#define ENCODER_PIN_B A3

#define DEBOUNCE_DELAY 100
#define DEBOUNCE_INTERVAL 10UL

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup1()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(ENCODER1, INPUT_PULLUP);
  pinMode(ENCODER2, INPUT_PULLUP);
}

void loop1()
{
  digitalWrite(LED_RED, digitalRead(ENCODER1));
  digitalWrite(LED_BLUE, digitalRead(ENCODER2));
}

void updateMenuPosition(int position) {
  switch (position + 1) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">Red LED");
      lcd.setCursor(1, 1);
      lcd.print("Green LED");
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Red LED");
      lcd.setCursor(0, 1);
      lcd.print(">Green LED");
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">Blue LED");
  }
}

void setup2()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RED, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  updateMenuPosition(0);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10);
}

int getLEDPinByMenuOption(int menuOption)
{
  switch (menuOption) {
    case 0: return LED_RED;
    case 1: return LED_GREEN;
    default: return LED_BLUE;
  }
}

void displayLedIntensity(int intensity) {
  char buffer[16];
  sprintf(buffer, "Intensity: %3d", intensity);
  lcd.setCursor(2, 0);
  lcd.print(buffer);
}

bool isRedBtnPressed(){
  static int stableState = HIGH;
  static int previousState = HIGH;
  static unsigned long debounceTimer = 0UL;
  bool pressed = false;

  int currentButtonState = digitalRead(BUTTON_PIN_RED);

  if (previousState != currentButtonState) 
  {
    debounceTimer = millis();
  }

  if ((millis() - debounceTimer) > DEBOUNCE_INTERVAL) {
    if (currentButtonState != stableState) {
      if (stableState == HIGH && currentButtonState == LOW) {
        pressed = true;
      }
      stableState = currentButtonState;
    }
  }

  previousState = currentButtonState;

  return pressed;
}

void adjustLEDBrightness(int ledPin, int brightness) {
  displayLedIntensity(brightness);
  analogWrite(ledPin, brightness);
}

volatile bool isMenuOn = true;
volatile int rotaryAState = HIGH;
volatile int rotaryBState = HIGH;
volatile unsigned long rotaryInterruptTime = 0UL;

int selectedMenu;
volatile int rotaryCounter = 0;
int lastRotaryA = LOW;
unsigned long lastInteractionTime = 0UL;
int currentRotaryA;
int currentRotaryB;
unsigned long currentInterruptTime;

ISR(PCINT1_vect) {
  rotaryAState = digitalRead(ENCODER_PIN_A);
  rotaryBState = digitalRead(ENCODER_PIN_B);
  rotaryInterruptTime = millis();
}

void readRotaryEncoderState() {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    currentRotaryA = rotaryAState;
    currentRotaryB = rotaryBState;
    currentInterruptTime = rotaryInterruptTime;
  }
}

void updateState() {
  lastRotaryA = currentRotaryA;
  lastInteractionTime = currentInterruptTime;
}

bool isDebounced() {
  return currentInterruptTime > lastInteractionTime + DEBOUNCE_DELAY;
}

void updateRotaryCounterForMenu() {
  if (currentRotaryB == HIGH) {
    if (rotaryCounter < 2)
      rotaryCounter += 1;
  } else {
    if (rotaryCounter > 0)
      rotaryCounter -= 1;
  }
}

void updateRotaryCounterForBrightness() {
  if (currentRotaryB == HIGH) {
    if (rotaryCounter < 255)
      rotaryCounter += 15;
  } else {
    if (rotaryCounter > 0)
      rotaryCounter -= 15;
  }
}

void toggleMenuMode() {
  if (isRedBtnPressed()) {
    isMenuOn = !isMenuOn;
    lcd.clear();
    if (!isMenuOn) {
      selectedMenu = rotaryCounter;
    }
  }
}

void handleMenuMode() {
  if (currentRotaryA == LOW && isDebounced()) {
    updateRotaryCounterForMenu();
    updateMenuPosition(rotaryCounter);
  }
  toggleMenuMode();
}

void handleNonMenuMode() {
  if (currentRotaryA == LOW && isDebounced()) {
    updateRotaryCounterForBrightness();
    adjustLEDBrightness(getLEDPinByMenuOption(selectedMenu), rotaryCounter);
  }
  toggleMenuMode();
}

void loop2()
{
  readRotaryEncoderState();

  if (isMenuOn)
  {
    handleMenuMode();
  }
  else
  {
    handleNonMenuMode();
  }

  updateState();
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