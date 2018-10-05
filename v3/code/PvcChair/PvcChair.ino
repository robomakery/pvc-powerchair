/*
  The purpose of this program is to test and demonstrate the display performance of the 128 x 64 display

  A0  Joystick yPin - white coming out of shield, turns to yellow going into joystick
  A1	Joystick xPin - brown coming out of shield, turns to green going into joystick
  A2	Voltage read (5K pot across +5, and ground)
  A3
  A4  SDA (if no SDA pin) - white
  A5  SCL (if not SCL pin) - blue

  D4  EL Left motor (e-stop - green wire)
  D12	ZF Left motor (direction - yellow wire)
  D9	VR Left speed (orange wire)

  D6  EL Right motor (e-stop - grey wire)
  D7	ZF Right motor (direction - blue wire)
  D10	VR Right speed (purple wire)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MotorControl.h"

#define OLED_RESET 4
Adafruit_SSD1306 oledDisplay(OLED_RESET);

// JOYSTICK PINS
int xPosition = 0;
int yPosition = 0;
const int xPin = A0;
const int yPin = A1;
const int voltagePin = A2;

// MOTOR DIRECTION CONTROLS
int leftMotorDirectionPin = 12;
int rightMotorDirectionPin = 7;
int threshold = 20;

MotorControl controller;

int leftMotorSpeedPin = 9;
int rightMotorSpeedPin = 10;

int leftMotorEnabledPin = 4;
int rightMotorEnabledPin = 6;

String formatDouble(double val, int dec, int dig )
{
  int addpad = 0;
  char sbuf[20];
  String fdata = (dtostrf(val, dec, dig, sbuf));
  int slen = fdata.length();
  for (addpad = 1; addpad <= dec + dig - slen; addpad++) {
    fdata = " " + fdata;
  }
  return fdata;
}

void updateDisplay(int xPos, int yPos, int lMotorCommand, int rMotorCommand, float batteryPercentage)
{
  oledDisplay.setTextSize(1);
  oledDisplay.setTextColor(WHITE, BLACK);
  oledDisplay.setCursor(0, 0);
  // oledDisplay.print("Battery: ");
  // oledDisplay.print(formatDouble(batteryPercentage * 100, 3, 2));
  // oledDisplay.println(" %");

  oledDisplay.setTextSize(0.5);
  oledDisplay.print("X: "); oledDisplay.println(formatDouble(xPos, 3, 2));
  oledDisplay.print("Y: "); oledDisplay.println(formatDouble(yPos, 3, 2));
  oledDisplay.print("L MC: "); oledDisplay.println(formatDouble(lMotorCommand, 3, 2));
  oledDisplay.print("R MC: "); oledDisplay.println(formatDouble(rMotorCommand, 3, 2));

  oledDisplay.display();
}

void setup()
{
  Serial.begin(9600);
	
  // Activate Joystick pins
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  // Activate Joystick pins
  pinMode(leftMotorEnabledPin, OUTPUT);
  pinMode(rightMotorEnabledPin, OUTPUT);

  digitalWrite(leftMotorEnabledPin,HIGH);
  digitalWrite(rightMotorEnabledPin,HIGH);

  // Set up Direction outputs for the motor driver inputs
  pinMode(leftMotorDirectionPin, OUTPUT);
  pinMode(rightMotorDirectionPin, OUTPUT);
	
  // Set up Speed outputs for Motor Driver
  pinMode(leftMotorSpeedPin, OUTPUT);
  pinMode(rightMotorSpeedPin, OUTPUT);

  pinMode(voltagePin, INPUT);
  oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  oledDisplay.clearDisplay();
  oledDisplay.display();

  // use initial values as zeros in MotorControl object
  controller.initialize(analogRead(xPin), analogRead(yPin), threshold);
}

void loop()
{
  xPosition = analogRead(xPin);
  int xPositionReversed = map(xPosition, 0, 1023, 1023, 0);
  yPosition = analogRead(yPin);
  int yPositionReversed = map(yPosition, 0, 1023, 1023, 0);

  int motor1Command;
  int motor2Command;
  controller.translateJoystickToMotorCommands(xPositionReversed, yPositionReversed, motor1Command, motor2Command);

  // motor 1 == right motor
  if (motor1Command > 0) {
    digitalWrite(rightMotorDirectionPin, LOW); // forward
  } else {
    digitalWrite(rightMotorDirectionPin, HIGH); // backwards
  }
  analogWrite(rightMotorSpeedPin, motor1Command); 

  // motor2 == left motor
  if (motor2Command > 0) {
    digitalWrite(leftMotorDirectionPin, HIGH); // forward
  } else {
    digitalWrite(leftMotorDirectionPin, LOW); // backwards
  }
  analogWrite(leftMotorSpeedPin, motor2Command); 

  // Compute voltage from the resistor value
  //
  float resistance = analogRead(voltagePin);

  updateDisplay(xPosition, yPosition, motor1Command, motor2Command, resistance / 1024.0f);
}
