#ifndef _Magentometer
#define _Magentometer
#include "magnetometer.h" 
#include <LIS3MDL.h>    // Follow the labsheet to get this working
#define BUZZER_PIN      6   // To make the annoying beeping

const int NUM_CALIBRATIONS_MAG = 100;

class magnetometer {
    public:
        void init();
        void setup_();
        void  readRaw();
        void  readCalibrated();
        void  calibrate();
        void  calculateOffsets();
        void  set_sensitivity();
        float yaw();
        float filtered();
        LIS3MDL mag;
        float x = 0;
        float y = 0;
        float z = 0;
        float initial_angle = 0;
    private:
        // From datasheet
        float sensitivity = 6.842;
        
        int x_min = 32767;  // Very big values so that we can
        int y_min = 32767;  // make a comparison to save small values.
        int z_min = 32767;
        int x_max = -32768; // Very small values so that we can 
        int y_max = -32768; // make a comparison to save big values.
        int z_max = -32768;
        
        float x_offset = 0;
        float y_offset = 0;
        float z_offset = 0;
        float x_scale = 0;
        float y_scale = 0;
        float z_scale = 0;
        float m_alpha = 0.4;
        float mag_yaw_old = 0;
        float mag_yaw_filtered = 0;    
        int j = 0;

};

void magnetometer::setup_(){
  //initialise and calibrate mag
  init();
  calibrate();
}


// This function initialises the connection to the
// sensor. Note!  Only call this AFTER Wire.begin()
void magnetometer::init()
{
    if (!mag.init()) {
        Serial.println("Failed to detect and initialize magnetometer!");
        while (1);
    }

    mag.enableDefault();
}

// This function will perform a read and save
// the uncalibrated (raw) values to the class
// variables.
void magnetometer::readRaw() {
  mag.read();
  x = mag.m.x;
  y = mag.m.y;
  z = mag.m.z;
}


// This function reads the sensor and then
// corrects the value for the calibration and
// sensitivity.
void magnetometer::readCalibrated() {

  mag.read();
  
  x = sensitivity * (mag.m.x - x_offset) * x_scale;
  y = sensitivity * (mag.m.y - y_offset) * y_scale;
  z = sensitivity * (mag.m.z - z_offset) * z_scale;

}

// This calibration routine:
// - beeps quickly when it starts.
// - you should wave your romi around in the air to get
//   the largest range of values in your environment.
// - it is done when there are slow beeps
void magnetometer::calibrate() {
    analogWrite(BUZZER_PIN, 10);
    delay(50);
    analogWrite(BUZZER_PIN, 0);
    delay(50);
    analogWrite(BUZZER_PIN, 10);
    delay(50);
    analogWrite(BUZZER_PIN, 0);
    delay(50);
    analogWrite(BUZZER_PIN, 10);
    delay(50);
    analogWrite(BUZZER_PIN, 0);
    delay(50);
    
  for (int i=0;i<NUM_CALIBRATIONS_MAG;i++) {
    
    mag.read();

    x_max = max(x_max, mag.m.x);
    y_max = max(y_max, mag.m.y);
    z_max = max(z_max, mag.m.z);

    x_min = min(x_min, mag.m.x);
    y_min = min(y_min, mag.m.y);
    z_min = min(z_min, mag.m.z);

    // Slow down calibration reads
    delay(50);
  }

  calculateOffsets();

  analogWrite(BUZZER_PIN, 10);
  delay(500);
  digitalWrite( BUZZER_PIN, LOW );
  delay(500);
  analogWrite(BUZZER_PIN, 10);
  delay(500);
  digitalWrite( BUZZER_PIN, LOW );
  delay(500);
  
}

// This function looks at the min/max values
// for each axis to calculate the bias.
void magnetometer::calculateOffsets() {
  
  x_offset = (x_max + x_min) / 2;
  y_offset = (y_max + y_min) / 2;
  z_offset = (z_max + z_min) / 2;

  x_scale = (x_max - x_min) / 2;
  y_scale = (y_max - y_min) / 2;
  z_scale = (z_max - z_min) / 2; 

  // Should we make this scale per axis?
  float avg_scale = (x_scale + y_scale + z_scale) / 3;

  x_scale = avg_scale / x_scale;
  y_scale = avg_scale / y_scale;
  z_scale = avg_scale / z_scale;

  Serial.print("X: ");
  Serial.print(x_offset);  
  Serial.print("Y: ");
  Serial.print(y_offset);
  Serial.print("Z: ");
  Serial.println(z_offset);
  
  Serial.print("X: ");
  Serial.print(x_scale);  
  Serial.print("Y: ");
  Serial.print(y_scale);
  Serial.print("Z: ");
  Serial.println(z_scale);
 
}

float magnetometer::yaw(){
  readCalibrated();
  float heading = atan2(y,x);
  float angle = heading * 180.0 / PI;
  if (j == 0)
  {
    initial_angle = angle;
    j = 1;
  }
  float  mag_yaw =  initial_angle - angle;
  //Serial.println(mag_yaw);
  return mag_yaw;
}

float magnetometer::filtered(){
  mag_yaw_filtered = (m_alpha * yaw()) + ((1.0 - m_alpha) * mag_yaw_old);
  mag_yaw_old = mag_yaw_filtered;

  return mag_yaw_filtered;
}

#endif
