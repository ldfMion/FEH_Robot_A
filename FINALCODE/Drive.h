#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHRPS.h>
#include <FEHUtility.h>
#include <FEHLCD.h>

#include <math.h>
// #include <algorithm>

//*constants for movement
#define CLICKS_PER_INCH 400.0 / 12.0
#define CLICKS_PER_DEGREE CLICKS_PER_INCH * 0.065
#define M_PI 3.14

#define DEFAULT_DRIVE_POWER 25
#define DEFAULT_TURN_POWER 20

//* constants for RPS
#define RPS_ANGLE_ERROR 2 // angle error in degrees
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

    int drivePower = DEFAULT_DRIVE_POWER;
    int turnPower = DEFAULT_TURN_POWER;

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

    // put get position back here

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
        if (diff > 180)
        {
            diff -= 360;
        }
        else if (diff < -180)
        {
            diff += 360;
        }
        return diff;
    }

    /**
     * get the angle from 0 to 360 deg based on the difference between
     * the x coordinates and the difference between the y coordinates
     * (WITH 0 BEING POINTING UP AS PER THE TRACK)
     */
    float GetAngleFromDeltas(float deltaX, float deltaY)
    {

        // arctan ∆y/∆x
        float angle = radiansToDegrees(atan2f(deltaY, deltaX));

        return angle < 0 ? angle + 360 : angle;
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
    void Forward()
    {
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

    void ForwardTimed(float time) {
        float t_now;
        t_now = TimeNow();

        leftMotor.SetPercent(drivePower);
        rightMotor.SetPercent(-drivePower);

        while(TimeNow()-t_now<time);

        Stop();
    }

    void BackTimed(float time) {
        float t_now;
        t_now = TimeNow();

        leftMotor.SetPercent(-drivePower);
        rightMotor.SetPercent(drivePower);

        while(TimeNow()-t_now<time);

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
    void Turn(float angleDiff)
    {

        angleDiff *= 0.7;

        if (angleDiff > 0)
        {
            TurnLeft(angleDiff);
        }
        else
        {
            TurnRight(abs(angleDiff));
        }
    }

    // Reset the current drive power
    void SetDrivePercent(int percent)
    {
        drivePower = percent;
    }

    // Reset the current drive power
    void SetTurnPercent(int percent)
    {
        turnPower = percent;
    }

    /**
     * Gets the current x and y coordinates and heading
     */
    Position GetPosition()
    {

        // vars for the current position in the rps
        float currX;
        float currY;
        float currHeading;

        // set the vars when RPS returns valid values
        do
        {
            Sleep(20);
            currX = RPS.X();
            currY = RPS.Y();
            currHeading = RPS.Heading();
            LCD.Clear();
            LCD.WriteLine("Waiting for signal");
            LCD.Write("x: ");
            LCD.WriteLine(currX);
            LCD.Write("y: ");
            LCD.WriteLine(currY);
            LCD.Write("heading: ");
            LCD.WriteLine(currHeading);
        } while (currX < 0 || currY < 0 || currHeading < 0);

        return {
            currX,
            currY,
            currHeading};
    }

    void checkInfo(Position desired)
    {

        LCD.Clear();
        // vars for the current position in the rps
        Position currPos = GetPosition();

        float deltaX = desired.x - currPos.x;
        float deltaY = desired.y - currPos.y;

        // desired angle from 0 to 360
        float desiredAngle = GetAngleFromDeltas(deltaX, deltaY);

        LCD.Write("Desired angle");
        LCD.WriteLine(desiredAngle);
        LCD.Write("Current angle");
        LCD.WriteLine(currPos.heading);

        // shortest angle the robot needs to turn
        float angleDiff = calcSmallestAngleDifference(desiredAngle, currPos.heading);
        LCD.Write("Angle diff: ");
        LCD.WriteLine(angleDiff);
    }

    /**
     * Turns the robot towards the desired point
     *
     * @param desired coordinates of the point you want to go to
     */
    void TurnTo(Position desired)
    {
        LCD.Clear();
        // vars for the current position in the rps
        Position currPos = GetPosition();

        float deltaX = desired.x - currPos.x;
        float deltaY = desired.y - currPos.y;

        // desired angle from 0 to 360
        float desiredAngle = GetAngleFromDeltas(deltaX, deltaY);

        LCD.Write("Desired angle");
        LCD.WriteLine(desiredAngle);
        LCD.Write("Current angle");
        LCD.WriteLine(currPos.heading);

        // shortest angle the robot needs to turn
        float angleDiff = calcSmallestAngleDifference(desiredAngle, currPos.heading);
        LCD.Write("Angle diff: ");
        LCD.WriteLine(angleDiff);

        // if the difference is close enough the robots stops turning (base case)
        if (abs(angleDiff) < RPS_ANGLE_ERROR)
        {
            return;
        }

        // robot turns the shortest way towards the desired angle
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
        LCD.Clear();
        LCD.WriteLine("Restarting");
        // just so the robot doesnt crash
        // SetDrivePercent(10);

        // vars for the current position in the rps
        Position currPos = GetPosition();

        // distance form current position to desired position
        float dist = Dist(currPos, desired);

        // stop moving when close enough (base case)
        if (dist < RPS_DIST_ERROR)
        {
            Stop();
            // SetDrivePercent(DEFAULT_DRIVE_POWER);
            LCD.WriteLine("Reached point");
            return;
        }

        float deltaX = desired.x - currPos.x;
        float deltaY = desired.y - currPos.y;

        // desired angle from 0 to 360
        float desiredAngle = GetAngleFromDeltas(deltaX, deltaY);

        // shortest angle the robot needs to turn
        float angleDiff = calcSmallestAngleDifference(desiredAngle, currPos.heading);
        LCD.Write("Angle difference: ");
        LCD.WriteLine(angleDiff);

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
