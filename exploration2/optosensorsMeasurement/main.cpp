#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>

AnalogInputPin optoRight(FEHIO::P1_0);
AnalogInputPin optoMiddle(FEHIO::P2_0);
AnalogInputPin optoLeft(FEHIO::P3_0);

int main(void)
{
    int numSensors = 3;
    AnalogInputPin optoSensors[3] = {optoLeft, optoMiddle, optoRight};
    
    while(true){
        LCD.Clear(BLACK);
        for (int i = 0; i < numSensors; i++) {
            LCD.WriteLine(optoSensors[i].Value());
            Sleep(100);
        }
    }
    
	return 0;
}
