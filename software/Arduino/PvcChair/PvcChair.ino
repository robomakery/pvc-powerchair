// PVC Chair

#include <Arduino.h>
// https://github.com/olikraus/u8g2/wiki/u8x8reference
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
// https://www.dimensionengineering.com/info/arduino
#include <SabertoothSimplified.h>

// Connections to make:
//   Arduino TX->1  ->  Sabertooth S1
//   Arduino GND    ->  Sabertooth 0V
//   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
SabertoothSimplified ST;

// https://www.amazon.com/gp/customer-reviews/RE5PIJFQ7K685/ref=cm_cr_getr_d_rvw_ttl?ie=UTF8&ASIN=B01HEBIJKK
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); // OLEDs without Reset of the Display

char cols[15];
char rows[15];

const int battPin = A0;
const int threshold = 10;
float R1 = 30000.0;
float R2 = 7500.0;

const int joyUD = A2;
const int joyLR = A3;

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  sprintf(cols, "%d", u8x8.getCols());
  sprintf(rows, "%d", u8x8.getRows());
  Serial.println(strcat("Cols: ", cols));
  Serial.println(strcat("Rows: ", rows));
}


void loop()
{
  u8x8.drawString(0,0,"Hello World!");

  // Battery Voltage input and calculations
  int rawBatt = analogRead(battPin);
  float vout = (rawBatt * 5.0) / 1024.0; // see text
  float vin = vout / (R2 / (R1 + R2));
  int battPct = (vin / 12.4) * 100;
  char batteryMsg[50];
  sprintf(batteryMsg, "Batt. Volt: %d", vin);
  u8x8.drawString(0,1,batteryMsg);
    
  // Collect raw analog data (range: 0-1023)
  int valUD = analogRead(joyUD);
  int valLR = analogRead(joyLR);

  char updown[50];
  sprintf(updown, "Val UD: %d", valUD);
  u8x8.drawString(0,4,updown);

  char leftright[50];
  sprintf(leftright, "Val LR: %d", valLR);
  u8x8.drawString(0,5,leftright);

  //ST.motor(1, 0);    // Stop.
  //ST.motor(2, 0);    // Stop.

  // ST.motor(1, 127);  // Go forward at full power.
  // delay(2000);       // Wait 2 seconds.
  // ST.motor(1, 0);    // Stop.
  // delay(2000);       // Wait 2 seconds.
  // ST.motor(1, -127); // Reverse at full power.
  // delay(2000);       // Wait 2 seconds.
  // ST.motor(1, 0);    // Stop.
  // delay(2000);

  // ST.motor(2, 127);  // Go forward at full power.
  // delay(2000);       // Wait 2 seconds.
  // ST.motor(2, 0);    // Stop.
  // delay(2000);       // Wait 2 seconds.
  // ST.motor(2, -127); // Reverse at full power.
  // delay(2000);       // Wait 2 seconds.
  // ST.motor(2, 0);    // Stop.
  // delay(2000);
}
