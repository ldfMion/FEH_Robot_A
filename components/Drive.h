#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHRPS.h>
#include <FEHUtility.h>

#include <math.h>
//#include <algorithm>

#define CLICKS_PER_INCH 400.0/12.0
#define CLICKS_PER_DEGREE CLICKS_PER_INCH * 0.065

#define RPS_ERROR 5
#define PULSE_INTERVAL .2

class Drive
{
private:

    FEHMotor leftMotor;
    FEHMotor rightMotor;

    int drivePower = 25;
    int turnPower = 20;

    DigitalEncoder leftEncoder;
    DigitalEncoder rightEncoder;

    void ResetEncoderCounts()
    {
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();
    }

    void Stop(){
        leftMotor.Stop();
        rightMotor.Stop();
    }

    void PulseCounterClockwise()
    {
        // Set both motors to desired percent
        leftMotor.SetPercent(turnPower);
        rightMotor.SetPercent(-turnPower);

        // Wait for the correct number of seconds
        Sleep(PULSE_INTERVAL);

        // Turn off motors
        Stop();
    }

    void PulseClockwise()
    {
        // Set both motors to desired percent
        leftMotor.SetPercent(-turnPower);
        rightMotor.SetPercent(turnPower);

        // Wait for the correct number of seconds
        Sleep(PULSE_INTERVAL);

        // Turn off motors
        Stop();
    }

    /**
     * MATH HELPER METHODS
    */

    float radiansToDegrees(float radians) {
        float degrees = radians * (180.0 / M_PI);
    }

    /** 
     * Calculate the positive or negative difference between the angles with the smallest 
     * absolute value (to find out which direction the robot should turn)
    */
    float calcSmallestAngleDifference(float desired, float current){

        float shiftedDesired = desired - current;
        float diff;
        if (abs(shiftedDesired) > 180) {
            diff = 360 - abs(shiftedDesired);
            if(shiftedDesired < 0) {
                diff = -diff;
            }
        } else {
            diff = shiftedDesired;
        }

        return diff;
    }

    /**
     * get the angle from 0 to 360 deg based on the difference between 
     * the x coordinates and the difference between the y coordinates
    */
    float getAngleFromDeltas(float deltaX, float deltaY){

        //arctan ∆y/∆x
        float deltaAngle = radiansToDegrees(atan(deltaY/deltaX));

        if(deltaX < 0){
            if(deltaAngle < 0){
                // when angle is on the third quadrant of unit circle
                return 180 - deltaAngle;
            }
            if(deltaAngle > 0){
                // when angle is on the second quadrant
                return 90 + deltaAngle;
            }
        } else {
            if(deltaAngle < 0){
                // when angle is on the fourth quadrant of unit circle
                return 360 - deltaAngle;
            }
        }
        
    }

public:
    Drive(FEHMotor::FEHMotorPort leftPort, FEHMotor::FEHMotorPort rightPort, FEHIO::FEHIOPin leftEncoderPort, FEHIO::FEHIOPin rightEncoderPort) : 
        leftMotor(leftPort, 9), 
        rightMotor(rightPort, 9),
        leftEncoder(leftEncoderPort),
        rightEncoder(rightEncoderPort)
    {
        ResetEncoderCounts();
        RPS.InitializeTouchMenu();
    }
    void Straight(int inches)
    {
        ResetEncoderCounts();

        leftMotor.SetPercent(drivePower);
        rightMotor.SetPercent(-drivePower);

        int clicks = (int)(CLICKS_PER_INCH * inches);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

        Stop();
    }

    void Back(int inches)
    {
        ResetEncoderCounts();

        leftMotor.SetPercent(-drivePower);
        rightMotor.SetPercent(drivePower);

        int clicks = (int)(CLICKS_PER_INCH * inches);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

        Stop();
    }

    void TurnRight(int degrees)
    {
        ResetEncoderCounts();
        leftMotor.SetPercent(turnPower);
        rightMotor.SetPercent(turnPower);

        int clicks = (int)(CLICKS_PER_DEGREE * degrees);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

        Stop();
    }

    void TurnLeft(int degrees)
    {
        ResetEncoderCounts();
        leftMotor.SetPercent(-turnPower);
        rightMotor.SetPercent(-turnPower);

        int clicks = (int)(CLICKS_PER_DEGREE * degrees);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks) {};

        Stop();
    }

    // Reset the current drive power. Defaults to 25.
    void SetDrivePercent(int percent) {
        drivePower = percent;
    }

    // Reset the current drive power. Defaults to 25.
    void SetTurnPercent(int percent) {
        turnPower = percent;
    }

    /**
    * Turns the robot towards the desired point
    * 
    * @param desiredX x coordinate of the points you want to go
    * @param desirecY y coordinate of the ponint you want to go
    */
    void TurnTo (float desiredX, float desiredY) {

        // vars for the current position in the rps
        float currX = -1;
        float currY = -1;
        
        // set the vars when RPS returns valid values
        while(currX < 0 && currY < 0){
            Sleep(20);
            currX = RPS.X();
            currY = RPS.Y();
        }

        float deltaX = desiredX - currX;
        float deltaY = desiredY - currY;

        // desired angle from 0 to 360
        float desiredAngle = getAngleFromDeltas(deltaY, deltaX);

        while(true){
            // restart loop if heading value is invalid
            if(RPS.Heading() < 0){
                continue;
            }

            // shortest angle the robot needs to turn
            float angleDiff = calcSmallestAngleDifference(RPS.Heading(), desiredAngle);

            // if the difference is close enough the robots stops turning
            if(abs(angleDiff) < RPS_ERROR){
                break;
            }
            // robot turns to a different direction based on which is faster
            if(angleDiff > 0){
                PulseCounterClockwise();
            } else {
                PulseClockwise();
            }
        }

    }
};

