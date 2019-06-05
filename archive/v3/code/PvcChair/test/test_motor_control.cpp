#include "catch.hpp"
#include "../MotorControl.h"

TEST_CASE( "MotorControl", "[motor_control]" ) {
  MotorControl controller = MotorControl();
  controller.initialize(512, 512, 10); // halfway points on sensors; 10 is threshold

  int motorCommand1;
  int motorCommand2;
  SECTION( "neutral under threshold" ) {
    controller.translateJoystickToMotorCommands(520, 510, motorCommand1, motorCommand2);
    REQUIRE( motorCommand1 == 0 );
    REQUIRE( motorCommand2 == 0 );
  }
  SECTION( "Full Forward" ) {
    controller.translateJoystickToMotorCommands(520, 1023, motorCommand1, motorCommand2);
    REQUIRE( motorCommand1 >= 120 );
    REQUIRE( motorCommand2 >= 120 );
  }
  SECTION( "Full Reverse" ) {
    controller.translateJoystickToMotorCommands(520, 0, motorCommand1, motorCommand2);
    REQUIRE( motorCommand1 <= -120 );
    REQUIRE( motorCommand2 <= -120 );
  }
}
