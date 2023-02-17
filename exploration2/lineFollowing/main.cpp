#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

AnalogInputPin optoRight(FEHIO::P1_0);
AnalogInputPin optoMiddle(FEHIO::P2_0);
AnalogInputPin optoLeft(FEHIO::P3_0);

FEHMotor leftMotor(FEHMotor::Motor0,9);
FEHMotor rightMotor(FEHMotor::Motor1,9);



/*
class Sensor {
    public:

        AnalogInputPin sensor;
        double onLine;
        double offLine;
        double average;
        double getValue(){
            return sensor.Value();
        }
        Sensor() {

        }

}
*/

enum LineStates
{
    MIDDLE,
    RIGHT,
    LEFT
};


void driveStraight() {
    leftMotor.SetPercent(25);
    rightMotor.SetPercent(25);
}

void driveRight() {
    leftMotor.SetPercent(20);
    rightMotor.SetPercent(10);
}

void driveLeft() {
    leftMotor.SetPercent(10);
    rightMotor.SetPercent(20);
}

int main(void)
{
    //int numSensors = 3;
    //AnalogInputPin optoSensors[3] = {optoLeft, optoMiddle, optoRight};

    double leftAvg = (2.33 + 1.117) / 2;
    double middleAvg = (1.982 + 0.768) / 2;
    double rightAvg = (2.201 + 0.935) / 2;

    int state = MIDDLE;

    while (true)
    {
        /* LCD.Clear(BLACK);
        for (int i = 0; i < numSensors; i++)
        {
            LCD.WriteLine(optoSensors[i].Value());
            Sleep(100);
        } */

        while (true)
        { // I will follow this line forever!
            switch (state)
            {
            // If I am in the middle of the line...
            case MIDDLE:
                LCD.Clear(BLACK);
                LCD.WriteLine("IS ON THE MIDDLE");
                driveStraight();
                if (optoRight.Value() > rightAvg)
                {
                    state = RIGHT; // update a new state
                } else if (optoLeft.Value() > leftAvg){
                    state = LEFT;
                }
                
                break;
            // If the right sensor is on the line...
            case RIGHT:
                LCD.Clear(BLACK);
                LCD.WriteLine("IS ON THE LEFT");
                LCD.WriteLine("DRIVING RIGHT");
                driveRight();

                /* if I no longer need to turn right... */
                if (optoRight.Value() < rightAvg) { /* update a new state */
                    state = MIDDLE;
                }
                break;

            // If the left sensor is on the line...
            case LEFT:
                LCD.Clear(BLACK);
                LCD.WriteLine("IS ON THE RIGHT");
                LCD.WriteLine("DRIVING LEFT");
                driveLeft();
                /* if I no longer need to turn left... */
                if (optoLeft.Value() < leftAvg) { /* update a new state */
                    state = MIDDLE;
                }
                break;
            default: // Error. Something is very wrong.
                LCD.Clear();
                LCD.WriteLine("SOMETHING IS VERY WRONG");
                break;
            }
            // Sleep a bit
            Sleep(50);
        }
    }

    return 0;
}

