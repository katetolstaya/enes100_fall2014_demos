#include <dfr_tank.h>

#include <SoftwareSerial.h>
#include "marker_utils.h"
    
int timeout = 1000;
Marker marker;
  DFRTank tank;

float target_x = 0.5;
float target_y = 0.5;
int turn_pwm = 180;
int fwd_pwm = 200; 

     //float x_point || = {0.5, 0.6, 0.7}
     //float y_point ||

void setup() 
{
  


  Serial.begin(9600);
  tank.init();

   pinMode(7,OUTPUT);
 pinMode(10,OUTPUT);
 pinMode(11,OUTPUT);
 pinMode(12,OUTPUT);
 digitalWrite(7, HIGH);
 digitalWrite(10, HIGH);
 digitalWrite(11, HIGH);
 digitalWrite(12, LOW);
  Serial.begin(9600);
  sSerial.begin(9600);
  delay(100);
  sSerial.print("@OSV connected*");
}

void loop() 
{
  sSerial.flush();
  
  // Locate marker target
  sSerial.print("#5*");
  if(getNextMarker(marker, timeout)){
    target_x = marker.x;
    target_y = marker.y;
  }else{
    sSerial.print("@Timeout 5*");
  }
  sSerial.flush();
  sSerial.print("#4*");
  if(getNextMarker(marker,timeout))
  {
    // while the rover isn't within 10 cm of 1,1
    if(!((marker.x-target_x)*(marker.x-target_x) + (marker.y-target_y)*(marker.y-target_y) < 0.09)) {
     
//      Serial.print(marker.id);
//      Serial.print(",");
//      Serial.print(marker.x);
//      Serial.print(",");
//      Serial.print(marker.y);
//      Serial.print(",");
//      Serial.print(marker.theta);
//      Serial.print(",");
//      Serial.println(marker.time);
     
     //turn until heading matches angle of vector between current location and destination
     
    float orientation_x = cos(marker.theta);
    float orientation_y =  sin(marker.theta);
    
    float d_x = target_x - marker.x;
    float d_y = target_y - marker.y;
    
    float orientation_delta_x = (orientation_x * d_x + orientation_y * d_y);
    float orientation_delta_y = (orientation_y * d_x + orientation_x * -1 * d_y);
    
    float orientation_delta_theta = atan2(orientation_delta_y, orientation_delta_x);
    
    Serial.println(orientation_delta_theta);
    
    if (orientation_delta_theta > 0.3){
        Serial.println('R');
        tank.setLeftMotorPWM(turn_pwm);
        tank.setRightMotorPWM(-1*turn_pwm);
    }else if (orientation_delta_theta < -0.3){
        Serial.println('L');
        tank.setLeftMotorPWM(-1*turn_pwm);
        tank.setRightMotorPWM(turn_pwm);
    }else {
        tank.setLeftMotorPWM(fwd_pwm);
        tank.setRightMotorPWM(fwd_pwm);
    }

  //Stop
//  tank.turnOffMotors();


    
     // forward
     
     // sleep
    
     delay(60);
    } else {
        tank.setLeftMotorPWM(0);
        tank.setRightMotorPWM(0); 
    }
    //}
  }
  else{
    Serial.println("@Timeout 4");
    tank.setLeftMotorPWM(0);
    tank.setRightMotorPWM(0);
    delay(100);
  }

}
