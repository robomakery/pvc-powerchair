#ifndef H_MOTOR_CONTROL
#define H_MOTOR_CONTROL

class MotorControl
{
  int centerX;
  int centerY;
  int t;

public:
  MotorControl();
  void initialize(int initialX, int initialY, int threshold);
  void translateJoystickToMotorCommands(int x, int y, int &motor1Command, int &motor2Command);

private:
  long mymap(long, long, long, long, long);

};

#endif // H_MOTOR_CONTROL
