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

/* Define methods */
void move(FEHMotor left, FEHMotor right, DigitalEncoder rightE, DigitalEncoder leftE, int count);

void turn(FEHMotor motor, DigitalEncoder right, int count);


int main(void)
{
    FEHMotor rightDrive(FEHMotor::Motor1, 9.0);
    FEHMotor leftDrive(FEHMotor::Motor0, 9.0);

    DigitalEncoder right_encoder(FEHIO::P1_1);
    DigitalEncoder left_encoder(FEHIO::P0_0);
    DigitalInputPin bumper1(FEHIO::P0_1);
    DigitalInputPin bumper2(FEHIO::P0_2);

    rightDrive.SetPercent(25.0);
    leftDrive.SetPercent(25.0);
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    while (right_encoder.Counts() < 607) {}
    leftDrive.Stop();

    left_encoder.ResetCounts();
    while(left_encoder.Counts() < 409) {}
    leftDrive.SetPercent(25.0);

    left_encoder.ResetCounts();
    while (left_encoder.Counts() < 434) {}
    rightDrive.Stop();

    right_encoder.ResetCounts();
    while(right_encoder.Counts() < 409) {}
    rightDrive.SetPercent(25.0);

    left_encoder.ResetCounts();
    while (left_encoder.Counts() < 174) {}
    leftDrive.Stop();
    rightDrive.Stop();

}
