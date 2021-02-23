#define A_CONVERSION_FACTOR 0.061
#include <Wire.h>
#include <LSM6.h>
LSM6 imu;

//Accelerometer Variables
float a_alpha = 0.7;

float x_acceleration;
float x_acceleration_filtered;
float x_acceleration_old = 0;

float y_acceleration;
float y_acceleration_filtered;
float y_acceleration_old = 0;

float z_acceleration;
float z_acceleration_filtered;
float z_acceleration_old = 0;
int i = 0;
class accelerometer {
  public:
    // This is a function prototype.
    // The actual function is written outside
    // of the class (see below).
    void setup_();
    float acceleration(char axis, char filtered);
    float roll(char filtered);
    float pitch(char filtered);
};
void accelerometer::setup_(){
    Wire.begin();
    imu.init();
    imu.enableDefault();
    imu.read();
}

float accelerometer::acceleration(char axis, char filtered){
  imu.read();
  
  float rtn = 0;
  float acc = 0;
  float acc_fill = 0;
  
  if (axis == 'x'){
    
    x_acceleration = imu.a.x * A_CONVERSION_FACTOR / 1000.0;
    x_acceleration_filtered = (a_alpha * x_acceleration) + ((1 - a_alpha) * x_acceleration_old);     //Low pass filter for x axis readings
    x_acceleration_old = x_acceleration;  
    acc = x_acceleration;
    acc_fill = x_acceleration_filtered;
  }
  else if(axis == 'y'){
    
    y_acceleration = imu.a.y * A_CONVERSION_FACTOR / 1000.0;
    y_acceleration_filtered = (a_alpha * y_acceleration) + ((1 - a_alpha) * y_acceleration_old);    //Low pass filter for y axis readings
    y_acceleration_old = y_acceleration; 
    acc = y_acceleration;   
    acc_fill = y_acceleration_filtered;
  
  }
  else if(axis == 'z'){
    z_acceleration = imu.a.z * A_CONVERSION_FACTOR / 1000.0;
    z_acceleration_filtered = (a_alpha * z_acceleration) + ((1 - a_alpha) * z_acceleration_old);    //Low pass filter for z axis readings
    z_acceleration_old = z_acceleration;
    acc = z_acceleration;
    acc_fill = z_acceleration_filtered;
  }
  if (filtered == 'a'){
    rtn = acc;
  }
  
  else if (filtered == 'f'){
    rtn = acc_fill;
  }
  
  //Serial.println(rtn);
  return rtn;

}

float accelerometer::roll(char filtered){
  float rtn = 0;
  float a_roll = atan(acceleration('y','a') / sqrt(pow(acceleration('x','a'), 2) + pow(acceleration('z','a'), 2))) * 180 / PI;
  float a_roll_filtered = atan(acceleration('y','f') / sqrt(pow(acceleration('x','f'), 2) + pow(acceleration('z','f'), 2))) * 180 / PI;
  
  if (filtered == 'f'){
    rtn = a_roll_filtered;
  }
  else{
    rtn = a_roll;
  }
  Serial.println(rtn);
  return rtn;
}
float accelerometer::pitch(char filtered){
  float rtn = 0;
  float a_pitch = atan(acceleration('x','a') / sqrt(pow(acceleration('y','a'), 2) + pow(acceleration('z','a'), 2))) * 180 / PI;
  float a_pitch_filtered = atan(acceleration('x','f') / sqrt(pow(acceleration('y','f'), 2) + pow(acceleration('z','f'), 2))) * 180 / PI;
  
  if (filtered == 'f'){
    rtn = a_pitch_filtered;
  }
  else{
    rtn = a_pitch;
  }
  Serial.println(rtn);
  return rtn;
}
