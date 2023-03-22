#define INPUT_SBUS
#include "pimech_controller.hpp"

using namespace PiMechOS;
Groundmech_Controller B2EMO;

void setup()
{
    B2EMO.addMotorShield(48);
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