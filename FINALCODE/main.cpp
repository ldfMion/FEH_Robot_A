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

#include "Drive.h"

FEHServo leverServo(FEHServo::Servo0);
AnalogInputPin cds(FEHIO::P1_1);

// SERVO VALUES
#define SERVO_MAX 2500
#define SERVO_MIN 550

#define COURSE_Y 72
#define LIGHT_COORDINATES {13.3, 60.0}

#define LEFTRAMP_COORDINATES \
    {                     \
        7, 45          \
    }

Drive drive(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::P0_0, FEHIO::P0_1);



Position GetCoordinate(){
    LCD.Clear();
    LCD.WriteLine("Getting point");
    //Sleep(5000);
    Position point = drive.GetPosition();
    LCD.WriteLine("Point registered");
    //Sleep(5000);
    return point;
}
bool lightIsRed(){
    return (cds.Value() < 0.7);
}
/**
 * to be called from the start position. Completes the ticket task.
 * End Position: backed up against passport wall.
 */
void Ticket(Position lightCoordinates)
{
    // Turn left to face the light
    //Position light = LIGHT_COORDINATES;
    drive.GoToWithCorrection(lightCoordinates);

    // Drive on top of the light
    // drive.Forward(24);

    LCD.Clear();
    Sleep(2.0);
    bool isRed = lightIsRed();
    drive.TurnTo(135);
    if(isRed)
    {
        LCD.Write("THE COLOR IS RED");
        /* Touch the button */
        drive.TurnLeft(45);

        // drive to the button
        drive.Back(9.5);

        // Face the button
        drive.TurnLeft(90);
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
    }
    // Actually press the button
    drive.BackTimed(2);
    drive.Wiggle();
    drive.BackTimed(1);
    
}

/**
 * to be called from the passport wall. Completes the passport task.
 * End Position: aligned with the passport task, facing west.
 */
void Passport()
{   
    /* Moves forward to position */
    drive.Forward(2);
    //lever up
    leverServo.SetDegree(75);
    /* Turns right to position */
    drive.TurnRight(90);
    /* Moves forward to position */
    drive.Forward(4);
    /* Turns left to position, facing right-most ramp */
    //drive.TurnTo(90);
    drive.TurnLeft(44);
    /* Moves forward up the ramp, stopping in front of the passport mechanism */
    drive.SetDrivePercent(50);
    drive.Forward(30);
    drive.SetDrivePercent(25);

    leverServo.SetDegree(15);

    // Back into the passport wall
    drive.TurnLeft(110);
    leverServo.SetDegree(15);
    drive.BackTimed(1.5);
    drive.Wiggle();
    drive.BackTimed(0.5);

    /* Moves forward to position, parallel to passport */
    drive.Forward(7.8);
    /* Turns right to face passport, bar aligned with lever */
    drive.TurnRight(93);
    /* Move bar up while moving forward slowly until lever reaches top */
    drive.Forward(1);
    /* Moves bar up */
    leverServo.SetDegree(80);
    //move into the passport
    drive.ForwardTimed(1.5);
    // move bar down
    drive.Back(4);
    leverServo.SetDegree(15);
    
}

/**
 * to be called from being aligned with the passport task. Completes the luggage task.
 * End Position: at the bottom of the left ramp, facing south.
 */
void Luggage()
{
    leverServo.SetDegree(120);
    drive.ForwardTimed(6.5);
    drive.Back(6);
    drive.TurnTo(0);
    drive.BackTimed(5);
    drive.Wiggle();
    drive.BackTimed(1);
    drive.Forward(5);
    drive.TurnRight(95);
    Position leftRamp = LEFTRAMP_COORDINATES;
    drive.GoToNoTurn(leftRamp);

    /*
    drive.TurnTo(260);
    //drive.TurnTo(270);

    drive.SetDrivePercent(70);
    drive.Forward(2);
    drive.SetDrivePercent(25);    
    Sleep(2);
    
    //----
    //whack the luggage
    drive.SetDrivePercent(70);
    drive.ForwardTimed(0.2);
    drive.SetDrivePercent(25);
     //----
    
    drive.SetDrivePercent(70);
    for (int i = 0; i < 5; i++) {
        drive.Forward(0.6);
        Sleep(.5);
    }
    drive.SetDrivePercent(25);

    drive.Forward(7);
    */

    drive.SetDrivePercent(35);

    drive.Forward(3);
    Sleep(.5);
    for (int i = 0; i < 5; i++) {
        drive.Forward(1.1);
        Sleep(.5);
        drive.Back(.5);
        Sleep(.5);
    }
    

    drive.SetDrivePercent(25);

    drive.Forward(2);
}

/**
 * to be called from the bottom of the left ramp facing south. Completes the lever task.
 * End Position: same place as start.
 */
void Levers()
{

    // Drive to lever
    //drive.Forward(2);
    drive.TurnLeft(3);

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
    drive.TurnLeft(65);

    // drive half way
    drive.ForwardTimed(2.0);

    // 180
    drive.TurnLeft(175);
    drive.SetDrivePercent(40);

    // hit the button
    drive.BackTimed(10.0);
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

    Position lightCoordinates = GetCoordinate();

    WaitToStart();

    LCD.Clear();

    Passport();
    Ticket(lightCoordinates);
    Luggage();
    Levers();
    FinalButton();

    LCD.Clear();
    LCD.Write("Done!");
}
