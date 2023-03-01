/********************************/
/*      First Checkpoint        */
/*     OSU FEH Spring 2023      */
/*           Team A             */
/*          02/27/22            */
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

/* Declared Robot Movement Methods */

/* Moves the robot forward */
void moveForward(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder, int clicks, int power);

/* Moves the robot backward */
void moveBackward(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder, int clicks, int power);

/* Turns the robot left */
void turnLeft(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder,  int clicks, int power);

/* Turns the robot right */
void turnRight(FEHMotor leftMotor, FEHMotor rightMotor, DigitalEncoder leftEncoder, DigitalEncoder rightEncoder,  int clicks, int power);

/* Rotates the servo */
void rotateServo(FEHServo servo, int degrees);

int main(void) {
    FEHMotor leftMotor(FEHMotor::Motor0, 9.0);
    FEHMotor rightMotor(FEHMotor::Motor1, 9.0);
    DigitalEncoder leftEncoder(FEHIO::P0_0);
    DigitalEncoder rightEncoder(FEHIO::P0_1);
    FEHServo frontServo(FEHServo::Servo0);
    FEHServo rearServo(FEHServo::Servo1);
    AnalogInputPin cds(FEHIO::P1_1);

    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    while (cds.Value() > 0.5) {
        LCD.Write("~[,,_,,]:3");
        LCD.Clear();
    }
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 5.0), 25);
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 42.0), 20);
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 34.0), 25);
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 90.0), 20);
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 8.0), 25);
    turnRight(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 100.0), 20);
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 16.0), 25);
    moveBackward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 16.0), 25);
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 90.0), 20);
    moveBackward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 20), 25);
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 5.0), 25);
    turnRight(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 100.0), 20);
    moveBackward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 35.0), 25);
    leftMotor.Stop();
    rightMotor.Stop();
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

void rotateServo(FEHServo servo, int degrees) {

}
