#include <FEHMotor.h>
#include <FEHIO.h>

#define CLICKS_PER_INCH 400.0/12.0
#define CLICKS_PER_DEGREE CLICKS_PER_INCH * 0.065

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

public:
    Drive(FEHMotor::FEHMotorPort leftPort, FEHMotor::FEHMotorPort rightPort, FEHIO::FEHIOPin leftEncoderPort, FEHIO::FEHIOPin rightEncoderPort) : 
        leftMotor(leftPort, 9), 
        rightMotor(rightPort, 9),
        leftEncoder(leftEncoderPort),
        rightEncoder(rightEncoderPort)
    {
        ResetEncoderCounts();
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
    void setDrivePercent(int percent) {
        drivePower = percent;
    }

    // Reset the current drive power. Defaults to 25.
    void setTurnPercent(int percent) {
        turnPower = percent;
    }
};
