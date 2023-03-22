/********************************/
/*      Third checkpoint        */
/*     OSU FEH Spring 2023      */
/*           Team A             */
/*          03/30/23            */
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

/* Declared Robot Movement Methods */

/* Moves the robot forward */
void moveForward(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder, int clicks, int power);

/* Moves the robot backward */
void moveBackward(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder, int clicks, int power);

/* Turns the robot left */
void turnLeft(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder,  int clicks, int power);

/* Turns the robot right */
void turnRight(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder,  int clicks, int power);

int main(void) {
    FEHMotor leftMotor(FEHMotor::Motor0, 9.0);
    FEHMotor rightMotor(FEHMotor::Motor1, 9.0);
    DigitalEncoder leftEncoder(FEHIO::P0_0);
    DigitalEncoder rightEncoder(FEHIO::P0_1);
    FEHServo leverServo(FEHServo::Servo0);
    AnalogInputPin cds(FEHIO::P1_1);

    /* Resets the encoders to calibrate */
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    //sets servo values
    leverServo.SetMin(SERVO_MIN);
    leverServo.SetMax(SERVO_MAX);

    
    while(true){
        leverServo.SetDegree(90);
        Sleep(2000);
        leverServo.SetDegree(0);
        Sleep(2000);

    }

    /* Waits until robot reads a value from the starting light */
    while (cds.Value() > 0.5) {
        // (just random writing)
        LCD.Write("~[,,_,,]:3");
        LCD.Clear();
    }

    //move forward diagonally
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * START_TO_READY_FOR_TURN), 25);

    // turn left
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * LEFT_TURN), 20);

    // move forward until in front of the levers
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * TURN_TO_IN_FRONT_OF_LEVERS), 25);

    // turn towards the levers
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * NINETY_DEGREES), 20);

    // move into the levers
    //moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * INTO_LEVERS), 25);

    // flip levers down

    // go back
    moveBackward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * INTO_LEVERS), 25);

    // wait 5 seconds
    Sleep(5000);

    // move into levers again
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * INTO_LEVERS), 25);

    // flip levers up

    // move back
    moveBackward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * INTO_LEVERS), 25);
}

/* Defined Robot Movement Methods */
void moveForward(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder, int clicks, int power) {
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    leftMotor.SetPercent(power);
    rightMotor.SetPercent(-1 * power);

    while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

    leftMotor.Stop();
    rightMotor.Stop();
}

void moveBackward(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder, int clicks, int power) {
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    leftMotor.SetPercent(-1 * power);
    rightMotor.SetPercent(power);

    while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

    leftMotor.Stop();
    rightMotor.Stop();
}

void turnLeft(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder,  int clicks, int power) {
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    leftMotor.SetPercent(-1 * power);
    rightMotor.SetPercent(-1 * power);

    while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

    leftMotor.Stop();
    rightMotor.Stop();
}

void turnRight(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder,  int clicks, int power) {
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    leftMotor.SetPercent(power);
    rightMotor.SetPercent(power);

    while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

    leftMotor.Stop();
    rightMotor.Stop();
}
