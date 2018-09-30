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

#define OLED_RESET 4
Adafruit_SSD1306 oledDisplay(OLED_RESET);

int r = 0;
int i = 0;

// JOYSTICK PINS
int xPosition = 0;
int yPosition = 0;
int xPin = A1;
int yPin = A0;
int voltagePin = A2;

// MOTOR DIRECTION CONTROLS
int leftMotor = 12;
int rightMotor = 7;

// MOTOR SPEED CONTROL
int coord_y = 0; 
int coord_x = 0; 

int leftSpeed = 9;	 // PWM Pins for sending to Driver Pin
int rightSpeed = 10; // Note, make a low-pass filter to flaten out the signal

int leftEnabled = 4;
int rightEnabled = 6;

String formatDouble(double val, int dec, int dig )
{
	int addpad = 0;
	char sbuf[20];
	String fdata = (dtostrf(val, dec, dig, sbuf));
	int slen = fdata.length();
	for ( addpad = 1; addpad <= dec + dig - slen; addpad++) {
	  fdata = " " + fdata;
	}
	return (fdata);

}

void updateDisplay(int xPos, int yPos, float batteryPercentage)
{
	oledDisplay.setTextSize(1);
	oledDisplay.setTextColor(WHITE, BLACK);
	oledDisplay.setCursor(0, 0);
	oledDisplay.print("Battery: ");
	oledDisplay.print(formatDouble(batteryPercentage * 100, 3, 2));
	oledDisplay.println(" %");

	oledDisplay.setTextSize(0.5);
	oledDisplay.setCursor(0, 10);
	oledDisplay.print("X: "); oledDisplay.println(formatDouble(xPos, 3, 2));
	oledDisplay.print("Y: "); oledDisplay.println(formatDouble(yPos, 3, 2));

	/*
	//draw the bar graph
	oledDisplay.fillRect(voltage, 50, 128 - voltage, 10, BLACK);
	oledDisplay.fillRect(3, 50, voltage, 10, WHITE);

	for (i = 1; i < 13; i++) {
	  oledDisplay.fillRect(i * 10, 50, 2, 10, BLACK);
	}
	*/

	// now that the display is build, display it...
	oledDisplay.display();
}

void motorForward(int dir, int mot_spd, int spd)
{
	//mot_spd = control which motor to move: left or right
	digitalWrite(dir,LOW);
	analogWrite(mot_spd, spd);
}

void motorBackward(int dir,int mot_spd, int spd)
{
	digitalWrite(dir, HIGH);
	analogWrite(mot_spd, spd);
}

void motorStop()
{
	analogWrite(leftSpeed, 0);
	analogWrite(rightSpeed, 0);
}

void setup()
{
	Serial.begin(9600);
	
	// Activate Joystick pins
	pinMode(xPin, INPUT);
	pinMode(yPin, INPUT);

	// Activate Joystick pins
	pinMode(leftEnabled, OUTPUT);
	pinMode(rightEnabled, OUTPUT);

	digitalWrite(leftEnabled,HIGH);
	digitalWrite(rightEnabled,HIGH);

	// Set up Direction outputs for the motor driver inputs
	pinMode(leftMotor, OUTPUT);
	pinMode(rightMotor, OUTPUT);
	
	// Set up Speed outputs for Motor Driver
	pinMode(leftSpeed, OUTPUT);
	pinMode(rightSpeed, OUTPUT);

	pinMode(voltagePin, INPUT);
	oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

	oledDisplay.clearDisplay();
	oledDisplay.display();
	delay (1000);
}

void loop()
{
	xPosition = analogRead(xPin);
	yPosition = analogRead(yPin);

	// Downscaled power values from joystick. The motor controller takes
	// values between 0 and 256 and we're downscaling to 32 for now.
	coord_y = map(yPosition,0,1024,-64,64);         //  Up: 0 Down: 1024
	coord_x = map(xPosition,0,1024,-64,64);         //  Left: 0 Right: 1024

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
		motorStop();
		Serial.println("STOP");
	}
	else if (xPosition < 530 && xPosition > 470 && yPosition < 500)
	{
		// Backward
		motorBackward(leftMotor,leftSpeed,abs(coord_y));
		motorBackward(rightMotor,rightSpeed,abs(coord_y)); 
	} 
	else if (xPosition < 530 && xPosition > 470 && yPosition > 500)
	{
		// Forward
		motorForward(leftMotor,leftSpeed,abs(coord_y));
		motorForward(rightMotor,rightSpeed,abs(coord_y));   
	}
	else if (yPosition < 530 && yPosition > 470 && xPosition > 500)
	{   
		// Turn right
		motorForward(leftMotor,leftSpeed,abs(coord_x));
		motorBackward(rightMotor,rightSpeed,abs(coord_x));
	}
	else if (yPosition < 530 && yPosition > 470 && xPosition < 500)
	{
		// Turn left
		motorBackward(leftMotor,leftSpeed,abs(coord_x));
		motorForward(rightMotor,rightSpeed,abs(coord_x));
	}

	// Compute voltage from the resistor value
	//
	float resistance = analogRead(voltagePin);

	updateDisplay(coord_x, coord_y, resistance / 1024.0f);
}
