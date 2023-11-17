// Digital pins
const byte pin_topTop = 51;
const byte pin_topRight = 47;
const byte pin_decimalPoint = 43;
const byte pin_bottomRight = 39;
const byte pin_topLeft = 35;
const byte pin_middle = 31;
const byte pin_bottomBottom = 27;
const byte pin_bottomLeft = 23;

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

  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.println("Enter a number 0-9 (-1 = off)");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    int input = Serial.parseInt();
    if (isValidNumber(input)) {
      displayNumber(input);
      Serial.println("Enter a number 0-9 (-1 = off)");
    }
    else {
      Serial.println("Invalid Number");
    }
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