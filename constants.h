#ifndef robot_constants
#define robot_constants

#include "Arduino.h"

//wheel
constexpr int left_wheel_pin = 12;
constexpr int right_wheel_pin = 13;

//led
constexpr int led_pin = 11;

//button
constexpr int button_pin = 2;

//sensors (0 -> black 1-> white)
constexpr int left_most_sensor_pin = 3;
constexpr int left_pin = 4;
constexpr int middle_sensor_pin = 5;
constexpr int right_pin = 6;
constexpr int right_most_sensor_pin = 7;
constexpr int sensors[5] = { right_most_sensor_pin, right_pin, middle_sensor_pin, left_pin, left_most_sensor_pin };


//movement
constexpr int forward_align = 3050;
constexpr int rotation_align = 2220;


#endif
