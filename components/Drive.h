
#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHRPS.h>
#include <FEHUtility.h>

#include <math.h>
// #include <algorithm>

//*constants for movement
#define CLICKS_PER_INCH 400.0 / 12.0
#define CLICKS_PER_DEGREE CLICKS_PER_INCH * 0.065

//* constants for RPS
#define RPS_ANGLE_ERROR 5 // angle error in degrees
#define RPS_DIST_ERROR 3  // distance error in inches

struct Position
{
    float x;
    float y;
    float heading;
};

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

    void Stop()
    {
        leftMotor.Stop();
        rightMotor.Stop();
    }

    //* RPS HELPER FUNCTIONS

    /**
     * Gets the current x and y coordinates and heading
     */
    Position GetPosition()
    {

        // vars for the current position in the rps
        float currX;
        float currY;
        float heading;

        // set the vars when RPS returns valid values
        do
        {
            LCD.Clear();
            LCD.WriteLine("Waiting for signal");
            Sleep(20);
            currX = RPS.X();
            currY = RPS.Y();
            heading = RPS.Heading();
        } while (currX < 0 || currY < 0 || heading < 0);

        return {
            currX,
            currY};
    }

    //*MATH HELPER FUNCTIONS

    float radiansToDegrees(float radians)
    {
        float degrees = radians * (180.0 / M_PI);
        return degrees;
    }

    /**
     * Calculate the positive or negative difference between the angles with the smallest
     * absolute value (to find out which direction the robot should turn)
     * Positive for clockwise and negative for counterclockwise
     */
    float calcSmallestAngleDifference(float desired, float current)
    {

        float diff = desired - current;

        if(abs(diff) > 180){
            
            diff = fmod(diff, 180);
        }
        
    }

    /**
     * get the angle from 0 to 360 deg based on the difference between
     * the x coordinates and the difference between the y coordinates
     * (WITH 0 BEING POINTING UP AS PER THE TRACK)
     */
    float GetAngleFromDeltas(float deltaX, float deltaY)
    {

        // arctan ∆y/∆x
        float angle = radiansToDegrees(atan(deltaY / deltaX));

        if (deltaX < 0)
        {
            // Q2 and Q3
            angle += 180;
        }
        else
        {
            // Q1 and Q4 (Q1 overcorrects for the following subtraction)
            angle += 360;
        }
        // correction for the course (the course is with 0 pointing to the top,
        // unlike the unit circle, which is what we get from the atan function)
        return angle - 90;
    }

    float Dist(Position p1, Position p2)
    {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return hypot(dx, dy);
    }

public:
    Drive(FEHMotor::FEHMotorPort leftPort, FEHMotor::FEHMotorPort rightPort, FEHIO::FEHIOPin leftEncoderPort, FEHIO::FEHIOPin rightEncoderPort) : leftMotor(leftPort, 9),
                                                                                                                                                  rightMotor(rightPort, 9),
                                                                                                                                                  leftEncoder(leftEncoderPort),
                                                                                                                                                  rightEncoder(rightEncoderPort)
    {
        ResetEncoderCounts();
        RPS.InitializeTouchMenu();
        LCD.Clear();
        LCD.WriteLine("Initialization complete");
    }
    /**
     * Just turns the motors on forward wthout stopping
    */
    void Forward(){
        leftMotor.SetPercent(drivePower);
        rightMotor.SetPercent(-drivePower);
    }
    /**
     * Drives forward for a certain amount of inches
     * Method overload
    */
    void Forward(int inches)
    {
        ResetEncoderCounts();

        Forward();

        int clicks = (int)(CLICKS_PER_INCH * inches);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks)
        {
        };

        Stop();
    }

    void Back(int inches)
    {
        ResetEncoderCounts();

        leftMotor.SetPercent(-drivePower);
        rightMotor.SetPercent(drivePower);

        int clicks = (int)(CLICKS_PER_INCH * inches);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks)
        {
        };

        Stop();
    }

    void TurnRight(int degrees)
    {
        ResetEncoderCounts();
        leftMotor.SetPercent(turnPower);
        rightMotor.SetPercent(turnPower);

        int clicks = (int)(CLICKS_PER_DEGREE * degrees);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks)
        {
        };

        Stop();
    }

    void TurnLeft(int degrees)
    {
        ResetEncoderCounts();
        leftMotor.SetPercent(-turnPower);
        rightMotor.SetPercent(-turnPower);

        int clicks = (int)(CLICKS_PER_DEGREE * degrees);

        while (leftEncoder.Counts() <= clicks && rightEncoder.Counts() <= clicks)
        {
        };

        Stop();
    }
    void Turn(float angleDiff){

        if (angleDiff > 0)
        {
            TurnLeft(angleDiff);
        }
        else
        {
            TurnRight(abs(angleDiff));
        }
    }

    // Reset the current drive power. Defaults to 25.
    void SetDrivePercent(int percent)
    {
        drivePower = percent;
    }

    // Reset the current drive power. Defaults to 25.
    void SetTurnPercent(int percent)
    {
        turnPower = percent;
    }

    /**
     * Turns the robot towards the desired point
     *
     * @param desired coordinates of the point you want to go to
     */
    void TurnTo(Position desired)
    {

        // vars for the current position in the rps
        Position currPos = GetPosition();

        float deltaX = desired.x - currPos.x;
        float deltaY = desired.y - currPos.y;

        // desired angle from 0 to 360
        float desiredAngle = GetAngleFromDeltas(deltaY, deltaX);

        LCD.WriteLine(desiredAngle);
        LCD.WriteLine(currPos.heading);

        // shortest angle the robot needs to turn
        float angleDiff = calcSmallestAngleDifference(desiredAngle, currPos.heading);
        LCD.WriteLine(angleDiff);

        // if the difference is close enough the robots stops turning (base case)
        if (abs(angleDiff) < RPS_ANGLE_ERROR)
        {
            return;
        }

        // robot turns to a different direction based on which is faster
        Turn(angleDiff);

        // recursive call
        TurnTo(desired);
    }

    /**
     * Drive the robot in a Forward line to the desired point
     *
     * @param desired coordinate of the point you want to go to
     * 
     * @requires robot to be turned towards the point
     */
    void DriveTo(Position desired)
    {
        Position currPos = GetPosition();
        float dist = Dist(currPos, desired);
        Forward(dist);
    }

    void GoTo(Position desired)
    {
        TurnTo(desired);
        DriveTo(desired);
    }

    /**
     * Supposed to take a position, turn towards it,
     * and if it deviates too much from the correct heading, it corrects itself
     */
    void GoToWithCorrection(Position desired)
    {
        // just so the robot doesnt crash
        SetDrivePercent(10);

        // vars for the current position in the rps
        Position currPos = GetPosition();

        // distance form current position to desired position
        float dist = Dist(currPos, desired);

        // stop moving when close enough (base case)
        if (dist < RPS_DIST_ERROR)
        {
            Stop();
            return;
        }

        float deltaX = desired.x - currPos.x;
        float deltaY = desired.y - currPos.y;

        // desired angle from 0 to 360
        float desiredAngle = GetAngleFromDeltas(deltaY, deltaX);

        // shortest angle the robot needs to turn
        float angleDiff = calcSmallestAngleDifference(desiredAngle, currPos.heading);

        // turns only if the angle difference is too big
        if (abs(angleDiff) < RPS_ANGLE_ERROR)
        {
            // drive Forward if the angle is correct
            Forward();
        }
        else
        {
            // robot turns to a different direction based on which is faster
            Turn(angleDiff);
        }

        // make recursive call
        GoToWithCorrection(desired);
    }
};
