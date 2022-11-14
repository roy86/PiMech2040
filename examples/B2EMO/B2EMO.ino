#include "B2.h"

#ifndef PPM_CHs
#define PPM_CHs 8 // How many channels have your radio (max 8 PPM)
#endif

#ifndef PPM_PIN
#define PPM_PIN 26
#endif
//#define SMOOTHING 0.08

GroundMechB2 B2EMO;

Servo Motor_Body_Lower;
Servo Motor_Body_Upper;
Servo Motor_Head_Lift;
Servo Motor_Head_AxisL;
Servo Motor_Head_AxisR;
Servo Motor_Head_AxisY;

void setup()
{
  Serial.begin(9600);

  B2EMO.begin();

  Motor_Body_Lower.attach(0,500,2500);
  Motor_Body_Upper.attach(1,500,2500);
  Motor_Head_Lift.attach( 2,500,2500);
  Motor_Head_AxisL.attach(3,500,2500);
  Motor_Head_AxisR.attach(4,500,2500);
  Motor_Head_AxisY.attach(5,500,2500);
}

void loop()
{

  B2EMO.update();

  Motor_Body_Lower.write( B2EMO.getServoPos(TORSO_MOTOR_LOWERLIFT) );
  Motor_Body_Upper.write( B2EMO.getServoPos(TORSO_MOTOR_UPPERLIFT) );
  Motor_Head_Lift.write(B2EMO.getServoPos(HEAD_MOTOR_LIFT));
  Motor_Head_AxisY.write(B2EMO.getServoPos(HEAD_MOTOR_TURN));
  Motor_Head_AxisL.write(B2EMO.getServoPos(HEAD_MOTOR_NECKL));
  Motor_Head_AxisR.write(B2EMO.getServoPos(HEAD_MOTOR_NECKR));

  delay(10);
}

