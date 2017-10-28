#include "MotorControl.h"

MotorControl::MotorControl()
{
}

void MotorControl::initialize(int initialULLR, int initialLLUR, int threshold)
{
  ullr = initialULLR;
  llur = initialLLUR;
  t = threshold;
}

void MotorControl::translateJoystickToMotorCommands(int upperLeftToLowerRight, int lowerLeftToUpperRight, int &motor1Command, int &motor2Command)
{
  motor1Command = 100;
  motor2Command = 100;
}
