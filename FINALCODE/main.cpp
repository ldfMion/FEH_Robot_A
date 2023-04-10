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

Drive drive(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::P0_0, FEHIO::P0_1); 

// SERVO VALUES
#define SERVO_MAX 2390
#define SERVO_MIN 500

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
    drive.turnRight(90);
    /* Moves forward to position */
    drive.Forward(4);
    /* Turns left to position, facing right-most ramp */
    drive.turnLeft(38);
    /* Moves forward up the ramp, stopping in front of the passport mechanism */
    drive.Forward(30);
    // Turn left to face the light
    drive.turnLeft(55);
    // Drive on top of the light
    drive.Forward(20);

    LCD.Clear();
    Sleep(2.0);
    if (cds.Value() < 0.7) {
        LCD.Write("THE COLOR IS RED");
        /* Touch the button */
        drive.turnLeft(45);

        // drive to the button
        drive.Back(8);
        // Face the button
        drive.turnLeft(90);
        
        // Actually press the button
        drive.Back(5);

        // Drive back to the luggage
        drive.Forward(23);

        // Back into the passport wall
        drive.turnRight(90);
        drive.moveBack(18);
    } else {
        LCD.Write("THE COLOR IS BLUE");
        /* Touch the button */
        drive.turnLeft(45);

        // drive to the button
        drive.Back(3);
        // Face the button
        drive.turnLeft(90);
        
        // Actually press the button
        drive.Back(5);

        // Drive back to the luggage
        drive.Forward(23);

        // Back into the passport wall
        drive.turnRight(90);
        drive.moveBack(18);
    }
}

/**
 * to be called from the passport wall. Completes the passport task.
 * End Position: aligned with the passport task, facing west.
*/
int passport() {
    /* Moves forward to position, parellel to passport */
    drive.Forward(9.5);
    /* Turns right to face passport, bar aligned with lever */
    drive.turnRight(95);
    /* Move bar up while moving forward slowly until lever reaches top */
    drive.Forward(2);
    /* Moves bar up */
    leverServo.SetDegree(75);
    /* Move bar up while moving forward slowly until lever reaches top */
    drive.Forward(3);
    /* Move backward a little */
    drive.Back(2);
    /* Turn left 90 degrees */
    drive.turnLeft(95);
    /* Move bar down */
    leverServo.SetDegree(5);
}

/**
 * to be called from being aligned with the passport task. Completes the luggage task.
 * End Position: at the bottom of the left ramp, facing south.
*/
int luggage() {

    // Drive to be aligned with the luggage and the ramp.
    drive.Forward(18);

    // Face the ramp
    drive.turnLeft(70);

    drive.setDrivePercent(10);
    // Stop angled on the ramp, then continue down to give luggage time to drop.
    drive.Forward(3);
    Sleep(1.0);
    drive.Forward(5);

    drive.setDrivePercent(25);
}

/**
 * to be called from the bottom of the left ramp facing south. Completes the lever task.
 * End Position: same place as start.
*/
int levers() {

    // Put arm up
    leverServo.SetDegree(60);

    // Drive up to the lever
    drive.Forward(1);
    
    // Drop arm down
    leverServo.SetDegree(20);

    // Drive back
    drive.Back(1);

    // Wait the required time.
    Sleep(5.0);

    // Drive back forward, and lift the arm
    drive.Forward(1);
    leverServo.SetDegree(60);

    // Drive back and lower the servo.
    drive.Back(1);
    leverServo.SetDegree(5);
}

/**
 * to be called from the bottom of the left ramp facing south. Completes the final button task.
 * End Position: at the final button
*/
int finalButton() {

    // face the button
    drive.turnLeft(60);

    // drive into it
    drive.Forward(25);
}

int main(void) {
    /* FEHMotor leftMotor(FEHMotor::Motor0, 9.0);
    FEHMotor rightMotor(FEHMotor::Motor1, 9.0); */
    //DigitalEncoder leftEncoder(FEHIO::P0_0);
    //DigitalEncoder rightEncoder(FEHIO::P0_1);
    

    //sets servo values
    leverServo.SetMin(SERVO_MIN);
    leverServo.SetMax(SERVO_MAX);

    

    /* Waits until robot reads a value from the starting light */
    while (cds.Value() > 0.5) {
        LCD.Write("Waiting to start");
        LCD.Clear();
    }

    
}

