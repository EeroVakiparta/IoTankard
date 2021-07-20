// Motor //
int IN1 = 12;
int motorSpeed = 10;
int maxVibrateSpeed = 255;
int minVibrateSpeed = 40;

// Buttons //
int buttonDelay = 100;
int buttonPressed = 0;

const int indexFinger_ButtonPin = 5;
const int middleFinger_ButtonPin = 6;
const int ringFinger_ButtonPin = 7;
const int pinkey_ButtonPin = 8;

int indexFinger_Btn;
int middleFinger_Btn;
int ringFinger_Btn;
int pinkey_Btn;

void setup() {
  Serial.begin(9600);

  /* Buttons */
  pinMode(indexFinger_ButtonPin, INPUT);
  pinMode(middleFinger_ButtonPin, INPUT);
  pinMode(ringFinger_ButtonPin, INPUT);
  pinMode(pinkey_ButtonPin, INPUT);

  /* Motor */
  pinMode(IN1, OUTPUT);

  Serial.println("SETUP DONE");
  delay(10);
}

int readButton(int pin) {
  pin = digitalRead(pin);
  if (pin == 1) {
    delay(50);
    pin = digitalRead(pin);
    if (pin == 0) {
      return HIGH;
    }
  }
}

void loop() {
  buttonPressed = 0;

  indexFinger_Btn = readButton(indexFinger_ButtonPin);
  middleFinger_Btn = readButton(middleFinger_ButtonPin);
  ringFinger_Btn = readButton(ringFinger_ButtonPin);
  pinkey_Btn = readButton(pinkey_ButtonPin);

  if (indexFinger_Btn == HIGH && buttonPressed == 0 ) {
    Serial.println("Q pressed ");
    delay(buttonDelay);
    buttonPressed = 1;
  } else if (middleFinger_Btn == HIGH && buttonPressed == 0) {
    Serial.println("W pressed ");
    delay(buttonDelay);
    Pulse(10);
    buttonPressed = 1;
  } else if (ringFinger_Btn == HIGH && buttonPressed == 0) {
    Serial.println("E pressed ");
    delay(buttonDelay);
    Climb(5);
    buttonPressed = 1;
  } else if (pinkey_Btn == HIGH && buttonPressed == 0) {
    Serial.println("R pressed ");
    StopMotor()
    delay(buttonDelay);
    buttonPressed = 1;
  }
}

void Pulse(int pulseCount) {
  for (int i = 0; i < pulseCount; i++ ) {
    analogWrite(IN1, maxVibrateSpeed);
    delay(100);
    analogWrite(IIN1N2, 0);
    delay(100);
    Serial.println("Pulse");
  }
}

void Climb(int climbCount)
{
  for (int i = 0; i < climbCount; i++ ) {
    for (int i = minVibrateSpeed; i < maxVibrateSpeed; i = i + 5) {
      analogWrite(IN1, i);
      Serial.println(i);
      delay(20);
    }
    for (int i = maxVibrateSpeed; i > minVibrateSpeed; i = i - 5) {
      analogWrite(IN1, i);
      Serial.println(i);
      delay(20);
    }
    Serial.println("Climbed");
    Serial.println();
    analogWrite(IN1, 0);
  }
}

void StopMotor()
{
  digitalWrite(IN2, LOW);
  Serial.println("Stop Vibration");
}
