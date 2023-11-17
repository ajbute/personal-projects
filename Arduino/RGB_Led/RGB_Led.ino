const byte pin_red = 6;
const byte pin_green = 4;
const byte pin_blue = 2;
//const byte pin_potentiometer = A15;

void setup() {
  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  //int potVal = analogRead(pin_potentiometer);
  Serial.println("Enter a red value");
  while (Serial.available() == 0) {}
  int red = Serial.parseInt();

  Serial.println("Enter a green value");
  while (Serial.available() == 0) {}
  int green = Serial.parseInt();

  Serial.println("Enter a blue value");
  while (Serial.available() == 0) {}
  int blue = Serial.parseInt();

  analogWrite(pin_red, red);
  analogWrite(pin_green, green);
  analogWrite(pin_blue, blue);
}
