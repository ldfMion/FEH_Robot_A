/********************************/
/*      Proteus Test Code       */
/*     OSU FEH Spring 2020      */
/*        Drew Phillips         */
/*    02/03/20  Version 3.0.1     */
/********************************/

// AM 02/03/20

/* Include preprocessor directives */
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

#define TURN_TIME 1.2
#define BACK_TIME 1.0


void turn(bool rightdir, FEHMotor left, FEHMotor right)
{
    float direction = 25.;
    if (rightdir) {direction *= -1;}
    
    left.SetPercent(direction);
    right.SetPercent(direction);
    
    Sleep(TURN_TIME);
    left.Stop();
    right.Stop();
}

void backUp(FEHMotor left, FEHMotor right)
{
    left.SetPercent(-25.);
    right.SetPercent(25.);
    Sleep(BACK_TIME);
    left.Stop();
    right.Stop();
}

void moveForwardUntilHit(FEHMotor left, FEHMotor right, DigitalInputPin bumpR, DigitalInputPin bumpL)
{
    left.SetPercent(25.);
    right.SetPercent(-25.);
    while (bumpR.Value() || bumpL.Value()) {}
    left.Stop();
    right.Stop();
}

void moveBackUntilHit(FEHMotor left, FEHMotor right, DigitalInputPin bumpR, DigitalInputPin bumpL)
{
    left.SetPercent(-25.);
    right.SetPercent(25.);
    while (bumpR.Value() || bumpL.Value()) {}
    left.Stop();
    right.Stop();
}

/* Main function to control menu system */
int main(void)
{
    //Define the motors
    FEHMotor right_motor(FEHMotor::Motor0,9.0);
    FEHMotor left_motor(FEHMotor::Motor1,9.0);

    // Define the bump pins
    DigitalInputPin bump_FLeft(FEHIO::P0_0);
    DigitalInputPin bump_FRight(FEHIO::P3_0);
    DigitalInputPin bump_BLeft(FEHIO::P0_2);
    DigitalInputPin bump_BRight(FEHIO::P0_3);
    
    // Do the course
    moveForwardUntilHit(left_motor, right_motor, bump_FRight, bump_FLeft);
    backUp(left_motor, right_motor);
    turn(false, left_motor, right_motor);
    moveBackUntilHit(left_motor, right_motor, bump_BRight, bump_BLeft);
    moveForwardUntilHit(left_motor, right_motor, bump_FRight, bump_FLeft);
    backUp(left_motor, right_motor);
    turn(true, left_motor, right_motor);
    moveBackUntilHit(left_motor, right_motor, bump_BRight, bump_BLeft);
    moveForwardUntilHit(left_motor, right_motor, bump_FRight, bump_FLeft);
    
    
}

