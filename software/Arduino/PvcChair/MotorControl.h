#ifndef H_MOTOR_CONTROL
#define H_MOTOR_CONTROL

class MotorControl
{
  int ullr;
  int llur;
  int t;

public:
  MotorControl();

  void initialize(int initialULLR, int initialLLUR, int threshold);

  void translateJoystickToMotorCommands(int upperLeftToLowerRight, int lowerLeftToUpperRight, int &motor1Command, int &motor2Command);
};

#endif // H_MOTOR_CONTROL
