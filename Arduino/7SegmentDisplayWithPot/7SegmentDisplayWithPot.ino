#include <math.h>

// Digital pins
const byte pin_topTop = 51;
const byte pin_topRight = 47;
const byte pin_decimalPoint = 43;
const byte pin_bottomRight = 39;
const byte pin_topLeft = 35;
const byte pin_middle = 31;
const byte pin_bottomBottom = 27;
const byte pin_bottomLeft = 23;
const byte pin_selectButton = 52;
const byte pin_resetButton = 22;
const byte pin_potentiometer = A8;
  
const double scalingFactor = 113.66;   // Scaling factor to turn the 0-1023 output of the analog pin into a 0-9 range for displayNumber()
int lastNormalizedVal = -1;  // Giving an initial value that will always pass the if statement the first time
const int OFF = -1;   // Variable used for readability
bool displayOff = false;
bool isPressed = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin_topTop, OUTPUT);
  pinMode(pin_topRight, OUTPUT);
  pinMode(pin_decimalPoint, OUTPUT);
  pinMode(pin_bottomRight, OUTPUT);
  pinMode(pin_topLeft, OUTPUT);
  pinMode(pin_middle, OUTPUT);
  pinMode(pin_bottomBottom, OUTPUT);
  pinMode(pin_bottomLeft, OUTPUT);
  pinMode(pin_selectButton, INPUT_PULLUP);
  pinMode(pin_resetButton, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.println("Turn the pot to change the number. Press the button to select it.");
}

void loop() {
  // put your main code here, to run repeatedly:
  int resetButtonVal = digitalRead(pin_resetButton);
  if (!resetButtonVal && !isPressed) {   // Reset button pressed
    displayOff = !displayOff;
    isPressed = true;
    if (displayOff) {
      displayNumber(OFF);
    }
  }
  else if (resetButtonVal) {    // Reset button not pressed
    isPressed = false;
  }
  if (!displayOff) {
    int selectButtonVal = digitalRead(pin_selectButton);
    int potVal = analogRead(pin_potentiometer);
    int normalizedVal = round(potVal / scalingFactor);  // Rounding quotient to get a 0-9 range of integers
    //Serial.println(potVal);

    displayNumber(normalizedVal);
    if (!selectButtonVal && normalizedVal != lastNormalizedVal) {   // Select button pressed
      Serial.print("Selected number is: ");
      Serial.println(normalizedVal);
      lastNormalizedVal = normalizedVal;
    }
  }
}

void displayNumber(int num) {
  switch (num) {
    case -1:
      turnOnLeds(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW);
      break;
    case 0:
      turnOnLeds(HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH);
      break;
    case 1:
      turnOnLeds(LOW, HIGH, LOW, HIGH, LOW, LOW, LOW, LOW);
      break;
    case 2:
      turnOnLeds(HIGH, HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH);
      break;
    case 3:
      turnOnLeds(HIGH, HIGH, LOW, HIGH, LOW, HIGH, HIGH, LOW);
      break;
    case 4:
      turnOnLeds(LOW, HIGH, LOW, HIGH, HIGH, HIGH, LOW, LOW);
      break;
    case 5:
      turnOnLeds(HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH, LOW);
      break;
    case 6:
      turnOnLeds(HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH);
      break;
    case 7:
      turnOnLeds(HIGH, HIGH, LOW, HIGH, LOW, LOW, LOW, LOW);
      break;
    case 8:
      turnOnLeds(HIGH, HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH);
      break;
    case 9:
      turnOnLeds(HIGH, HIGH, LOW, HIGH, HIGH, HIGH, HIGH, LOW);
      break;
  }
}

void turnOnLeds(bool topTop, bool topRight, bool decimalPoint, bool bottomRight, bool topLeft, bool middle, bool bottomBottom, bool bottomLeft) {
  digitalWrite(pin_topTop, topTop);
  digitalWrite(pin_topRight, topRight);
  digitalWrite(pin_decimalPoint, decimalPoint);
  digitalWrite(pin_bottomRight, bottomRight);
  digitalWrite(pin_topLeft, topLeft);
  digitalWrite(pin_middle, middle);
  digitalWrite(pin_bottomBottom, bottomBottom);
  digitalWrite(pin_bottomLeft, bottomLeft);
}
