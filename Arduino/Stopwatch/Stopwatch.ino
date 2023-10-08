// Defining constants
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

const short NO_DIGITS = -1;
const byte FIRST_DIGIT = 1;
const byte SECOND_DIGIT = 2;
const byte THIRD_DIGIT = 3;
const byte FOURTH_DIGIT = 4;

const short NO_NUMBER = NO_DIGITS;

const byte STOPWATCH_START = 1;
const byte STOPWATCH_PAUSE = 2;
const byte STOPWATCH_RESET = 3;
const byte STOPWATCH_OFF = 4;

const unsigned long ONE_SECOND = 1000; // Unit is ms

const unsigned short MAX_STOPWATCH_TIME = 9999;

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

int* getLastFourDigits(int num, int* amountOfDigits = 0, int count = 0);

// Code in setup() runs once
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
    Serial.println("1: Start | 2: Pause | 3: Reset | 4: Off");
}

int stopwatchTime = 0;
int* digits = NO_NUMBER; // Initalizing digits to -1
int numOfDigits;
bool hasPaused = false;
bool hasStarted = false;
bool hasReset = false;

// Code in loop() runs at the clock speed of your arduino ( For the arduino mega 2460 Rev3 / Uno Rev3 the clock speed is 16MHz )
void loop() {
    if (Serial.available() > 0) {   // Wait for user input
        long input = Serial.parseInt();     // Stores user input
        
        if (input == STOPWATCH_START && !hasStarted) {      // Only runs if the last state wasn't STOPWATCH_START
            Serial.println("Stopwatch Started");
            unsigned long referenceTime = millis();     // Storing a reference time
            unsigned long currentTime;
            while (Serial.available() == 0) {       // Runs while there is no user input
                currentTime = millis();
                if (currentTime - referenceTime >= ONE_SECOND) {        // One second has passed
                    stopwatchTime++;        // Increment display time
                    referenceTime = currentTime;        // Set the new reference time
                    hasReset = false;
                    hasPaused = false;
                    if (stopwatchTime > MAX_STOPWATCH_TIME) {     // Reset stopwatch time back to 0 when it goes over the maximum limit
                        stopwatchTime = 0;
                    }
                }
                else if (hasPaused) {        // Last state was STOPWATCH_PAUSE, so we're displaying the last stored number
                    displayTime(digits, numOfDigits);
                }
                else if (hasReset) {        // Last state was STOPWATCH_RESET, so we're Displaying all 0's
                    displayNumberWithDelay(FOURTH_DIGIT, 0);
                    displayNumberWithDelay(THIRD_DIGIT, 0);
                    displayNumberWithDelay(SECOND_DIGIT, 0);
                    displayNumberWithDelay(FIRST_DIGIT, 0);
                }
                else {
                    digits = getLastFourDigits(stopwatchTime, &numOfDigits);        // Storing the digits of stopwatchTime
                    displayTime(digits, numOfDigits);       // Display stopwatch time
                }
            }
            hasStarted = true;
        }
        else if (input == STOPWATCH_PAUSE && !hasReset) {       // Only pauses if the last state wasn't STOPWATCH_RESET
            Serial.println("Stopwatch Paused");
            hasPaused = true;
            hasStarted = false;
            hasReset = false;
            if (digits != NO_NUMBER) {      // If there was a previous number displayed, display it
                while (Serial.available() == 0) {       // Runs while there is no user input
                    displayTime(digits, numOfDigits);
                }
            }
        }
        else if (input == STOPWATCH_RESET) {
            Serial.println("Stopwatch Reset");
            stopwatchTime = 0;      // Resets stopwatch time
            digits = NO_NUMBER;     // Resetting digits
            hasPaused = false;
            hasStarted = false;
            hasReset = true;
            while (Serial.available() == 0) {       // Runs while there is no user input
                displayNumberWithDelay(FOURTH_DIGIT, 0);        // Displaying all 0's
                displayNumberWithDelay(THIRD_DIGIT, 0);
                displayNumberWithDelay(SECOND_DIGIT, 0);
                displayNumberWithDelay(FIRST_DIGIT, 0);
            }
        }
        else if (input == STOPWATCH_OFF) {
            Serial.println("Stopwatch Off");
            hasStarted = false;
            displayNumber(NO_DIGITS, NO_NUMBER);        // Turning off display
        }
        else {
            Serial.println("Invalid Input");
            hasStarted = false;
            if (digits != NO_NUMBER) {      // If there was a previous number displayed, display it
                while (Serial.available() == 0) {       // Runs while there is no user input
                    displayTime(digits, numOfDigits);
                }
            }
            else if (hasReset) {
                while (Serial.available() == 0) {       // Runs while there is no user input
                    displayNumberWithDelay(FOURTH_DIGIT, 0);        // Displaying all 0's
                    displayNumberWithDelay(THIRD_DIGIT, 0);
                    displayNumberWithDelay(SECOND_DIGIT, 0);
                    displayNumberWithDelay(FIRST_DIGIT, 0);
                }
            }
        }
    }
}

int* getLastFourDigits(int num, int* amountOfDigits, int count) {
    static int digitsArr[4];        // Declared static because unknown behavior happens when returning a pointer to an array
    int currentCount = count;
    int digit = num % 10;       // Grabs the digit at the end of the number
    if (currentCount <= 3) {        // Makes sure the function only runs 4 times
        if (num >= 10) {        // Checks if there's more digits to get
            digitsArr[currentCount] = digit;        // Stores the first digit in the array
            currentCount++;
            getLastFourDigits(num / 10, amountOfDigits, currentCount);      // Calls the function again, passing number with one less digit
        }   
        else {      // Stores last digit
            digitsArr[currentCount] = digit;
            if (amountOfDigits) {       // amountOfDigits was passed as a var so we need to store it
                *amountOfDigits = ++currentCount;
            }
        }
    }
    else if (amountOfDigits) {      // amountOfDigits was passed as a var so we need to store it
        *amountOfDigits = 4;        // Setting to 4
    }
    return digitsArr;
}

void displayTime(int digits[4], int amountOfDigits) {
    switch (amountOfDigits) {
        case 1:
            displayNumber(FOURTH_DIGIT, digits[0]);
        break;
        case 2:
            displayNumberWithDelay(FOURTH_DIGIT, digits[0]);
            displayNumberWithDelay(THIRD_DIGIT, digits[1]);
        break;
        case 3:
            displayNumberWithDelay(FOURTH_DIGIT, digits[0]);
            displayNumberWithDelay(THIRD_DIGIT, digits[1]);
            displayNumberWithDelay(SECOND_DIGIT, digits[2]);
        break;
        case 4:
            displayNumberWithDelay(FOURTH_DIGIT, digits[0]);
            displayNumberWithDelay(THIRD_DIGIT, digits[1]);
            displayNumberWithDelay(SECOND_DIGIT, digits[2]);
            displayNumberWithDelay(FIRST_DIGIT, digits[3]);
        break;
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
    delay(2);
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