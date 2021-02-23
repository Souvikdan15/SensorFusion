/*
  Created by Amirit, Souvik, Kegean, Nii Yemo December 17, 2020.
  Released into the public domain.
*/
#define BAUD_RATE 9600
#include <Wire.h>
#include <LSM6.h>
#include "accelerometer.h"
#include "gyroscope.h"
#include "magnetometer.h"
#include "complementaryF.h"
#include "ghFilter.h"

magnetometer magnet;
accelerometer a;
gyroscope gy;
complementaryF comp;
ghFilter gh;

unsigned long last_millis;
unsigned long time_now;
unsigned long diff_time;

unsigned long last_millis_comp;
unsigned long time_now_comp;
unsigned long diff_time_comp;



void setup() {
  // put your setup code here, to run once:
  a.setup_();
  gy.setup_();
  magnet.setup_();

  // Initialise Serial communication
  Serial.begin( BAUD_RATE );
  last_millis     = millis();
  last_millis_comp     = millis();
}

void loop() {
  time_now = millis();
  diff_time = time_now - last_millis;
  last_millis = time_now;

  float time_delta = (float)diff_time;
  /*
  'f' refers to the filtered option. Anyother char variable will return the non -filtered option
  'x','y','z' stop refers to the axis. 
  */
  float x_acceleration = a.acceleration('x','f');
  
  float acceleration_roll_filtered = a.roll('f');
  
  float acceleration_roll_raw = a.roll('n');
  
  float acceleration_pitch_filtered = a.pitch('f');
  
  float acceleration_pitch_raw = a.pitch('n');
  
  float gyroscope_z_axis_aka_yaw = gy.rotation('z',true,time_delta);//(wrapped around 360 quadrants

  float gyroscope_z_axis_aka_yaw_radian = gy.rotation('z',false,time_delta);

  float gyroscope_x_axis_aka_roll_radian = gy.rotation('x',false,time_delta);
  
  float magnetometer_yaw = magnet.yaw();
  
  float magnetometer_yaw_filtered = magnet.filtered();
  
  float complementary_filter_roll = comp.rotation(gyroscope_x_axis_aka_roll_radian, acceleration_roll_raw,time_delta );
  
  float gh_filter_yaw = gh.rotation(gyroscope_z_axis_aka_yaw_radian,magnetometer_yaw_filtered,time_delta );
  
  delay(100);
}
