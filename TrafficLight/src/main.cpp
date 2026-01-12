#include <Arduino.h>
#include "TrafficControl.h"
#include "DistanceSensor.h"
#include "Net.h"

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
        // startTL();
    pinMode((int)Pin_South::RED_PIN,OUTPUT);
    pinMode((int)Pin_South::YELLOW_PIN,OUTPUT);
    pinMode((int)Pin_South::GREEN_PIN,OUTPUT);
}
    
void loop()
{
    digitalWrite((int)Pin_South::RED_PIN,HIGH);
    digitalWrite((int)Pin_South::YELLOW_PIN,HIGH);
    digitalWrite((int)Pin_South::GREEN_PIN,HIGH);
    Serial.println("Running");
    delay(1000);
}
