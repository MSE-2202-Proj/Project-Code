/*
 * Cube, logic for locating and moving cube into positon
 * Authors: Daniel Zadagen, Sean Troy, Alexander Noens
 * Date: 3/28/2017
 */

void Cube(){
  //when limit switch detects cube...stop robot
  if ((digitalRead(LimitSwitch) == LOW)&&(CubeGrabbed == false)){
    servo_LeftMotor.writeMicroseconds(1500);
    servo_RightMotor.writeMicroseconds(1500);
    Serial.println("Cube Detected");
    CubeStep = 1;
  }
  switch(CubeStep){

    case 1:
    //when cube is detectred and robot is stopped...
      //get cube
      CubeMotor.attach(10);
      ArmMotor.detach();
      CubeMotor.write(85);
      delay(3000);
  
      Serial.println("Cube has been grabbed");
      CubeGrabbed = true;
      CubeStep++; 
    break;
    
    case 2:
      ArmMotor.attach(11);
      ArmMotor.write(0);
      delay(2000);
      Serial.println("Ready to drop cube");
  
      ArmMotor.detach();
      //Next Stage
      StageCounter = 2;
    break;
  }
}


