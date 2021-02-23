class complementaryF {
  public:
    // This is a function prototype.
    // The actual function is written outside
    // of the class (see below).
    
    float rotation(float gyro, float acceleration ,float time_delta);

    //Complementary Filter Variable
    float cf_alpha = 0.92;
    float cf_old = 0;


};

float complementaryF::rotation(float gyro, float multiple ,float time_delta){
  /*
   For the variable gyro use object_name.rotation('x',false,time_delta) from gyroscope 
   For roll; use gyro = object_name.rotation('x',false,time_delta), and set multiple to accelerometer roll without the filter (accelerometer_object_name.roll()) 
   For pitch; use gyro = object_name.rotation('y',false,time_delta), and set multiple to accelerometer pitch without the filter (accelerometer_object_name.pitch())   
   For yaw; use gyro = object_name.rotation('z',false,time_delta), and multiple tobe magnetometer_object_name.yaw()  
  
  */
  float cf = cf_alpha * (cf_old + (gyro * time_delta / 1000.0)) + ((1.0 - cf_alpha) * multiple);
  cf_old = cf;
  Serial.println(cf);

  return cf;
}
