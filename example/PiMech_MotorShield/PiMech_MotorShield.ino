#include "pimech_motorshield.hpp"
#include <Wire.h>

using namespace PiMechOS;
Groundmech_MotorDriver B2EMO(0x30);

void setup()
{
  B2EMO.begin();
}

void setup1()
{
  B2EMO.begin1();
}

void loop()
{
  B2EMO.loop();
}

void loop1()
{
  B2EMO.loop1();
}