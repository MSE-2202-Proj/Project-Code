/*
 * FinalStep, logic for Mvoing cube under pyramid and releasing pyramid
 * Authors: Daniel Zadagen, Sean Troy, Alexander Noens
 * Date: 3/28/2017
 */

void FinalStep()
{
  //Put arm into cube-dropping position
  
  servo_LeftMotor.writeMicroseconds(1500);
  servo_RightMotor.writeMicroseconds(1500);
  delay(1000);

  servo_LeftMotor.writeMicroseconds(1350);
  servo_RightMotor.writeMicroseconds(1500);
  delay(200);

  servo_LeftMotor.writeMicroseconds(1500);
  servo_RightMotor.writeMicroseconds(1500);
  
  ArmMotor.attach(11);
  ArmMotor.write(0);
  ArmMotor.detach();


  //Drop Cube
  CubeMotor.write(180);
  Serial.println("Cube Dropped");

  delay(2000);


  //Drop Pyramid on cube
  LAPyramid.write(0);



  Serial.println("CONGRATULATIONS!!!!!!!");



}

