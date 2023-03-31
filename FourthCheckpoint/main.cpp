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
    DigitalEncoder leftEncoder(FEHIO::P0_1);
    DigitalEncoder rightEncoder(FEHIO::P0_0);
    FEHServo servo(FEHServo::Servo0);
    AnalogInputPin cds(FEHIO::P1_1);

    /* Resets the encoders to calibrate */
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    /* Servo Calibration*/
    servo.SetMin(500);
    servo.SetMax(2500);

    /* Waits until robot reads a value from the starting light */
    //LCD.Write("~[,,_,,]:3");
    //while (cds.Value() > 0.5) {};
    //LCD.Clear();

    /* Moves bar up */
    rotateServo(servo, 75);
    /* Moves forward to position */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 2.0), 25);
    /* Turns right to position */
    turnRight(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 90.0), 25);
    /* Moves forward to position */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 4.0), 25);
    /* Turns left to position, facing right-most ramp */
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 38.0), 25);
    /* Moves forward up the ramp, stopping in front of the passport mechanism */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 30.0), 50);
    /* Turns left to position */
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 120.0), 25);
    /* Moves bar down */
    rotateServo(servo, 5);
    /* Moves backward to square up against wall */
    moveBackward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 8.0), 25);
    /* Moves forward to position, parellel to passport */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 9.5), 25);
    /* Turns right to face passport, bar aligned with lever */
    turnRight(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 95.0), 25);
    /* Move bar up while moving forward slowly until lever reaches top */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 2.0), 25);
    /* Moves bar up */
    rotateServo(servo, 75);
    /* Move bar up while moving forward slowly until lever reaches top */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 3.0), 25);
    /* Move backward a little */
    moveBackward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 2.0), 25);
    /* Turn left 90 degrees */
    turnLeft(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 95.0), 25);
    /* Move forward a bit */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 5.5), 25);
    /* Turn right 90 degrees */
    turnRight(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 95.0), 25);
    /* Move forward a bit */
    moveForward(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_INCH * 8.5), 25);
    /* Turn right 360 degrees to knock down lever */
    turnRight(leftMotor, rightMotor, leftEncoder, rightEncoder, (int)(CLICKS_PER_DEGREE * 360.0), 25);
    /* Move bar down */
    rotateServo(servo, 5);

    /* Stops the robot */
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
    servo.SetDegree(degrees);
}
