/*
  Created by Amirit, Souvik, Kegean, Nii Yemo December 17, 2020.
  Released into the public domain.
*/
class ghFilter {
  public:

    
    float rotation(float gyro, float acceleration ,float time_delta);
 
    //GH Filter Variable
    float prediction;
    float estimate = 0;
    float residual;
    
    float g = 0.2;
    float h = 0.02;

};

float ghFilter::rotation(float gyro, float subtraction,float time_delta){
  /*
   For roll; use gyro = object_name.rotation('x',false,time_delta), and set subtraction to accelerometer roll without the filter (accelerometer_object_name.roll()) 
   For pitch; use gyro = object_name.rotation('y',false,time_delta), and set subtraction to accelerometer pitch without the filter (accelerometer_object_name.pitch())   
   For yaw; use gyro = object_name.rotation('z',false,time_delta), and set subtraction to be magnetometer_object_name.filtered()  
  
  */  
  prediction = estimate + gyro * time_delta / 1000.0;
  residual = subtraction - prediction;
  gyro = gyro + h * residual * 1000 / time_delta;
  estimate = prediction + g * residual;
  Serial.println(estimate);
  return estimate;
}
