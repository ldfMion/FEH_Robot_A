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

#define CLICKS_PER_INCH 400.0/12.0
#define CLICKS_PER_DEGREE CLICKS_PER_INCH * 0.065

FEHServo leverServo(FEHServo::Servo0);
AnalogInputPin cds(FEHIO::P1_1);

// SERVO VALUES
#define SERVO_MAX 2500
#define SERVO_MIN 550

Drive drive(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::P0_0, FEHIO::P0_1); 

/**
 * to be called from the start position. Completes the ticket task.
 * End Position: backed up against passport wall.
*/
void ticket() {
    /* Moves bar up */
    leverServo.SetDegree(75);
    /* Moves forward to position */
    drive.Forward(2);
    /* Turns right to position */
    drive.TurnRight(90);
    /* Moves forward to position */
    drive.Forward(4);
    /* Turns left to position, facing right-most ramp */
    drive.TurnLeft(38);
    /* Moves forward up the ramp, stopping in front of the passport mechanism */
    drive.Forward(30);
    // Turn left to face the light
    Position light = {13.5,61.5};
    drive.TurnTo(light);

    leverServo.SetDegree(15);
    // Drive on top of the light
    drive.Forward(24);

    LCD.Clear();
    Sleep(2.0);
    if (cds.Value() < 0.7) {
        LCD.Write("THE COLOR IS RED");
        /* Touch the button */
        drive.TurnLeft(45);

        // drive to the button
        drive.Back(8);

        // Face the button
        drive.TurnLeft(90);
        
        // Actually press the button
        drive.BackTimed(2.5);

        leverServo.SetDegree(90);
        // Drive back to the luggage
        drive.ForwardTimed(6.5);

        // Back into the passport wall
        drive.TurnRight(90);
        leverServo.SetDegree(15);
        drive.BackTimed(5.0);

    } else {
        LCD.Write("THE COLOR IS BLUE");
        /* Touch the button */
        drive.TurnLeft(45);

        // drive to the button
        drive.Back(5);

        // Face the button
        drive.TurnLeft(90);
        
        // Actually press the button
        drive.BackTimed(2.5);

        leverServo.SetDegree(90);

        // Drive back to the luggage
        drive.ForwardTimed(6.5);

        // Back into the passport wall
        
        drive.TurnRight(90);
        leverServo.SetDegree(15);
        drive.BackTimed(5.0);
    }
}

/**
 * to be called from the passport wall. Completes the passport task.
 * End Position: aligned with the passport task, facing west.
*/
void passport() {
    /* Moves forward to position, parellel to passport */
    drive.Forward(9.5);
    /* Turns right to face passport, bar aligned with lever */
    drive.TurnRight(95);
    /* Move bar up while moving forward slowly until lever reaches top */
    drive.Forward(2);
    /* Moves bar up */
    leverServo.SetDegree(75);
    /* Move bar up while moving forward slowly until lever reaches top */
    drive.Forward(3);
    /* Move backward a little */
    drive.Back(2);
    /* Turn left 90 degrees */
    drive.TurnLeft(95);
    /* Move bar down */
    leverServo.SetDegree(15);
}

/**
 * to be called from being aligned with the passport task. Completes the luggage task.
 * End Position: at the bottom of the left ramp, facing south.
*/
void luggage() {

    // Drive to be aligned with the luggage and the ramp.
    drive.Forward(17);

    // Face the ramp
    drive.TurnLeft(90);

    drive.SetDrivePercent(35);

    // Stop angled on the ramp, then continue down to give luggage time to drop.
    drive.Forward(5);
    drive.Back(1);
    drive.Forward(1);

    drive.SetDrivePercent(10);
    Sleep(1.0);
    drive.Forward(5);

    drive.SetDrivePercent(25);
}

/**
 * to be called from the bottom of the left ramp facing south. Completes the lever task.
 * End Position: same place as start.
*/
void levers() {

    drive.Forward(3);

    // Put arm up
    leverServo.SetDegree(60);

    // Drive up to the lever
    drive.Forward(3);
    
    // Drop arm down
    leverServo.SetDegree(20);

    // Drive back
    drive.Back(3);

    // Wait the required time.
    Sleep(5.0);

    // Drive back forward, and lift the arm
    drive.Forward(3);
    leverServo.SetDegree(60);

    // Drive back and lower the servo.
    drive.Back(3);
    leverServo.SetDegree(15);
}

/**
 * to be called from the bottom of the left ramp facing south. Completes the final button task.
 * End Position: at the final button
*/
void finalButton() {

    drive.Forward(1.5);

    // face the button
    drive.TurnLeft(65);

    // drive into it
    drive.Forward(25);
}

int main(void) {

    drive.initialize();

    //sets servo values
    leverServo.SetMin(SERVO_MIN);
    leverServo.SetMax(SERVO_MAX);


    LCD.Write("Waiting to start");
    /* Waits until robot reads a value from the starting light */
    while (cds.Value() > 0.6) {
        
    }
    
    LCD.Clear();

    ticket();
    passport();
    luggage();
    levers();
    finalButton();

    
    LCD.Clear();
    LCD.Write("Done!");
}
