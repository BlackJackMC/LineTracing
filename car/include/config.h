#ifndef CONFIG_H
#define CONFIG_H

#define SSID "v"
#define PASSWORD "yourmomfat"

#define MOTOR_IN1 12
#define MOTOR_IN2 13
#define MOTOR_PWM 14
#define MOTOR_PPR 11.0
#define MOTOR_GEAR_RATIO 9.6

#define ENC_A 17
#define ENC_B 18

#define SERVO_PIN 16

#define QTR_IR_PIN 15
const int qtr_pins[] = {4, 5, 6, 7, 8, 3, 9, 10};
const int qtr_num_sensors = 8;

#endif