const int greenLed = 39;
const int redLed = 23;

void setup() {
  // put your setup code here, to run once:
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    Serial.begin(9600);
    Serial.println("\n1: Red\n2: Green\n3: Off");
}

void loop() {
  // put your main code here, to run repeatedly:
    while (Serial.available() == 0) {
        // Do nothing
    }

    int input = Serial.parseInt();

    if (input == 1) {
        digitalWrite(redLed, HIGH);
        digitalWrite(greenLed, LOW);
    }
    else if (input == 2) {
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, HIGH);
    }
    else if (input == 3) {
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, LOW);
    }
    else if (input != 0) {
        Serial.println("Invalid input");
    }
}
