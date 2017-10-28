#include "catch.hpp"
#include "../MotorControl.h"

TEST_CASE( "isNegative returns 0" ) {
  REQUIRE( isNegative(0) == 0 );
}
