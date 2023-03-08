#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHSD.h>
#include <math.h>

// Number of points of interest (i.e. A, B, C, D)
#define NUM_POINTS_OF_INTEREST 4

// RPS Delay time
#define RPS_WAIT_TIME_IN_SEC 0.35

// Shaft encoding counts for CrayolaBots
#define COUNTS_PER_INCH 40.5
#define COUNTS_PER_DEGREE 2.48

/* Defines for how long each pulse should be and at what motor power. 
These value will normally be small, but you should play around with the values to find what works best */
#define PULSE_TIME .1
#define PULSE_POWER 20

// Define for the motor power while driving (not pulsing)
#define POWER 25

#define HEADING_TOLERANCE 2

/* Direction along axis which robot is traveling
Examples:
	- if robot is traveling to the upper level, that is a PLUS as the y-coordinate is increasing
	- if robot is traveling to the lower level, that is a MINUS as the y-coordinate is decreasing
*/
#define PLUS 0
#define MINUS 1

// Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P2_0);
DigitalEncoder left_encoder(FEHIO::P2_1);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
FEHMotor left_motor(FEHMotor::Motor0, 9.0);

void set_points_of_interest()
{
    // Declare variables
    float touch_x, touch_y;
    char points[NUM_POINTS_OF_INTEREST] = {'A', 'B', 'C', 'D'};

    // Open SD file for writing
    FEHFile *fptr = SD.FOpen("RPS_POIs.txt", "w");

    Sleep(100);
    LCD.Clear();

    // Wait for touchscreen to be pressed and released
    LCD.WriteLine("Press Screen to Record");
    while (!LCD.Touch(&touch_x, &touch_y));
    while (LCD.Touch(&touch_x, &touch_y));

    LCD.ClearBuffer();

    // Clear screen
    Sleep(100); // wait for 100ms to avoid updating the screen too quickly
    LCD.Clear();

    // Write initial screen info
    LCD.WriteRC("X Position:", 11, 0);
    LCD.WriteRC("Y Position:", 12, 0);
    LCD.WriteRC("   Heading:", 13, 0);

    // Step through each path point to record position and heading
    for (int n = 0; n < NUM_POINTS_OF_INTEREST; n++)
    {
        // Write point letter
        LCD.WriteRC("Touch to set point ", 9, 0);
        LCD.WriteRC(points[n], 9, 20);

        // Wait for touchscreen to be pressed and display RPS data
        while (!LCD.Touch(&touch_x, &touch_y))
        {
            LCD.WriteRC(RPS.X(), 11, 12);       // update the x coordinate
            LCD.WriteRC(RPS.Y(), 12, 12);       // update the y coordinate
            LCD.WriteRC(RPS.Heading(), 13, 12); // update the heading

            Sleep(100); // wait for 100ms to avoid updating the screen too quickly
        }
        while (LCD.Touch(&touch_x, &touch_y));
        LCD.ClearBuffer();

        // Print RPS data for this path point to file
        SD.FPrintf(fptr, "%f %f\n", RPS.X(), RPS.Y());
    }

    // Close SD file
    SD.FClose(fptr);
    LCD.Clear();
}

/*
 * Pulse forward a short distance using time
 */
void pulse_forward(int percent, float seconds)
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Pulse counterclockwise a short distance using time
 */
void pulse_counterclockwise(int percent, float seconds)
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Move forward using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void move_forward(int percent, int counts) // using encoders
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Turn counterclockwise using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void turn_counterclockwise(int percent, int counts)
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Use RPS to move to the desired x_coordinate based on the orientation of the QR code
 */
void check_x(float x_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code
    int power = PULSE_POWER;
    if (orientation == MINUS)
    {
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while (RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        if (RPS.X() < 0) {
            continue;
        }

        if (RPS.X() > x_coordinate)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if (RPS.X() < x_coordinate)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}

/*
 * Use RPS to move to the desired y_coordinate based on the orientation of the QR code
 */
void check_y(float y_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code
    int power = PULSE_POWER;
    if (orientation == MINUS)
    {
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if (RPS.Y() < 0) {
            continue;
        }

        if (RPS.Y() > y_coordinate)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if (RPS.Y() < y_coordinate)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}

// // ABS
// float abs(float x) {
//     return (x < 0) ? -x : x;
// }

/*
 * Use RPS to move to the desired heading
 */
void check_heading(float heading)
{
    // Init max threshold error
    int MAX_ERROR = 5;

    float headingDir = -1;
    while (headingDir < 0) {
        headingDir = RPS.Heading();
    }

    float direction = headingDir - heading < 0;

    // While the heading is not within the accepted amount
    while (!(360 - abs(RPS.Heading() - heading) < MAX_ERROR || abs(RPS.Heading() - heading) < MAX_ERROR) || RPS.Heading() < 0) {

        if (RPS.Heading() < 0) {
            continue;
        } 

        // If needs to turn clockwise
        if (direction) {
            pulse_counterclockwise(PULSE_POWER, PULSE_TIME);
        } else {
            pulse_counterclockwise(-PULSE_POWER, PULSE_TIME);
        }

        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}

int main(void)
{    
    float touch_x, touch_y;

    float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y;
    float A_heading, B_heading, C_heading, D_heading;
    int B_C_counts, C_D_counts, turn_90_counts, turn_180_counts;

    RPS.InitializeTouchMenu();

    // Sets points A B C D positions and headings to SD Card
    set_points_of_interest();

    LCD.Clear();
    LCD.WriteLine("Press Screen To Start Run");
    while (!LCD.Touch(&touch_x, &touch_y));
    while (LCD.Touch(&touch_x, &touch_y));

    // COMPLETE CODE HERE TO READ SD CARD FOR LOGGED X AND Y DATA POINTS
    FEHFile *fptr = SD.FOpen("RPS_POIs.txt", "r");
    SD.FScanf(fptr, "%f%f", &A_x, &A_y);
    SD.FScanf(fptr, "%f%f", &B_x, &B_y);
    SD.FScanf(fptr, "%f%f", &C_x, &C_y);
    SD.FScanf(fptr, "%f%f", &D_x, &D_y);
    SD.FClose(fptr);

    // WRITE CODE HERE TO SET THE HEADING DEGREES AND COUNTS VALUES
    A_heading = 180;
    B_heading = 270;
    C_heading = 90;
    D_heading = 0;

    // Assumed counts it takes to get from B to C, then C to D.
    B_C_counts = 100;
    C_D_counts = 100;

    // Assumed count it takes to turn 90 degrees and 180 degrees.
    turn_90_counts = 100;
    turn_180_counts = 200;

    
    // Open file pointer for writing
    fptr = SD.FOpen("RESULTS.txt", "w");

    // move from A --> B
    LCD.Clear();
    LCD.WriteLine("MOVE A->B");
    check_y(B_y, PLUS);
    check_heading(B_heading);
    Sleep(1.0);
    
    // COMPLETE CODE HERE TO WRITE EXPECTED AND ACTUAL POSITION INFORMATION TO SD CARD
    SD.FPrintf(fptr, "Expected B Position: %f %f %f\n", B_x, B_y, B_heading);
    SD.FPrintf(fptr, "Actual B Position:   %f %f %f\n\n", RPS.X(), RPS.Y(), RPS.Heading());

    // move B --> C
    LCD.Clear();
    LCD.WriteLine("MOVE B->C");
    move_forward(POWER, B_C_counts);
    check_x(C_x, MINUS);
    turn_counterclockwise(POWER,  turn_180_counts);
    check_heading(C_heading);
    Sleep(1.0);

    // COMPLETE CODE HERE TO WRITE EXPECTED AND ACTUAL POSITION INFORMATION TO SD CARD
    SD.FPrintf(fptr, "Expected C Position: %f %f %f\n", C_x, C_y, C_heading);
    SD.FPrintf(fptr, "Actual C Position:   %f %f %f\n\n", RPS.X(), RPS.Y(), RPS.Heading());

    // C --> D
    LCD.Clear();
    LCD.WriteLine("MOVE C->D");
    move_forward(POWER, C_D_counts);
    check_x(D_x, PLUS);
    turn_counterclockwise(-POWER, turn_90_counts);
    check_heading(D_heading);
    check_y(D_y, MINUS);
    Sleep(1.0);

    // COMPLETE CODE HERE TO WRITE EXPECTED AND ACTUAL POSITION INFORMATION TO SD CARD
    SD.FPrintf(fptr, "Expected D Position: %f %f %f\n", D_x, D_y, D_heading);
    SD.FPrintf(fptr, "Actual D Position:   %f %f %f\n\n", RPS.X(), RPS.Y(), RPS.Heading());
    
    // Close file pointer
    SD.FClose(fptr);

    return 0;
}