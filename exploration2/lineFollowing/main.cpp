#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

class LineSensor
{
    private:
        double average;
        AnalogInputPin sensor;

    public:
        LineSensor(FEHIO::FEHIOPin port, double onLine, double offLine) : sensor(port)
        {
            average = (onLine + offLine) / 2;
        }
        double getValue()
        {
            return sensor.Value();
        }
        bool isOnLine()
        {
            return getValue() > average;
        }
};

class Drive {
    private:
        FEHMotor leftMotor;
        FEHMotor rightMotor;
    public: 
        Drive(FEHMotor::FEHMotorPort leftPort, FEHMotor::FEHMotorPort rightPort) : leftMotor(leftPort, 9), rightMotor(rightPort, 9) {

        }
        void straight()
        {
            leftMotor.SetPercent(25);
            rightMotor.SetPercent(25);
        }

        void right()
        {
            leftMotor.SetPercent(20);
            rightMotor.SetPercent(10);
        }

        void left()
        {
            leftMotor.SetPercent(10);
            rightMotor.SetPercent(20);
        }

};

LineSensor optoLeft(FEHIO::P3_0, 2.33, 1.117);
LineSensor optoMiddle(FEHIO::P2_0, 1.982, 0.768);
LineSensor optoRight(FEHIO::P1_0, 2.201, 0.935);

Drive drive(FEHMotor::Motor0, FEHMotor::Motor1);

enum LineStates
{
    MIDDLE,
    RIGHT,
    LEFT
};



int main(void)
{

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
                drive.straight();
                if (optoRight.isOnLine())
                {
                    state = RIGHT; // update a new state
                }
                else if (optoLeft.isOnLine())
                {
                    state = LEFT;
                }

                break;
            // If the right sensor is on the line...
            case RIGHT:
                LCD.Clear(BLACK);
                LCD.WriteLine("IS ON THE LEFT");
                LCD.WriteLine("DRIVING RIGHT");
                drive.right();

                /* if I no longer need to turn right... */
                if (!optoRight.isOnLine())
                { /* update a new state */
                    state = MIDDLE;
                }
                break;

            // If the left sensor is on the line...
            case LEFT:
                LCD.Clear(BLACK);
                LCD.WriteLine("IS ON THE RIGHT");
                LCD.WriteLine("DRIVING LEFT");
                drive.left();
                /* if I no longer need to turn left... */
                if (!optoLeft.isOnLine())
                { /* update a new state */
                    state = MIDDLE;
                }
                break;
            default: // Error. Something is very wrong.
                LCD.Clear(BLACK);
                LCD.WriteLine("SOMETHING IS VERY WRONG");
                break;
            }
            // Sleep a bit
            Sleep(50);
        }
    }

    return 0;
}
