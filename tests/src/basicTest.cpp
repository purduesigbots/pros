#include "catch.hpp"

#include <ostream>
#include <iostream>

TEST_CASE( "Basic Test", "[basic]" )
{
  REQUIRE(1 == 1);
  std::cout << "Hello World" << std::flush;
}
