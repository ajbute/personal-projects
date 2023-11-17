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
    Serial.println("Enter a 4 digit number (0-9999, -1 = off)");
}

void loop() {
    if (Serial.available() > 0) {   // Wait for user input
        long number = Serial.parseInt();
        Serial.print("Number: ");
        Serial.println(number);
        if (isValidNumber(number)) {
            // Reset display
            displayNumber(NO_DIGITS, NO_NUMBER);
            // Displays a number if the display should be on
            if (number != NO_NUMBER) {
                int digits[4];
                int numOfDigits;
                getDigits(number, 0, digits, numOfDigits);      // Storing the digits of the number
                switch (numOfDigits) {
                case 1:
                    displayNumber(FOURTH_DIGIT, number);
                break;
                case 2:
                    while (Serial.available() == 0) {   // Displays number while there's no input
                        displayNumberWithDelay(FOURTH_DIGIT, digits[0]);
                        displayNumberWithDelay(THIRD_DIGIT, digits[1]);
                    }
                break;
                case 3:
                    while (Serial.available() == 0) {   // Displays number while there's no input
                        displayNumberWithDelay(FOURTH_DIGIT, digits[0]);
                        displayNumberWithDelay(THIRD_DIGIT, digits[1]);
                        displayNumberWithDelay(SECOND_DIGIT, digits[2]);
                    }
                break;
                case 4:
                    while (Serial.available() == 0) {   // Displays number while there's no input
                        displayNumberWithDelay(FOURTH_DIGIT, digits[0]);
                        displayNumberWithDelay(THIRD_DIGIT, digits[1]);
                        displayNumberWithDelay(SECOND_DIGIT, digits[2]);
                        displayNumberWithDelay(FIRST_DIGIT, digits[3]);
                    }
                break;
                }
            }
        }
        else {
            Serial.println("Invalid Number");
            Serial.println("Enter a 4 digit number (0-9999)");
        }
    }
}

bool isValidNumber(int num) {
    if (num >= -1 && num <= 9999) {
        return true;
    }
    else {
        return false;
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

void displayNumberWithDelay(int digit, int number) {
    displayNumber(digit, number);
    delay(2);       // 2ms delay
    // Resets display
    displayNumber(NO_DIGITS, NO_NUMBER);
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