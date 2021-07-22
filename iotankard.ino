#include <SparkFun_ADXL345.h> // accelerometer
#include <SoftwareSerial.h>  // Bluetooth

// Bluetooth //
SoftwareSerial bt(2,3); // RX/TS pins as parameters

// Accelerometer //
ADXL345 adxl = ADXL345(10);           // SPI com ADXL345(CS_PIN);

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
  bt.begin(9600);
  /* Accelerometer */
  adxl.powerOn();
  adxl.setRangeSetting(16);           // range in G (2,4,8,16) Drawback with higher Gs is lower sensitivity
  adxl.setSpiBit(0);                  // 0 = 4wire SPI , 1=3wire SPI

  adxl.setActivityXYZ(1, 0, 0);       // Which axis the activity feature uses
  adxl.setActivityThreshold(75);      // Adjust sensitivity of previous 62.5mg per increment (0-255)

  adxl.setInactivityXYZ(1, 0, 0);     // Which axis the inactivity feature uses
  adxl.setInactivityThreshold(75);    // 62.5mg per increment (0-255)
  adxl.setTimeInactivity(10);         // Sec to inactivity to activate

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Selection of tap features axis

  adxl.setTapThreshold(50);           // 62.5 mg /step Tap strenght (0-255)
  adxl.setTapDuration(15);            // 625 μs /step
  adxl.setDoubleTapLatency(80);       // 1.25 ms /step Doubletap
  adxl.setDoubleTapWindow(200);       // 1.25 ms /step

  adxl.setFreeFallThreshold(7);       // Free fall (5 - 9) recommended - 62.5mg per increment (0-255)
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment (0-255)

  adxl.ActivityINT(1); // Turn activity on
  adxl.InactivityINT(1); // Turn inactivity on
  adxl.singleTapINT(1); // Turn tap on
  adxl.doubleTapINT(1); // Turn double-tap on
  adxl.FreeFallINT(1); // Turn Free fall on

  /* Buttons */
  pinMode(indexFinger_ButtonPin, INPUT);
  pinMode(middleFinger_ButtonPin, INPUT);
  pinMode(ringFinger_ButtonPin, INPUT);
  pinMode(pinkey_ButtonPin, INPUT);

  /* Motor */
  pinMode(IN1, OUTPUT);

  Serial.println("SETUP DONE");
  bt.println("SERUP DONE");
  delay(10);
}

void loop() {

  bt.println("BT ping");
  // Accelerometer
  int x, y, z;
  adxl.readAccel(&x, &y, &z);
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z);
  delay(500);
  ADXL_ISR();


  buttonPressed = 0;

  indexFinger_Btn = readButton(indexFinger_ButtonPin);
  middleFinger_Btn = readButton(middleFinger_ButtonPin);
  ringFinger_Btn = readButton(ringFinger_ButtonPin);
  pinkey_Btn = readButton(pinkey_ButtonPin);

  if (indexFinger_Btn == HIGH && buttonPressed == 0 ) {
    Serial.println("Q pressed ");
    bt.println("Q");
    delay(buttonDelay);
    buttonPressed = 1;
  } else if (middleFinger_Btn == HIGH && buttonPressed == 0) {
    Serial.println("W pressed ");
    bt.println("W");
    delay(buttonDelay);
    Pulse(10);
    buttonPressed = 1;
  } else if (ringFinger_Btn == HIGH && buttonPressed == 0) {
    Serial.println("E pressed ");
    bt.println("E");
    delay(buttonDelay);
    Climb(5);
    buttonPressed = 1;
  } else if (pinkey_Btn == HIGH && buttonPressed == 0) {
    Serial.println("R pressed ");
    bt.println("R");
    StopMotor();
    delay(buttonDelay);
    buttonPressed = 1;
  }
}

// --- Button functions --- //
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

// --- Motor functions --- //
void Pulse(int pulseCount) {
  for (int i = 0; i < pulseCount; i++ ) {
    analogWrite(IN1, maxVibrateSpeed);
    delay(100);
    analogWrite(IN1, 0);
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
  digitalWrite(IN1, LOW);
  Serial.println("Stop Vibration");
}

// --- Accelerometer functions --- //
void ADXL_ISR() {

  byte interrupts = adxl.getInterruptSource(); // clears all triggers

  if (adxl.triggered(interrupts, ADXL345_FREE_FALL)) {
    Serial.println("FREE FALL");
  }

  if (adxl.triggered(interrupts, ADXL345_INACTIVITY)) {
    Serial.println("INACTIVITY");
  }

  if (adxl.triggered(interrupts, ADXL345_ACTIVITY)) {
    Serial.println("ACTIVITY");
  }

  if (adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
    Serial.println("DOUBLE TAP");
  }

  if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
    Serial.println("TAP");
  }
}
