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

#include <Drive.h>

#define CLICKS_PER_INCH 400.0 / 12.0
#define CLICKS_PER_DEGREE CLICKS_PER_INCH * 0.065

FEHServo leverServo(FEHServo::Servo0);
AnalogInputPin cds(FEHIO::P1_1);


// SERVO VALUES
#define SERVO_MAX 2500
#define SERVO_MIN 550

#define COURSE_Y 72
#define LIGHT_COORDINATES \
    {                     \
        10.5, COURSE_Y-11.5          \
    }

#define LEFTRAMP_COORDINATES \
    {                     \
        7, 45          \
    }

Drive drive(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::P0_0, FEHIO::P0_1);




/**
 * to be called from the start position. Completes the ticket task.
 * End Position: backed up against passport wall.
 */
void Ticket()
{

    leverServo.SetDegree(75);
    /* Moves forward to position */
    drive.Forward(2);
    /* Turns right to position */
    drive.TurnRight(90);
    /* Moves forward to position */
    drive.Forward(4);
    /* Turns left to position, facing right-most ramp */
    drive.TurnLeft(38);
    // lowers lever, but enough for it to not hit the ramp
    leverServo.SetDegree(75);
    /* Moves forward up the ramp, stopping in front of the passport mechanism */
    drive.SetDrivePercent(50);
    drive.Forward(30);
    drive.SetDrivePercent(25);

    leverServo.SetDegree(15);
    // Turn left to face the light
    Position light = LIGHT_COORDINATES;
    drive.GoToWithCorrection(light);
    drive.TurnTo(135);

    // Drive on top of the light
    // drive.Forward(24);

    LCD.Clear();
    Sleep(2.0);
    if (cds.Value() < 0.7)
    {
        LCD.Write("THE COLOR IS RED");
        /* Touch the button */
        drive.TurnLeft(45);

        // drive to the button
        drive.Back(8);

        // Face the button
        drive.TurnLeft(90);

        // Actually press the button
        drive.BackTimed(2);
        drive.Wiggle();
        drive.BackTimed(1);

        leverServo.SetDegree(90);
        // Drive back to the luggage
        drive.ForwardTimed(6.5);

        // Back into the passport wall
        drive.Back(1);
        drive.TurnRight(105);
        leverServo.SetDegree(15);
        drive.BackTimed(3.0);
        drive.Wiggle();
        drive.BackTimed(1.0);
    }
    else
    {
        LCD.Write("THE COLOR IS BLUE");
        /* Touch the button */
        drive.TurnLeft(45);

        // drive to the button
        drive.Back(5);

        // Face the button
        drive.TurnLeft(90);

        // Actually press the button
        drive.BackTimed(2);
        drive.Wiggle();
        drive.BackTimed(1);

        leverServo.SetDegree(90);

        // Drive back to the luggage
        drive.ForwardTimed(6.5);

        // Back into the passport wall

        drive.Back(1);

        drive.TurnRight(105);
        leverServo.SetDegree(15);
        drive.BackTimed(4.0);
        drive.Wiggle();
        drive.BackTimed(1.0);
    }
}

/**
 * to be called from the passport wall. Completes the passport task.
 * End Position: aligned with the passport task, facing west.
 */
void Passport()
{
    /* Moves forward to position, parellel to passport */
    drive.Forward(7.8);
    /* Turns right to face passport, bar aligned with lever */
    drive.TurnRight(90);
    /* Move bar up while moving forward slowly until lever reaches top */
    drive.Forward(1);
    /* Moves bar up */
    leverServo.SetDegree(90);
    /* Move bar up while moving forward slowly until lever reaches top */
    //drive.Forward(3);
    drive.ForwardTimed(1.0);
    /* Move backward a little */
    drive.Back(2.5);
    /* Turn left 90 degrees */
    drive.TurnLeft(80);
    /* Move bar down */
    leverServo.SetDegree(15);
}

/**
 * to be called from being aligned with the passport task. Completes the luggage task.
 * End Position: at the bottom of the left ramp, facing south.
 */
void Luggage()
{
    drive.SetDrivePercent(40);

    // Drive to be aligned with the luggage and the ramp.
    drive.ForwardTimed(3.5);

    drive.SetDrivePercent(25);

    drive.Back(2);
    drive.TurnLeft(90);
    drive.Back(2);
    
    Position leftRamp = LEFTRAMP_COORDINATES;
    drive.GoToWithCorrection(leftRamp);

    drive.TurnTo(270);
    drive.TurnTo(270);

    drive.SetDrivePercent(35);

    drive.Forward(2);
    Sleep(.5);
    for (int i = 0; i < 8; i++) {
        drive.Forward(.4);
        Sleep(.5);
    }
    
    drive.Forward(2);


    drive.SetDrivePercent(25);

    Sleep(3.0);
    drive.Forward(3);

    
}

/**
 * to be called from the bottom of the left ramp facing south. Completes the lever task.
 * End Position: same place as start.
 */
void Levers()
{

    // Drive up to the lever
    drive.Back(1);
    // Put arm up
    leverServo.SetDegree(100);
    Sleep(2.0);
    drive.Forward(3);

    

    // Drop arm down
    leverServo.SetDegree(20);

    // Drive back
    drive.Back(3);

    // Wait the required time.
    Sleep(5.0);

    // Drive back forward, and lift the arm
    drive.Forward(3);
    leverServo.SetDegree(100);
    Sleep(1.0);
    
    

    // Drive back and lower the servo.
    leverServo.SetDegree(15);
    drive.Back(3);
    
}

/**
 * to be called from the bottom of the left ramp facing south. Completes the final button task.
 * End Position: at the final button
 */
void FinalButton()
{

    drive.Forward(1.5);

    // face the button
    drive.TurnLeft(60);

    // drive into it
    drive.ForwardTimed(15.0);
}

/* Waits until robot reads a value from the starting light */
void WaitToStart()
{
    LCD.Write("Waiting to start");
    while (cds.Value() > 0.6)
    {
    }
}

int main(void)
{

    drive.initialize();

    // sets servo values
    leverServo.SetMin(SERVO_MIN);
    leverServo.SetMax(SERVO_MAX);

    WaitToStart();

    LCD.Clear();

    Ticket();
    Passport();
    Luggage();
    Levers();
    FinalButton();

    LCD.Clear();
    LCD.Write("Done!");
}
