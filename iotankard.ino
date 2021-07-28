#include <SparkFun_ADXL345.h> // accelerometer
#include <SoftwareSerial.h>  // Bluetooth

// Screen //
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bluetooth //
SoftwareSerial bt(2,3); // RX/TS pins as parameters remember to cross wire these

// Accelerometer //
ADXL345 adxl = ADXL345(10);           // SPI com ADXL345(CS_PIN);

// Motor //
int IN1 = 9;
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

// IR sensor //
int IRsensor = A2;

// Status variables //
bool drinkLoaded = false;
bool sipTaken = false;
int sipcount = 0;

void setup() {
  Serial.begin(9600);
  bt.begin(9600);


  /* Screen */
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.setCursor(0, 0);
  delay(200);
  
  
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
  pinMode(A1, OUTPUT);

  /* IR sensor */
  pinMode (IRsensor, INPUT);

  Serial.println("SETUP DONE");
  bt.println("BT SETUP DONE");
  drawtext("IoTankard");
  delay(10);
}

void loop() {
  
  if(digitalRead (IRsensor) == 0){
    //Serial.println("Beer loaded."); 
    drinkLoaded = true;
  }else{
    //Serial.println("Load beer now.");
    drinkLoaded = false;
  }
  
  buttonPressed = 0;
  
  // bt.println("BT ping"); // BT debugging
  
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

  if(sipTaken){
    if(x < 10 && y < 10 && x > -10 && y > -10){
      Serial.println("Reseting sip.");
      sipTaken = false;
    }
  }

  if(drinkLoaded && (x > 20 || y > 20 || x < -20 || y < -20) && !sipTaken){
    Serial.println("Sip taken ! ");
    sipcount++;
    Serial.print("Sips total: ");
    Serial.println(sipcount);
    sipTaken = true;
  }
 
  indexFinger_Btn = readButton(indexFinger_ButtonPin);
  middleFinger_Btn = readButton(middleFinger_ButtonPin);
  ringFinger_Btn = readButton(ringFinger_ButtonPin);
  pinkey_Btn = readButton(pinkey_ButtonPin);

  if (indexFinger_Btn == HIGH && buttonPressed == 0 ) {
    Serial.println("Q pressed ");
    bt.println("Q");
    buttonFeedback(1);
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

// --- Screen functions --- //
void drawtext(String texti) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  //display.println(F("test"));
  display.println(texti);
  display.display();      // Show initial text
  delay(100);
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

void buttonFeedback(int pulseCount) {  
    analogWrite(IN1, maxVibrateSpeed);
    delay(15);
    analogWrite(IN1, 0);
    Serial.println("Button Feedback");
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