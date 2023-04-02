#include <FEHLCD.h>

#include "Drive.h"

int main(void)
{
    LCD.Clear(BLACK);
    LCD.WriteLine("Hello, World!");

    Drive drive(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::P0_0, FEHIO::P0_1);

    Position a = {0.0, 0.0};

    LCD.WriteLine("Testing TurnTo");
    drive.TurnTo(a);

    return 0;

    LCD.WriteLine("Testing DriveTo");
    drive.DriveTo(a);

    LCD.WriteLine("Testing GoTo");
    drive.GoTo(a);

    LCD.WriteLine("Testing GoToWithCorrection");
    drive.GoTo(a);
}
