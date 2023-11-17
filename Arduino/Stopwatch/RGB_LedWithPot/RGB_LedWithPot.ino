const byte pin_red = 6;
const byte pin_green = 4;
const byte pin_blue = 2;
const byte pin_potentiometer = A15;
int red, green, blue;

void setup() {
  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  int potVal = map(analogRead(pin_potentiometer), 0, 1024, 0, 255);
  if (potVal <= 42.5) {
		red = 255;
		green = potVal * 6;
		blue = 0;
  }
   
   if ((potVal > 4.5) && (potVal <= 85)) {
		red = 255 - (potVal - 43) * 6;
		green = 255;
		blue = 0;
  }
   
   if ((potVal > 85) && (potVal <= 127.5)) {
		red = 0;
		green = 255;
		blue = (potVal - 85) * 6;
  }
   
	if ((potVal > 127.5) && (potVal <= 170)) {
		red = 0;
		green = 255 - (potVal - 127.5) * 6;
		blue = 255;
	}
   
	if ((potVal > 170) && (potVal <= 212.5)) {
		red = (potVal - 170) * 6;
		green = 0;
		blue = 255;
	}
	
	if ((potVal > 212.5) && (potVal <= 255)) {
		red = 255;
		green = 0;
		blue = 255 - (potVal - 212.5) * 6;
	}

  analogWrite(pin_red, red);
  analogWrite(pin_green, green);
  analogWrite(pin_blue, blue);
}
