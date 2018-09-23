/*
  The purpose of this program is to test and demonstrate the display performance of the 128 x 64 display

  A0  Joystick yPin - white coming out of shield, turns to yellow going into joystick
  A1	Joystick xPin - brown coming out of shield, turns to green going into joystick
  A2	Voltage read (5K pot across +5, and ground)
  A3
  A4  SDA (if no SDA pin) - white
  A5  SCL (if not SCL pin) - blue

  D5	Left motor
  D7	Right motor
  D9	Left speed
  D10	Right speed
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 Display(OLED_RESET);

int r = 0;
int i = 0;

//JOYSTICK PINS
int xPosition = 0;
int yPosition = 0;
int xPin = A1;
int yPin = A0;
int voltagePin = A2;


//MOTOR DIRECTION CONTROLS
int left_motor = 5;
int right_motor = 7;
//MOTOR SPEED CONTROL

int coord_y = 0; 
int coord_x = 0; 


int left_speed = 9;          //PWM Pins for sending to Driver Pin
int right_speed = 10;         //Note, make a low-pass filter to flaten out the signal

void updateDisplay(int xPos, int yPos, float voltage)
{
  Display.setTextSize(1);
  Display.setTextColor(WHITE, BLACK);
  Display.setCursor(0, 0);
  Display.print("Battery: ");
  Display.print(Format(voltage, 3, 2));
  Display.println(" V");

  Display.setTextSize(0.5);
  Display.setCursor(0, 10);
  Display.print("X: "); Display.println(Format(xPos, 3, 2));
  Display.print("Y: "); Display.println(Format(yPos, 3, 2));

  /*
  //draw the bar graph
  Display.fillRect(voltage, 50, 128 - voltage, 10, BLACK);
  Display.fillRect(3, 50, voltage, 10, WHITE);

  for (i = 1; i < 13; i++) {
    Display.fillRect(i * 10, 50, 2, 10, BLACK);
  }
  */

  // now that the display is build, display it...
  Display.display();
}

void DrawTitles(void) {
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

void motorForward(int dir,int mot_spd, int spd){          //mot_spd = control which motor to move: left or right
  digitalWrite(dir,LOW);
  analogWrite(mot_spd,spd);
}

void motorBackward(int dir,int mot_spd, int spd){
  digitalWrite(dir,HIGH);
  analogWrite(mot_spd,spd);
}


void motorStop(){
  analogWrite(left_speed,0);
  analogWrite(right_speed,0);
}

void setup() {
  Serial.begin(9600);
  
  //activate Joystick pins
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //Set up Direction outputs for the motor driver inputs
  pinMode(left_motor, OUTPUT);
  pinMode(right_motor, OUTPUT);
  
  //Set up Speed outputs for Motor Driver
  pinMode(left_speed, OUTPUT);
  pinMode(right_speed, OUTPUT);

  pinMode(voltagePin, INPUT);
  Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  Display.clearDisplay();
  Display.display();
  delay (1000);

  DrawTitles();
}

void loop() {
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  
  coord_y = map(yPosition,0,1024,-35,35);         //  Up: 0 Down: 1024
  coord_x = map(xPosition,0,1024,-35,35);         //  Left: 0 Right: 1024
  
  Serial.print("Motor X: ");
  Serial.print(coord_x);
  Serial.print(" | Motor Y: ");
  Serial.print(coord_y);
  Serial.print(" | yPosition: ");
  Serial.print(yPosition);
  Serial.print(" | xPosition: ");
  Serial.println(xPosition);

  if (xPosition < 530 && xPosition > 500 && yPosition < 530 && yPosition > 500)
  {
    //Do Nothing
    motorStop();
    Serial.println("STOP");
  }
  else if (xPosition < 530 && xPosition > 470 && yPosition < 500)
  {
    //Backward
    motorBackward(left_motor,left_speed,abs(coord_y));
    motorBackward(right_motor,right_speed,abs(coord_y)); 
  } 
  else if (xPosition < 530 && xPosition > 470 && yPosition > 500)
  {
    //Forward
    motorForward(left_motor,left_speed,abs(coord_y));
    motorForward(right_motor,right_speed,abs(coord_y));   
  }
  else if (yPosition < 530 && yPosition > 470 && xPosition > 500)
  {   
    //Turn right
    motorForward(left_motor,left_speed,abs(coord_x));
    motorBackward(right_motor,right_speed,abs(coord_x));
  }
  else if (yPosition < 530 && yPosition > 470 && xPosition < 500)
  {
    //Turn left
    motorBackward(left_motor,left_speed,abs(coord_x));
    motorForward(right_motor,right_speed,abs(coord_x));
  }

  float resistance = analogRead(voltagePin);
  float voltage = resistance / 204.6;

  updateDisplay(coord_x, coord_y, voltage);
}
