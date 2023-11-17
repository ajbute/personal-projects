const byte pin_D1 = 27;
const byte pin_D2 = 53;
const byte pin_D3 = 51;
const byte pin_D4 = 50;

const byte pin_topTop = 25;
const byte pin_topRight = 49;
const byte pin_topLeft = 23;
const byte pin_middle = 52;
const byte pin_bottomBottom = 24;
const byte pin_bottomRight = 26;
const byte pin_bottomLeft = 22;

const uint8_t digitON = LOW;
const uint8_t digitOFF = HIGH;

const uint8_t segmentON = HIGH;
const uint8_t segmentOFF = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin_D1, OUTPUT);
  pinMode(pin_D2, OUTPUT);
  pinMode(pin_D3, OUTPUT);
  pinMode(pin_D4, OUTPUT);

  digitalWrite(pin_D1, digitOFF);
  digitalWrite(pin_D2, digitOFF);
  digitalWrite(pin_D3, digitOFF);
  digitalWrite(pin_D4, digitOFF);

  pinMode(pin_topTop, OUTPUT);
  pinMode(pin_topRight, OUTPUT);
  pinMode(pin_topLeft, OUTPUT);
  pinMode(pin_middle, OUTPUT);
  pinMode(pin_bottomBottom, OUTPUT);
  pinMode(pin_bottomRight, OUTPUT);
  pinMode(pin_bottomLeft, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.println("Enter a digit (1-4)");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {   // Wait for user input
    int digit = Serial.parseInt();
    if (isValidDigit(digit)) {
      Serial.println("Enter a number 0-9 (-1 = off)");
      while (Serial.available() == 0) {};   // Wait for user input
      int number = Serial.parseInt();
      if (isValidNumber(number)) {
        resetDigitPins();
        displayNumber(digit, number);
        Serial.println("Enter a digit (1-4)");
      }
      else {
        Serial.println("Invalid Number");
        Serial.println("Enter a digit (1-4)");
      }
    }
    else {
      Serial.println("Invalid Digit");
      Serial.println("Enter a digit (1-4)");
    }
  }
}

bool isValidDigit(int dig) {
  if (dig >= 1 && dig <= 4) {
    return true;
  }
  else {
    return false;
  }
}

bool isValidNumber(int num) {
  if (num >= -1 && num <= 9) {
    return true;
  }
  else {
    return false;
  }
}

void displayNumber(int dig, int num) {
  switch (dig) {
    case 1:
      digitalWrite(pin_D1, digitON);
      break;
    case 2:
      digitalWrite(pin_D2, digitON);
      break;
    case 3:
      digitalWrite(pin_D3, digitON);
      break;
    case 4:
      digitalWrite(pin_D4, digitON);
      break;
  }

  switch (num) {
    case -1:
      turnOnLeds(segmentOFF, segmentOFF, segmentOFF, segmentOFF, segmentOFF, segmentOFF, segmentOFF, segmentOFF);
      break;
    case 0:
      turnOnLeds(segmentON, segmentON, segmentOFF, segmentON, segmentON, segmentOFF, segmentON, segmentON);
      break;
    case 1:
      turnOnLeds(segmentOFF, segmentON, segmentOFF, segmentON, segmentOFF, segmentOFF, segmentOFF, segmentOFF);
      break;
    case 2:
      turnOnLeds(segmentON, segmentON, segmentOFF, segmentOFF, segmentOFF, segmentON, segmentON, segmentON);
      break;
    case 3:
      turnOnLeds(segmentON, segmentON, segmentOFF, segmentON, segmentOFF, segmentON, segmentON, segmentOFF);
      break;
    case 4:
      turnOnLeds(segmentOFF, segmentON, segmentOFF, segmentON, segmentON, segmentON, segmentOFF, segmentOFF);
      break;
    case 5:
      turnOnLeds(segmentON, segmentOFF, segmentOFF, segmentON, segmentON, segmentON, segmentON, segmentOFF);
      break;
    case 6:
      turnOnLeds(segmentON, segmentOFF, segmentOFF, segmentON, segmentON, segmentON, segmentON, segmentON);
      break;
    case 7:
      turnOnLeds(segmentON, segmentON, segmentOFF, segmentON, segmentOFF, segmentOFF, segmentOFF, segmentOFF);
      break;
    case 8:
      turnOnLeds(segmentON, segmentON, segmentOFF, segmentON, segmentON, segmentON, segmentON, segmentON);
      break;
    case 9:
      turnOnLeds(segmentON, segmentON, segmentOFF, segmentON, segmentON, segmentON, segmentON, segmentOFF);
      break;
  }
}

void resetDigitPins() {
  digitalWrite(pin_D1, digitOFF);
  digitalWrite(pin_D2, digitOFF);
  digitalWrite(pin_D3, digitOFF);
  digitalWrite(pin_D4, digitOFF);
}

void turnOnLeds(uint8_t topTop, uint8_t topRight, uint8_t decimalPoint, uint8_t bottomRight, uint8_t topLeft, uint8_t middle, uint8_t bottomBottom, uint8_t bottomLeft) {
  digitalWrite(pin_topTop, topTop);
  digitalWrite(pin_topRight, topRight);
  digitalWrite(pin_bottomRight, bottomRight);
  digitalWrite(pin_topLeft, topLeft);
  digitalWrite(pin_middle, middle);
  digitalWrite(pin_bottomBottom, bottomBottom);
  digitalWrite(pin_bottomLeft, bottomLeft);
}