#pragma once
#include <Arduino.h>
#include <array>
// PIO Servo
#include <pico.h>
#include "hardware/dma.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pio_servo.hpp"
// JSON
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
// PPM
// #define DEBUG_PPM
#ifndef PPM_CHs
#define PPM_CHs 8
#endif
#ifndef PPM_PIN
#define PPM_PIN 26
#endif
// SBUS
#include <sbus.h>
#ifndef SBUS_PIN
#define SBUS_PIN 27
#endif
#define SBUS_DEBUG_READ
#define SBUS_QUEUE_START 0xFFFFFFFF
extern std::array<int16_t,18> sbus_data;

#define GND_BUFFER_SIZE 32

#define JOYSTICK_L 0
#define JOYSTICK_R 1

class Groundmech
{
public:
    Groundmech(char* config);
    Groundmech(void);

    void init(void);
    void begin(void);
    void loop(void);
    void setServoPos(int servoID,int nextPos);
    int getServoPos(int servoID);

    int ppmch[PPM_CHs];
    int ppmOut=0;
    void PPMinit(void);
    void PPMupdate(void) {
        int ppmInit = pulseIn(PPM_PIN, HIGH);
        if(ppmInit > 4000) //If pulse > 4 miliseconds, continues
        {
            ppmOut = 0;
            for(int i = 0; i < PPM_CHs; i++) //Read the pulses of the remainig channels
            {
                ppmch[i]=pulseIn(PPM_PIN, HIGH);
            }
            #ifdef DEBUG_PPM
            Serial.print("CHi:"); //Channel
            Serial.print(ppmInit); //Print the value
            Serial.print("\t"); 
            for(int i = 0; i < PPM_CHs; i++) //Prints all the values readed
            {
                Serial.print("CH"); //Channel
                Serial.print(i); //Channel number
                Serial.print(":"); Serial.print(ppmch[i]); //Print the value
                Serial.print("\t"); 
            }
            Serial.println(";");
            //delay(10); //Give time to print values.
            #endif
        }else{
            ppmOut++;
            if (ppmOut>2)
            {
                for(int i = 0; i < PPM_CHs; i++)
                {
                    ppmch[i]=0;
                }
            }
            Serial.print("CHi:"); //Channel
            Serial.print(ppmInit); //Print the value
            Serial.print("\t ppmOut:"); //Channel
            Serial.print(ppmOut); //Print the value
            Serial.println(";");
        };
    };


private:
    byte mechState = 0x00;
    byte mechStateN = 0x00;
    int servoMin[24];
    int servoMax[24];
    int servoPos[24];
    int servoPosNext[24];
    int servoPosPrev[24];
    int servoPosSpeed[24];
    char* configstring;
    unsigned cmdPos;
    char cmdBuffer[GND_BUFFER_SIZE];
    String getValue(String data, char separator, int index);
    // remote control input
    int remoteData[23];

    int getServoMin(int servoID);
    int getServoMax(int servoID);
    int isServoReversed(int servoID);

    void updateState(int state);
    void receiveData(char ch);
    void processCommands(char* input);
    // Communication Inputs
    void sbusBegin(void);

    void updateDriveMotors(int x, int y, int z);

    int remoteConnected(void);
    int ButtonPressed(int buttonID);
    int ButtonDoublePressed(int buttonID);
    int ButtonHeld(int buttonID);
    float JoystickX(int joystickID);
    float JoystickY(int joystickID);
    float JoystickDirection(int joystickID);
    float JoystickStrength(int joystickID);
    int switchChanged(int switchID);

    /*-------------------------------------------------------- /
    /  Easing Functions - https://github.com/ai/easings.net    /
    / --------------------------------------------------------*/

    float easeInSine(float x) {
        return 1 - cos((x * 3.14159) / 2);
    }

    float easeOutSine(float x) {
        return sin((x * 3.14159) / 2);
    }

    float easeInOutSine(float x) {
        return -(cos(3.14159 * x) - 1) / 2;
    }

    float easeInQuad(float x) {
        return x * x;
    }

    float easeOutQuad(float x) {
        return 1 - (1 - x) * (1 - x);
    }

    float easeInOutQuad(float x) {
        return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
    }

    float easeInCubic(float x) {
        return x * x * x;
    }

    float easeOutCubic(float x) {
        return 1 - pow(1 - x, 3);
    }

    float easeInOutCubic(float x) {
        return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
    }

    float easeInQuart(float x) {
        return x * x * x * x;
    }

    float easeOutQuart(float x) {
        return 1 - pow(1 - x, 4);
    }

    float easeInOutQuart(float x) {
        return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
    }

    float easeInQuint(float x) {
        return x * x * x * x * x;
    }

    float easeOutQuint(float x) {
        return 1 - pow(1 - x, 5);
    }

    float easeInOutQuint(float x) {
        return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
    }

    float easeInExpo(float x) {
        return x == 0 ? 0 : pow(2, 10 * x - 10);
    }

    float easeOutExpo(float x) {
        return x == 1 ? 1 : 1 - pow(2, -10 * x);
    }

    float easeInOutExpo(float x) {
        return x == 0
            ? 0 : x == 1
            ? 1 : x < 0.5
            ? pow(2, 20 * x - 10) / 2 : (2 - pow(2, -20 * x + 10)) / 2;
    }

    float easeInCirc(float x) {
        return 1 - sqrt(1 - pow(x, 2));
    }

    float easeOutCirc(float x) {
        return sqrt(1 - pow(x - 1, 2));
    }

    float easeInOutCirc(float x) {
        return x < 0.5
            ? (1 - sqrt(1 - pow(2 * x, 2))) / 2
            : (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
    }

    float easeInBounce(float x) {
        return 1 - easeOutBounce(1 - x);
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

};

using namespace std;
//ToString
inline String ToString (double d)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%g", d);
	return buffer;
}
inline String ToString (float d)
{
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%g", d);
	return buffer;
}
inline String ToString (char a)
{
	return String(a).c_str();
}
inline String ToString (const char* a)
{
	return String(a).c_str();
}
inline String ToString (int number)
{
	return String(number).c_str();
}
inline String ToString (int number, String thing)
{
	String Prefix = "";
	if (thing == "00")
	{
		if (number <= 9)
		{
			Prefix = "0";	
		}
	}
	return (Prefix + String(number)).c_str();
}
inline String ToString (bool d)
{
	if (d)
	{
		return "true";	
	}
	return "false";
}