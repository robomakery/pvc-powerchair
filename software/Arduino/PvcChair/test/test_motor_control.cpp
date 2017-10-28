#include "catch.hpp"
#include "../MotorControl.h"

TEST_CASE( "translateJoystickToMotorCommands returns 100" ) {
  MotorControl controller = MotorControl();
  controller.initialize(0, 0, 10);

  int motorCommand1;
  int motorCommand2;
  controller.translateJoystickToMotorCommands(0, 0, motorCommand1, motorCommand2);
  REQUIRE( motorCommand1 == 100 );
  REQUIRE( motorCommand2 == 100 );
}
