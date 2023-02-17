#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHServo.h>

FEHServo servo(FEHServo::Servo0); // declare servo arm
AnalogInputPin CdS_cell(FEHIO::P1_0);

#define VOLTAGE_TO_DEGRERE 180/3.3
#define SERVO_MAX 2199
#define SERVO_MIN 500

//servo min: 500
//servo max: 2199

int main(void)
{

    //servo.TouchCalibrate(); // calibrate servo

    servo.SetMin(SERVO_MIN);
    servo.SetMax(SERVO_MAX);
    
    while(true){

        float val = CdS_cell.Value();
        servo.SetDegree(val*VOLTAGE_TO_DEGRERE);
        LCD.Clear(BLACK);
        LCD.WriteLine(val);
        Sleep(100);

    }
 
	return 0;
}
