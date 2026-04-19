#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PIDController
{
private:
    double Kp, Ki, Kd, setpoint;
    double out_min, out_max;
    unsigned long last_time;
    double integral, previous_error, last_output;

public:
    PIDController(double p = 0.0, double i = 0.0, double d = 0.0)
    {
        Kp = p;
        Ki = i;
        Kd = d;
        setpoint = 0.0;
        out_min = -255.0;
        out_max = 255.0;
        last_time = millis();
        integral = 0.0;
        previous_error = 0.0;
        last_output = 0.0;
    }

    void setKp(double p) { Kp = p; }

    void setKi(double i) { Ki = i; }

    void setKd(double d) { Kd = d; }

    void setSetpoint(double sp) { setpoint = sp; }

    void setLimits(double min, double max)
    {
        out_min = min;
        out_max = max;
    }

    double calculate(double current_value)
    {
        unsigned long current_time = millis();
        double dt = (current_time - last_time) / 1000.0;

        if (dt <= 0.0)
        {
            return last_output;
        }

        double error = setpoint - current_value;
        integral += error * dt;

        if (Ki != 0.0)
        {
            if (integral > out_max / Ki)
                integral = out_max / Ki;
            else if (integral < out_min / Ki)
                integral = out_min / Ki;
        }

        double derivative = (error - previous_error) / dt;

        last_output = (Kp * error) + (Ki * integral) + (Kd * derivative);

        if (last_output > out_max)
            last_output = out_max;
        else if (last_output < out_min)
            last_output = out_min;

        previous_error = error;
        last_time = current_time;

        return last_output;
    }

    void reset()
    {
        integral = 0.0;
        previous_error = 0.0;
        last_output = 0.0;
        last_time = millis();
    }
};

#endif