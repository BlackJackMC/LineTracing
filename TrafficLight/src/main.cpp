#include <Arduino.h>
#include "TrafficControl.h"
#include "DistanceSensor.h"
#include "Net.h"

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    startTL();
}
    
void loop()
{
    Serial.println("Running");
    delay(1000);
}
