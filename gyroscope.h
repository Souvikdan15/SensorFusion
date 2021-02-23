#define G_CONVERSION_FACTOR 8.75
//Gyroscope Variables
float g_x_bias = 0;
float g_y_bias = 0;
float g_z_bias = 0;

float g_roll;
float g_pitch;
float g_yaw;

class gyroscope {
  public:
    // This is a function prototype.
    // The actual function is written outside
    // of the class (see below).
    void setup_();
    float rotation(char axis, bool wrap, float time_delta);
    float g_x = 0;
    float g_y = 0;
    float g_z = 0;
};
void gyroscope::setup_(){

  //Calculate bias for gyroscope
  for ( int i = 0; i < 100; i++) {
    imu.read();
    g_x_bias += imu.g.x;
    g_y_bias += imu.g.y;
    g_z_bias += imu.g.z;
  }
  g_x_bias /= 100.0;
  g_y_bias /= 100.0;
  g_z_bias /= 100.0;
  
}

float gyroscope::rotation(char axis, bool wrap ,float time_delta){
  /*
   * axis refers to x y z.
   * wrap refers to whether the wraped gyro reading is used or not
   * time_delta referst to the time entry
  
  */
  imu.read();

  //Gyroscope
  float rtn = 0;
  float g_x = (imu.g.x - g_x_bias) * G_CONVERSION_FACTOR / 1000.0;
  float g_y = (imu.g.y - g_y_bias) * G_CONVERSION_FACTOR / 1000.0;
  float g_z = (imu.g.z - g_z_bias) * G_CONVERSION_FACTOR / 1000.0;

  g_roll  += g_x * time_delta / 1000.0;
  g_pitch += g_y * time_delta / 1000.0;
  g_yaw   += g_z * time_delta / 1000.0;

  //Wrap the values between -PI and PI
  if (g_roll > 180.0) {
    g_roll = 360 - g_roll;
  }
  else if (g_roll < -180.0) {
    g_roll = g_roll + 360;
  }


  if (g_pitch > 180.0) {
    g_pitch = 360 - g_pitch;
  }
  else if (g_pitch < -180.0) {
    g_pitch = g_pitch + 360;
  }


  if (g_yaw > 180.0) {
    g_yaw = 360 - g_yaw;
  }
  else if (g_yaw < -180.0) {
    g_yaw = g_yaw + 360;
  }

  if (wrap == true){
    if(axis == 'x'){
      rtn = g_roll;
    }
    else if(axis == 'y'){
      rtn = g_pitch;
    }
    else if(axis == 'z'){
      rtn = g_yaw;
    }
  }
  else if(axis == 'x'){
      rtn = g_x;
    }
    else if(axis == 'y'){
      rtn = g_y;
    }
    else if(axis == 'z'){
      rtn = g_z;
    }
  //Serial.println(rtn);
  return rtn;

}
