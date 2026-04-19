#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor
{
private:
    uint8_t pin_encA;
    uint8_t pin_encB;
    uint8_t pin_in1;
    uint8_t pin_in2;
    uint8_t pin_pwm;

    float total_ppr;
    volatile long pulse_count;
    unsigned long last_time;
    int current_rpm;

    static void IRAM_ATTR isrHandler(void *arg)
    {
        Motor *instance = static_cast<Motor *>(arg);
        instance->pulse_count++;
    }

public:
    Motor(uint8_t encA, uint8_t encB, uint8_t in1, uint8_t in2, uint8_t pwm, float motorPpr, float gearRatio)
    {
        pin_encA = encA;
        pin_encB = encB;
        pin_in1 = in1;
        pin_in2 = in2;
        pin_pwm = pwm;

        total_ppr = motorPpr * gearRatio;
        pulse_count = 0;
        last_time = 0;
        current_rpm = 0;
    }

    void begin()
    {
        pinMode(pin_in1, OUTPUT);
        pinMode(pin_in2, OUTPUT);
        pinMode(pin_pwm, OUTPUT);

        pinMode(pin_encA, INPUT_PULLUP);
        pinMode(pin_encB, INPUT_PULLUP);

        attachInterruptArg(digitalPinToInterrupt(pin_encA), isrHandler, this, RISING);
    }

    int getSpeed()
    {
        unsigned long current_time = millis();
        unsigned long delta_time = current_time - last_time;

        if (delta_time >= 100)
        {
            noInterrupts();
            long pulses = pulse_count;
            pulse_count = 0;
            interrupts();

            current_rpm = (pulses / total_ppr) * (60000.0 / delta_time);

            last_time = current_time;
        }

        return current_rpm;
    }

    void setSpeed(int speed)
    {
        if (speed > 0)
        {
            digitalWrite(MOTOR_IN1, HIGH);
            digitalWrite(MOTOR_IN2, LOW);
            analogWrite(MOTOR_PWM, speed);
        }
        else if (speed < 0)
        {
            digitalWrite(MOTOR_IN1, LOW);
            digitalWrite(MOTOR_IN2, HIGH);
            analogWrite(MOTOR_PWM, -speed);
        }
        else
        {
            // Brake
            digitalWrite(MOTOR_IN1, LOW);
            digitalWrite(MOTOR_IN2, LOW);
            analogWrite(MOTOR_PWM, 0);
        }
    }
};

#endif