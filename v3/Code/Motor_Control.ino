//JOYSTICK PINS
int xPosition = 0;
int yPosition = 0;
int xPin = A1;
int yPin = A0;


//MOTOR DIRECTION CONTROLS
int left_motor = 5;
int right_motor = 7;
//MOTOR SPEED CONTROL

int coord_y = 0; 
int coord_x = 0; 


int left_speed = 9;          //PWM Pins for sending to Driver Pin
int right_speed = 10;         //Note, make a low-pass filter to flaten out the signal

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
