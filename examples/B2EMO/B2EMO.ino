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

// Mecanum Drive: 3 PIN Output
//- Servo Motor_Drive_X;
//- Servo Motor_Drive_Y;
//- Servo Motor_Drive_Z;

// Mecanum Drive: 4 PIN Output
Servo Motor_Drive_FL;
Servo Motor_Drive_FR;
Servo Motor_Drive_RL;
Servo Motor_Drive_RR;

void setup()
{
  Serial.begin(9600);

  B2EMO.begin();
  B2EMO.setMecanum(0); // 0: 4x PWM Output, 1: 3x PWM output, 2: RAW Signal Output

  // Mecanum Drive: 3 PIN Output
  //- Motor_Drive_X.attach(6,500,2500);
  //- Motor_Drive_Y.attach(7,500,2500);
  //- Motor_Drive_Z.attach(8,500,2500);
  // Mecanum Drive: 4 PIN Output
  Motor_Drive_FL.attach(0,500,2500);
  Motor_Drive_FR.attach(1,500,2500);
  Motor_Drive_RL.attach(2,500,2500);
  Motor_Drive_RR.attach(3,500,2500);
  // Lift Servos
  Motor_Body_Lower.attach(4,500,2500);
  Motor_Body_Upper.attach(5,500,2500);
  Motor_Head_Lift.attach( 6,500,2500);
  // Head Movement
  Motor_Head_AxisL.attach(7,500,2500);
  Motor_Head_AxisR.attach(8,500,2500);
  Motor_Head_AxisY.attach(9,500,2500);
}

void loop()
{

  B2EMO.update();

  Motor_Body_Lower.write( B2EMO.getServoPos(TORSO_MOTOR_LOWERLIFT) );
  Motor_Body_Upper.write( B2EMO.getServoPos(TORSO_MOTOR_UPPERLIFT) );
  Motor_Head_Lift.write(  B2EMO.getServoPos(HEAD_MOTOR_LIFT));
  Motor_Head_AxisY.write( B2EMO.getServoPos(HEAD_MOTOR_TURN));
  Motor_Head_AxisL.write( B2EMO.getServoPos(HEAD_MOTOR_NECKL));
  Motor_Head_AxisR.write( B2EMO.getServoPos(HEAD_MOTOR_NECKR));

  Motor_Head_AxisY.write( B2EMO.getServoPos(HEAD_MOTOR_TURN));
  Motor_Head_AxisL.write( B2EMO.getServoPos(HEAD_MOTOR_NECKL));
  Motor_Head_AxisR.write( B2EMO.getServoPos(HEAD_MOTOR_NECKR));

  // Mecanum Drive: 3 PIN Output
  //- Motor_Drive_X.write(B2EMO.getServoPos(DRIVE_MOTOR_X));
  //- Motor_Drive_Y.write(B2EMO.getServoPos(DRIVE_MOTOR_Y));
  //- Motor_Drive_Z.write(B2EMO.getServoPos(DRIVE_MOTOR_Z));
  // Mecanum Drive: 4 PIN Output
  Motor_Drive_FL.write(B2EMO.getServoPos(DRIVE_MOTOR_FRONTLEFT));
  Motor_Drive_FR.write(B2EMO.getServoPos(DRIVE_MOTOR_FRONTRIGHT));
  Motor_Drive_RL.write(B2EMO.getServoPos(DRIVE_MOTOR_REARLEFT));
  Motor_Drive_RR.write(B2EMO.getServoPos(DRIVE_MOTOR_REARRIGHT));

  // Debugging
  //Serial.print("FL:"); Serial.print(B2EMO.getServoPos(DRIVE_MOTOR_FRONTLEFT)); Serial.print("\t");
  //Serial.print("FR:"); Serial.print(B2EMO.getServoPos(DRIVE_MOTOR_FRONTRIGHT)); Serial.print("\t");
  //Serial.print("RL:"); Serial.print(B2EMO.getServoPos(DRIVE_MOTOR_REARLEFT)); Serial.print("\t");
  //Serial.print("RR:"); Serial.print(B2EMO.getServoPos(DRIVE_MOTOR_REARRIGHT)); Serial.println("\t");
}

