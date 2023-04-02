# README

This code is for a client written in C++ that controls a robot's movements and interacts with the FEH RPS module. The client includes a `Drive` class that provides methods for driving forward and backward a certain number of inches, turning left and right by a certain number of degrees, and other helper functions.

## Requirements

* `FEHMotor.h`
* `FEHIO.h`
* `FEHRPS.h`
* `FEHUtility.h`
* `math.h`

## Constants

* `CLICKS_PER_INCH`: the number of encoder clicks that correspond to one inch of movement
* `CLICKS_PER_DEGREE`: the number of encoder clicks that correspond to one degree of rotation
* `RPS_ANGLE_ERROR`: the allowed error in degrees for RPS heading measurements
* `RPS_DIST_ERROR`: the allowed error in inches for RPS position measurements

## `Drive` class

### Properties

* `leftMotor`: a `FEHMotor` object representing the left motor
* `rightMotor`: a `FEHMotor` object representing the right motor
* `drivePower`: an integer representing the power percentage for driving forward/backward
* `turnPower`: an integer representing the power percentage for turning left/right
* `leftEncoder`: a `DigitalEncoder` object representing the left encoder
* `rightEncoder`: a `DigitalEncoder` object representing the right encoder

### Methods

* `Drive::Drive()`: constructor that initializes the `Drive` object and the RPS module
* `Drive::Forward()`: drives the robot forward at the `drivePower` percentage
* `Drive::Forward(int inches)`: drives the robot forward for a specified number of inches
* `Drive::Back(int inches)`: drives the robot backward for a specified number of inches
* `Drive::TurnRight(int degrees)`: turns the robot right by a specified number of degrees
* `Drive::TurnLeft(int degrees)`: turns the robot left by a specified number of degrees
* `Drive::GetPosition()`: gets the current position of the robot using the RPS module
* `Drive::radiansToDegrees(float radians)`: converts radians to degrees
* `Drive::calcSmallestAngleDifference(float desired, float current)`: calculates the smallest angle difference between two angles
* `Drive::GetAngleFromDeltas(float deltaX, float deltaY)`: gets the angle of rotation from two x and y differences
* `Drive::Dist(Position p1, Position p2)`: gets the distance between two positions
