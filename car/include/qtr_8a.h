#ifndef QTR_H
#define QTR_H

#include <Arduino.h>
#include <vector>

class QTR
{
private:
    std::vector<int> calib_min, calib_max;

public:
    std::vector<int> pins, output;
    int num_sensors;
    int last_position;

    QTR() {}
    QTR(const int _pins[], int _num_sensors)
    {
        setPins(_pins, _num_sensors);
    }

    void read()
    {
        for (int i = 0; i < num_sensors; i++)
            output[i] = analogRead(pins[i]);
    }

    void setPins(const int _pins[], int _num_sensors)
    {
        num_sensors = _num_sensors;
        pins.resize(num_sensors);
        output.resize(num_sensors);

        calib_min.assign(num_sensors, 4095);
        calib_max.assign(num_sensors, 0);

        for (int i = 0; i < num_sensors; i++)
        {
            pins[i] = _pins[i];
        }
    }

    void begin()
    {
        for (int i = 0; i < num_sensors; i++)
            pinMode(pins[i], INPUT);
    }

    void calibrate()
    {
        read();
        for (int i = 0; i < num_sensors; i++)
        {
            if (output[i] < calib_min[i])
                calib_min[i] = output[i];
            if (output[i] > calib_max[i])
                calib_max[i] = output[i];
        }
    }

    void resetCalibration()
    {
        calib_min.assign(num_sensors, 4095);
        calib_max.assign(num_sensors, 0);
    }

    int readPosition()
    {
        int sum = 0;
        long weighted_sum = 0;

        read();

        for (int i = 0; i < num_sensors; i++)
        {
            int calib_value = 0;
            int range = calib_max[i] - calib_min[i];

            if (range > 0)
            {
                calib_value = ((output[i] - calib_min[i]) * 1000) / range;

                if (calib_value < 0)
                    calib_value = 0;
                if (calib_value > 1000)
                    calib_value = 1000;
            }

            if (calib_value < 50)
                calib_value = 0;

            sum += calib_value;
            weighted_sum += static_cast<long>(calib_value) * (i * 1000);
        }

        last_position = sum ? (weighted_sum / sum) : 0;

        return last_position;
    }

    int readPin(int pin)
    {
        read();
        return output[pin];
    }
};

#endif