void setupFlugSteuerung()
{
  mainServo[0].attach(30);
  mainServo[1].attach(31);
  mainServo[2].attach(32);
  mainServo[3].attach(33);
}


// mainServo[4] 0-180 1 VL 2 VR 3 HL 4 HR
// pwm_setting[8] 0-64 1 VL 2 VR 3 HL 4 HR
int motorSpeed[4];
int servoAngle[4];

void take()
{
  motorSpeed[0] = constrain(motorSpeed[0],0,64);
  motorSpeed[1] = constrain(motorSpeed[1],0,64);
  motorSpeed[2] = constrain(motorSpeed[2],0,64);
  motorSpeed[3] = constrain(motorSpeed[3],0,64);
  servoAngle[0] = constrain(servoAngle[0],0,180);
  servoAngle[1] = constrain(servoAngle[1],0,180);
  servoAngle[2] = constrain(servoAngle[2],0,180);
  servoAngle[3] = constrain(servoAngle[3],0,180);
  memcpy(motorSpeed, pwm_setting, 4);
  mainServo[0].write(servoAngle[0]);    
  mainServo[1].write(servoAngle[1]);    
  mainServo[2].write(servoAngle[2]);    
  mainServo[3].write(servoAngle[3]);    
}

void FlugSteuerung(void *pvParameters)  // This is a task.
{

  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
    int yaw, roll, pitch;
    byte speedIn;
    pitch = map(rxDataIn[PitchChannel], 0, 255, MinPitch, MaxPitch);
    yaw = map(rxDataIn[YawChannel], 0, 255, MinYaw, MaxYaw);
    roll = map(rxDataIn[RollChannel], 0, 255, MinRoll, MaxRoll);
    servoAngle[0] = 90;
    servoAngle[1] = 90;
    servoAngle[2] = 90 + pitch + roll; //Hinten Links Servo
    servoAngle[2] = 90 + pitch - roll; //Hinten Rechts Servo
    speedIn = map(rxDataIn[ThrottleChannel], 0, 255, 0, 64 - HeadroomYaw); //Eigentlich bis 
    motorSpeed[0] = speedIn + yaw;
    motorSpeed[1] = speedIn - yaw;
    motorSpeed[2] = speedIn + yaw;
    motorSpeed[3] = speedIn - yaw;
    take();
    vTaskDelay( 20 / portTICK_PERIOD_MS ); // wait for one second
  }
}
