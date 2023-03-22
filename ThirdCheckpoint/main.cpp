/********************************/
/*      Third checkpoint        */
/*     OSU FEH Spring 2023      */
/*           Team A             */
/*          03/22/23            */
/********************************/

/* Include Preprocessor Directives */
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHRPS.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>

#include <Drive.h>

#define CLICKS_PER_INCH 400.0/12.0
#define CLICKS_PER_DEGREE CLICKS_PER_INCH * 0.065

//DISTANCE VALUES FOR THE THIRD CHECKPOINT

#define START_TO_READY_FOR_TURN 18
#define LEFT_TURN 45
#define TURN_TO_IN_FRONT_OF_LEVERS 8
#define NINETY_DEGREES 95
#define INTO_LEVERS 5

// SERVO VALUES
#define SERVO_MAX 2390
#define SERVO_MIN 500

int main(void) {
    /* FEHMotor leftMotor(FEHMotor::Motor0, 9.0);
    FEHMotor rightMotor(FEHMotor::Motor1, 9.0); */
    //DigitalEncoder leftEncoder(FEHIO::P0_0);
    //DigitalEncoder rightEncoder(FEHIO::P0_1);
    FEHServo leverServo(FEHServo::Servo0);
    AnalogInputPin cds(FEHIO::P1_1);

    Drive drive(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::P0_0, FEHIO::P0_1); 

    //sets servo values
    leverServo.SetMin(SERVO_MIN);
    leverServo.SetMax(SERVO_MAX);

    /*
    while(true){
        leverServo.SetDegree(90);
        Sleep(2000);
        leverServo.SetDegree(0);
        Sleep(2000);

    }*/

    leverServo.SetDegree(180);

    /* Waits until robot reads a value from the starting light */
    while (cds.Value() > 0.5) {

        LCD.Write("Waiting to start");
        LCD.Clear();
    }

    //move forward diagonally
    drive.Straight(START_TO_READY_FOR_TURN);

    // turn left
    drive.TurnLeft(LEFT_TURN);

    // move forward until in front of the levers
    drive.Straight(TURN_TO_IN_FRONT_OF_LEVERS);

    // turn towards the levers
    drive.TurnLeft(NINETY_DEGREES);

    // move into the levers
    //moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * INTO_LEVERS), 25);

    //flip down (20 deg down)
    leverServo.SetDegree(160);
    Sleep(1000);
    
    // go back
    drive.Back(INTO_LEVERS);

    // flip arm down
    leverServo.SetDegree(90);

    // wait 5 seconds
    Sleep(5000);

    // move into levers again
    drive.Straight(INTO_LEVERS);

    // flip levers up
    leverServo.SetDegree(70);
    Sleep(1000);
    leverServo.SetDegree(90);

    // move back
    drive.Back(INTO_LEVERS);
}

