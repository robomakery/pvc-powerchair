/*
  The purpose of this program is to test and demonstrate the display performance of the 128 x 64 display

  A0    Voltage read (5K pot across +5, and ground)
  A1
  A2
  A3
  A4    SDA (if no SDA pin)
  A5    SCL (if not SCL pin)

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ADJ_PIN A0
#define OLED_RESET 4
Adafruit_SSD1306 Display(OLED_RESET);

int r = 0;
int i = 0;

void setup()   {

  Serial.begin(9600);
  pinMode(ADJ_PIN, INPUT);
  Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  Display.clearDisplay();
  Display.display();
  delay (1000);

  DrawTitles();

}


void loop() {

  r = analogRead(ADJ_PIN);
  r = r / 7.98;

  Display.setTextSize(2);
  // note set the background color or the old text will still display
  Display.setTextColor(WHITE, BLACK);
  Display.setCursor(0, 33);
  Display.println(Format(r * 7.99 / 204.6, 3, 2));

  //draw the bar graph
  Display.fillRect(r, 50, 128 - r, 10, BLACK);
  Display.fillRect(3, 50, r, 10, WHITE);

  for (i = 1; i < 13; i++) {
    Display.fillRect(i * 10, 50, 2, 10, BLACK);
  }

  // now that the display is build, display it...
  Display.display();

}


void DrawTitles(void) {

  Display.setTextSize(2);
  Display.setTextColor(WHITE);
  Display.setCursor(0, 0);
  Display.println("Bar Graph");

  Display.setTextSize(1);
  Display.setTextColor(WHITE);
  Display.setCursor(0, 19);
  Display.println("Measured Volts");
  //Display.println("Random number");
  Display.display();

}


String Format(double val, int dec, int dig ) {

  int addpad = 0;
  char sbuf[20];
  String fdata = (dtostrf(val, dec, dig, sbuf));
  int slen = fdata.length();
  for ( addpad = 1; addpad <= dec + dig - slen; addpad++) {
    fdata = " " + fdata;
  }
  return (fdata);

}
