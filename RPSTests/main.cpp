#include <FEHLCD.h>

#include "Drive.h"

// points
#define LIGHT_COORDINATES {13.5,61.5}

int main(void)
{
    LCD.Clear(BLACK);
    LCD.WriteLine("Hello, World!");

    Drive drive(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::P0_0, FEHIO::P0_1);

    Position light = LIGHT_COORDINATES;

    while(true){
        drive.checkInfo(light);
        Sleep(2000);
    }

    return 0;

    LCD.WriteLine("Testing TurnTo");
    drive.TurnTo(light);

    LCD.WriteLine("Testing GoToWithCorrection");
    drive.GoToWithCorrection(light);

    LCD.WriteLine("Testing GoTo");
    drive.GoTo(light);

    

    LCD.WriteLine("Testing DriveTo");
    drive.DriveTo(light);


}
