
/* Arduion Code for the Open Source Wheel Chair

  //R.Beetem
  // Created 06/14/2017

  Because Sabertooth controls two motors with one 8 byte character, when operating in Simplified
  Serial mode, each motor has 7 bits of resolution. Sending a character between 1 and 127 will
  control motor 1. 1 is full reverse, 64 is stop and 127 is full forward. Sending a character between
  128 and 255 will control motor 2. 128 is full reverse, 192 is stop and 255 is full forward.
  Character 0 (hex 0x00) is a special case. Sending this character will shut down both motors.
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    12
#define OLED_CS    11
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

String cmdStrUD = "no command";
String cmdStrLR = "no command";

const int battPin = 0; // A Pin 0
const int joyUD = 4; // A Pin A3
const int joyLR = 5; // A Pin A4

const int threshold = 10;
float R1 = 30000.0; //
float R2 = 7500.0; //

int serialUD = 0;
int serialLR = 0;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

  display.begin(SSD1306_SWITCHCAPVCC);
}


void loop() {

  // Battery Voltage input and calculations
  int rawBatt = analogRead(battPin);
  float vout = (rawBatt * 5.0) / 1024.0; // see text
  float vin = vout / (R2 / (R1 + R2));
  int battPct = (vin / 12.4) * 100;

  Serial.println(vin);


  // Collect raw analog data (range: 0-1023)
  int valUD = analogRead(joyUD);
  int valLR = analogRead(joyLR);

  // Map analog input to stick percentages
  int percentUD = map(valUD, 0, 1023, -100, 100);
  int percentLR = map(valLR, 0, 1023, -100, 100);

  // Map UD percents to serial commands
  if (percentUD > threshold)
  {
    serialUD = map(percentUD, -100, -1, 1, 63); // 1 - 63 Reverse, Motor 1
    cmdStrUD = "BACKWARD";
  }
  else if (percentUD < -threshold)
  {
    serialUD = map(percentUD, 1, 100, 65, 127); // 65 - 127 Forward, Motor 1
    cmdStrUD = "FORWARD";
  }
  else
  {
    serialUD = 64; // STOP, Motor 1
    cmdStrUD = "Neutral";
  }


  // Map LR percents to serial commands
  if (percentLR > threshold)
  {
    serialLR = map(percentLR, -100, -1, 128, 191); // 128-191 Reverse, Motor 2
    cmdStrLR = "BACKWARD";
  }
  else if (percentLR < -threshold)
  {
    serialLR = map(percentLR, 1, 100, 193, 255);  // 193-255 Forward, Motor 2
    cmdStrLR = "FORWARD";
  }
  else
  {
    serialLR = 192; // STOP, Motor 2
    cmdStrLR = "Neutral";
  }

  Serial.print("UD: ");
  Serial.print(serialUD);
  Serial.println(cmdStrUD);

  Serial.print("LR: ");
  Serial.print(serialLR);
  Serial.println(cmdStrLR);



  //------ OLED ------//
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Robomakery & SDRC");
  display.setCursor(0, 8);
  display.println("PVC-Powerchair Rev. 1");

  display.setCursor(0, 18);
  display.setTextSize(1);
  display.println("Battery Level:");
  display.setCursor(84, 18);
  display.println(battPct);

  display.setCursor(0, 30);
  display.setTextSize(1);
  display.println("Joystick Commands:");

  display.setCursor(0, 40);
  display.println("F/R:");
  display.setCursor(30, 40);
  display.println(String(percentUD));
  display.setCursor(50, 40);
  display.println(" %");

  display.setCursor(0, 50);
  display.println("L/R:");
  display.setCursor(30, 50);
  display.println(String(percentLR));
  display.setCursor(50, 50);
  display.println(" %");

  display.display();

} // END 
