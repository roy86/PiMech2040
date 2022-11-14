#include <Arduino.h>
#include <Servo.h>
#include <math.h>

#ifndef PPM_CHs
#define PPM_CHs 8 // How many channels have your radio (max 8 PPM)
#endif
#ifndef PPM_PIN
#define PPM_PIN 26
#endif

#define SMOOTHING 0.08

#define DRIVE 0x00
#define BODY 0x01

#define DRIVE_MOTOR_FRONTLEFT   0x00
#define DRIVE_LIFT_FRONTLEFT    0x01
#define DRIVE_EXTEND_FRONTLEFT  0x02
#define DRIVE_MOTOR_FRONTRIGHT  0x03
#define DRIVE_LIFT_FRONTRIGHT   0x04
#define DRIVE_EXTEND_FRONTRIGHT 0x05
#define DRIVE_MOTOR_REARLEFT    0x06
#define DRIVE_LIFT_REARLEFT     0x07
#define DRIVE_EXTEND_REARLEFT   0x08
#define DRIVE_MOTOR_REARRIGHT   0x09
#define DRIVE_LIFT_REARRIGHT    0x0A
#define DRIVE_EXTEND_REARRIGHT  0x0B
#define TORSO_MOTOR_LOWERRING   0x0C
#define TORSO_MOTOR_UPPERRING   0x0D
#define TORSO_MOTOR_LOWERLIFT   0x0E
#define TORSO_MOTOR_UPPERLIFT   0x0F
#define HEAD_MOTOR_LIFT         0x10
#define HEAD_MOTOR_NECKL        0x11
#define HEAD_MOTOR_NECKR        0x12
#define HEAD_MOTOR_TURN         0x13

class GroundMechB2 {
public:
    GroundMechB2() {
    }

    void begin()
    {
        Serial.begin(9600);
        Serial.println("INIT B2 Groundmech Completed");
        pinMode(PPM_PIN,INPUT);

        for (int i=0;i<4;i++){emote[i]=0;};
        for (int i=0;i<19;i++){servos[i]=0;};
    }

    void update()
    {
        processPPM();
        //delay(10);
        //processDriveMotors(ppmch[0],ppmch[1],ppmch[2]);
        processLiftMotors(ppmch[5],ppmch[4]);
        processHeadMotors(ppmch[2],ppmch[3],ppmch[1]);
    }

    void processPPM()
    {
        if(pulseIn(PPM_PIN, HIGH) > 4000) //If pulse > 4 miliseconds, continues
        {
            for(int i = 0; i < PPM_CHs; i++) //Read the pulses of the remainig channels
            {
                ppmch[i]=pulseIn(PPM_PIN, HIGH);
            }

            // for(int i = 0; i < PPM_CHs; i++) //Prints all the values readed
            // {
            //     Serial.print("CH"); //Channel
            //     Serial.print(i); //Channel number
            //     Serial.print(":"); Serial.print(ppmch[i]); //Print the value
            //     Serial.print(" "); 
            // }
            // Serial.println(";");
            delay(10); //Give time to print values.
        };
    }

    void processLiftMotors(int l,int h)
    {
        int inputLift = constrain(map(l,600,1600,0,180),0,180);
        int inputHead = constrain(map(h,600,1600,0,180),0,180);
        int LowerLiftPos = servos[TORSO_MOTOR_LOWERLIFT];
        int UpperLiftPos = servos[TORSO_MOTOR_UPPERLIFT];
        int HeadLiftPos = servos[HEAD_MOTOR_LIFT];

        LowerLiftPos = constrain(LowerLiftPos + (inputLift - LowerLiftPos) * SMOOTHING,0,180);
        HeadLiftPos = constrain(HeadLiftPos + (inputHead - HeadLiftPos) * SMOOTHING,0,180);

        servos[TORSO_MOTOR_LOWERLIFT] = LowerLiftPos;
        servos[TORSO_MOTOR_UPPERLIFT] = 180 - LowerLiftPos;
        servos[HEAD_MOTOR_LIFT] = HeadLiftPos;
    }

    float easeInOutExpo(float x)
    {
        return x == 0 ? 0 : x == 1
            ? 1 : x < 0.5
            ? pow(2, 20 * x - 10) / 2
            : (2 - pow(2, -20 * x + 10)) / 2;
    }

    float easeInOutQuint(float x)
    {
        return x < 0.5 ? 16.00 * x * x * x * x * x : 1.00 - pow(-2 * x + 2, 5) / 2;
    }

    float easeOutBounce(float x) {
        float n1 = 7.5625;
        float d1 = 2.75;

        if (x < 1 / d1) {
            return n1 * x * x;
        } else if (x < 2 / d1) {
            return n1 * (x -= 1.5 / d1) * x + 0.75;
        } else if (x < 2.5 / d1) {
            return n1 * (x -= 2.25 / d1) * x + 0.9375;
        } else {
            return n1 * (x -= 2.625 / d1) * x + 0.984375;
        }
    }

    float easeInOutBounce(float x) {
        return x < 0.5
            ? (1 - easeOutBounce(1 - 2 * x)) / 2
            : (1 + easeOutBounce(2 * x - 1)) / 2;
    }

    void processHeadMotors(int x,int y,int z)
    {
        int inputX = constrain(map(x,600,1600,0,180),0,180);
        int inputY = constrain(map(y,600,1600,0,180),0,180);
        int inputZ = constrain(map(z,600,1600,0,180),0,180);

        int NeckLPos = servos[HEAD_MOTOR_NECKL];
        int NeckRPos = servos[HEAD_MOTOR_NECKR];
        int TurnPos  = servos[HEAD_MOTOR_TURN];

        NeckLPos = constrain(NeckLPos + (inputX - NeckLPos) * SMOOTHING,0,180);
        NeckRPos = constrain(NeckRPos + (180 - inputX - NeckRPos) * SMOOTHING,0,180);

        NeckLPos = constrain(NeckLPos + (180 - inputY - NeckLPos) * SMOOTHING,0,180);
        NeckRPos = constrain(NeckRPos + (180 - inputY - NeckRPos) * SMOOTHING,0,180);

        TurnPos  = constrain(TurnPos + (inputZ - TurnPos) * SMOOTHING,0,180);

        servos[HEAD_MOTOR_NECKL] = NeckLPos;
        servos[HEAD_MOTOR_NECKR] = NeckRPos;
        servos[HEAD_MOTOR_TURN] = TurnPos;

        //Serial.print("z:"); Serial.print(z); Serial.print(" ");
        //Serial.print("inputZ:"); Serial.print(inputZ); Serial.print(" ");
        //Serial.print("TurnPos:"); Serial.print(TurnPos); Serial.print(" ");
        //Serial.println(";");
    }

    int getServoPos(int sid)
    {
        sid = constrain(sid,0x00,0x13);
        return servos[sid];
    }

    /* int getEmote(int e){
      e = constrain(e,0,3);
      return emote[e];
    } */

    /* void setEmote(int e,int v){
      e = constrain(e,0,3);
      v = constrain(v,0,99);
      emote[e]=v;
    } */

private:
    int emote[4];
    int ppmch[PPM_CHs];
    int servos[19];
};
