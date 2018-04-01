void Cube()
{

  //when limit switch detects cube...stop robot
  if (digitalRead(LimitSwitch) == LOW)
  {
    servo_LeftMotor.writeMicroseconds(1500);
    servo_RightMotor.writeMicroseconds(1500);
    Serial.println("Cube Detected");

    CubeDetected = true;
  }

  //when cube is detectred and robot is stopped...
  if (CubeDetected == true)
  {
    //get cube
    ArmMotor.detach();
    CubeMotor.write(45);

    delay(3000);

    Serial.println("Cube has been grabbed");

    CubeGrabbed = true;
    
  }

 


  if (CubeGrabbed == true)
  {
    ArmMotor.attach(11);
    ArmMotor.write(0);
    delay(2000);
    Serial.println("Ready to drop cube");

    ArmMotor.detach();

    
    //Next Stage
    StageCounter = 2;

  }




}

