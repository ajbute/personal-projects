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

const uint8_t DIGIT_ON = LOW;
const uint8_t DIGIT_OFF = HIGH;

const uint8_t SEGMENT_ON = HIGH;
const uint8_t SEGMENT_OFF = LOW;

const int NO_DIGITS = -1;
const int FIRST_DIGIT = 1;
const int SECOND_DIGIT = 2;
const int THIRD_DIGIT = 3;
const int FOURTH_DIGIT = 4;

const int NO_NUMBER = NO_DIGITS;

// Defining the segment combination for different numbers
const int ZERO[7] = {SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_ON, SEGMENT_ON};
const int ONE[7] = {SEGMENT_OFF, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF};
const int TWO[7] = {SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON};
const int THREE[7] = {SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF};
const int FOUR[7] = {SEGMENT_OFF, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF};
const int FIVE[7] = {SEGMENT_ON, SEGMENT_OFF, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF};
const int SIX[7] = {SEGMENT_ON, SEGMENT_OFF, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON};
const int SEVEN[7] = {SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF};
const int EIGHT[7] = {SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON};
const int NINE[7] = {SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF};

void setup() {
  pinMode(pin_D1, OUTPUT);
  pinMode(pin_D2, OUTPUT);
  pinMode(pin_D3, OUTPUT);
  pinMode(pin_D4, OUTPUT);

  digitalWrite(pin_D1, DIGIT_OFF);
  digitalWrite(pin_D2, DIGIT_OFF);
  digitalWrite(pin_D3, DIGIT_OFF);
  digitalWrite(pin_D4, DIGIT_OFF);

  pinMode(pin_topTop, OUTPUT);
  pinMode(pin_topRight, OUTPUT);
  pinMode(pin_topLeft, OUTPUT);
  pinMode(pin_middle, OUTPUT);
  pinMode(pin_bottomBottom, OUTPUT);
  pinMode(pin_bottomRight, OUTPUT);
  pinMode(pin_bottomLeft, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.println("Enter a start time");
}

void loop() {
  if (Serial.available() > 0) {   // Wait for user input
    long startTime = Serial.parseInt();

    int startTimeDigits[4];
    int numOfDigits;

    getDigits(startTime, 0, startTimeDigits, numOfDigits);

    if (isValidTime(startTimeDigits, numOfDigits)) {
      int* currentTimeDigits = startTimeDigits;
      displayTime(currentTimeDigits, numOfDigits);    // Display the start time
      unsigned long referenceTime = millis();
      while (Serial.available() == 0) {   // Wait for user input
        if (millis() - referenceTime >= 1000) {   // 1 second has passed
          currentTimeDigits = incrementTime(currentTimeDigits, numOfDigits);
        }
      }
    }
    else {
      Serial.println("Invalid Start Time");
      Serial.println("Enter a start time");
    }
  }
}

// Stores the digits of a passed number in a passed array. Also stores the amount of digits in a passed var
void getDigits(int num, int count, int (&digitsArr)[4], int& amountOfDigits) {
  int currentCount = count;
  int digit = num % 10;

  if (num >= 10) {
    digitsArr[currentCount] = digit;
    currentCount++;
    getDigits(num / 10, currentCount, digitsArr, amountOfDigits);
  }
  else {
    digitsArr[currentCount] = digit;
    amountOfDigits = ++currentCount;
  }
}

bool isValidTime(int digits[4], int amountOfDigits) {
  int minutes = digits[0] + digits[1] * 10;
  int hour = digits[2] + digits[3] * 10;
  if (amountOfDigits == 3 || amountOfDigits == 4) {
    if (hour >= 1 && hour <= 24) {
      if (hour == 24) {
          if (minutes == 0) {
              return true;
          }
      }
      else if (minutes >= 0 && minutes <= 59) {
          return true;
      }
    }
  }
  return false;
}

void displayTime(int digits[4], int amountOfDigits) {
  int hour = digits[2] + digits[3] * 10;

  displayNumber(THIRD_DIGIT, digits[2]);    // Displays 1st digit of the hour
  delay(2);
  displayNumber(NO_DIGITS, NO_NUMBER);    // Resets display

  if (hour / 10 != 0) {   // Hour is 2 digits
    displayNumber(FOURTH_DIGIT, digits[3]);   // Displays the 2nd digit of the hour
    delay(2);
    displayNumber(NO_DIGITS, NO_NUMBER);    // Resets display
  }

  displayNumber(SECOND_DIGIT, digits[1]);
  delay(2);
  displayNumber(NO_DIGITS, NO_NUMBER);    // Resets display
  displayNumber(FIRST_DIGIT, digits[0]);
  delay(2);
  displayNumber(NO_DIGITS, NO_NUMBER);    // Resets display
}

int* incrementTime(int digits[4], int amountOfDigits) {
  int minutes = digits[0] + digits[1] * 10;
  int hour = digits[2] + digits[3] * 10;
  int hourDigits[2];
  int hourNumOfDigits;
  int minutesDigits[2];
  int numOfMinutesDigits;

  minutes++;
  if (minutes >= 60) {    // Hour needs to be incremented
    digits[0] = 0;    // Reset minutes digits
    digits[1] = 0;    // Reset minutes digits

    hour++;
    if (hour >= 24) {   // Hour needs to be rolled over
      digits[2] = 1;    // Resets hour to 1
      digits[3] = 0;
    }
    else {
      getDigits(hour, 0, hourDigits, int hourNumOfDigits);
      digits[2] = hourDigits[0];
      digits[3] = hourDigits[1];
    }
  }
  else {
    getDigits(minutes, 0, minutesDigits, numOfMinutesDigits);
    digits[0] = minutesDigits[1];
    digits[1] = minutesDigits[0];
  }
  return digits
}

void displayNumber(int dig, int num) {
  switch (dig) {
    case NO_DIGITS:
      digitalWrite(pin_D1, DIGIT_OFF);
      digitalWrite(pin_D2, DIGIT_OFF);
      digitalWrite(pin_D3, DIGIT_OFF);
      digitalWrite(pin_D4, DIGIT_OFF);
    break;
    case FIRST_DIGIT:
      digitalWrite(pin_D1, DIGIT_ON);
    break;
    case SECOND_DIGIT:
      digitalWrite(pin_D2, DIGIT_ON);
    break;
    case THIRD_DIGIT:
      digitalWrite(pin_D3, DIGIT_ON);
    break;
    case FOURTH_DIGIT:
      digitalWrite(pin_D4, DIGIT_ON);
    break;
  }

  switch (num) {
    case NO_NUMBER:
      setLedStates(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
    break;
    case 0:
      setLedStates(ZERO[0], ZERO[1], ZERO[2], ZERO[3], ZERO[4], ZERO[5], ZERO[6]);
    break;
    case 1:
      setLedStates(ONE[0], ONE[1], ONE[2], ONE[3], ONE[4], ONE[5], ONE[6]);
    break;
    case 2:
      setLedStates(TWO[0], TWO[1], TWO[2], TWO[3], TWO[4], TWO[5], TWO[6]);
    break;
    case 3:
      setLedStates(THREE[0], THREE[1], THREE[2], THREE[3], THREE[4], THREE[5], THREE[6]);
    break;
    case 4:
      setLedStates(FOUR[0], FOUR[1], FOUR[2], FOUR[3], FOUR[4], FOUR[5], FOUR[6]);
    break;
    case 5:
      setLedStates(FIVE[0], FIVE[1], FIVE[2], FIVE[3], FIVE[4], FIVE[5], FIVE[6]);
    break;
    case 6:
      setLedStates(SIX[0], SIX[1], SIX[2], SIX[3], SIX[4], SIX[5], SIX[6]);
    break;
    case 7:
      setLedStates(SEVEN[0], SEVEN[1], SEVEN[2], SEVEN[3], SEVEN[4], SEVEN[5], SEVEN[6]);
    break;
    case 8:
      setLedStates(EIGHT[0], EIGHT[1], EIGHT[2], EIGHT[3], EIGHT[4], EIGHT[5], EIGHT[6]);
    break;
    case 9:
      setLedStates(NINE[0], NINE[1], NINE[2], NINE[3], NINE[4], NINE[5], NINE[6]);
    break;
  }
}

void setLedStates(uint8_t topTop, uint8_t topRight, uint8_t bottomRight, uint8_t topLeft, uint8_t middle, uint8_t bottomBottom, uint8_t bottomLeft) {
  digitalWrite(pin_topTop, topTop);
  digitalWrite(pin_topRight, topRight);
  digitalWrite(pin_bottomRight, bottomRight);
  digitalWrite(pin_topLeft, topLeft);
  digitalWrite(pin_middle, middle);
  digitalWrite(pin_bottomBottom, bottomBottom);
  digitalWrite(pin_bottomLeft, bottomLeft);
}