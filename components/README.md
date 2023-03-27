# Components Documentation

- [Drive](#Drive)

## Drive

This code defines a `Drive` class that is used to control the movement of a robot using two motors and two digital encoders. The class provides four methods to move the robot: `Straight`, `Back`, `TurnRight`, and `TurnLeft`. 

### Class Variables

- `leftMotor`: an instance of `FEHMotor` class used to control the left motor of the robot.
- `rightMotor`: an instance of `FEHMotor` class used to control the right motor of the robot.
- `leftEncoder`: an instance of `DigitalEncoder` class used to count the number of clicks of the left wheel encoder.
- `rightEncoder`: an instance of `DigitalEncoder` class used to count the number of clicks of the right wheel encoder.
- `drivePower`: an integer value that specifies the percentage of power to be applied to both motors when moving straight or backward. The default value is 25.
- `turnPower`: an integer value that specifies the percentage of power to be applied to both motors when turning. The default value is 20.

### Class Methods

#### `Drive` constructor

The constructor of the `Drive` class takes four parameters:

- `leftPort`: a `FEHMotorPort` value that specifies the port of the left motor.
- `rightPort`: a `FEHMotorPort` value that specifies the port of the right motor.
- `leftEncoderPort`: a `FEHIOPin` value that specifies the pin of the left wheel encoder.
- `rightEncoderPort`: a `FEHIOPin` value that specifies the pin of the right wheel encoder.

#### `Straight` method

The `Straight` method takes an integer parameter `inches` that specifies the distance in inches to move straight. The method sets the motors to move forward, and the robot will move straight until both wheel encoders have counted the required number of clicks.

#### `Back` method

The `Back` method takes an integer parameter `inches` that specifies the distance in inches to move backward. The method sets the motors to move backward, and the robot will move backward until both wheel encoders have counted the required number of clicks.

#### `TurnRight` method

The `TurnRight` method takes an integer parameter `degrees` that specifies the number of degrees to turn right. The method sets the motors to turn right, and the robot will turn right until both wheel encoders have counted the required number of clicks.

#### `TurnLeft` method

The `TurnLeft` method takes an integer parameter `degrees` that specifies the number of degrees to turn left. The method sets the motors to turn left, and the robot will turn left until both wheel encoders have counted the required number of clicks.

#### `ResetEncoderCounts` method

The `ResetEncoderCounts` method is a private method that resets the counts of both wheel encoders.

#### `Stop` method

The `Stop` method is a private method that stops both motors.
