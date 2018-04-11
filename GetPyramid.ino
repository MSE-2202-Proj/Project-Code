/*
 * GetPyramid, logic for grabbing and tipping pyramid
 * Authors: Daniel Zadagen, Sean Troy, Alexander Noens
 * Date: 3/28/2017
 */

void GetPyramid()
{
  
  //Move Linear Acuator forward

  servo_LeftMotor.writeMicroseconds(1500);
  servo_RightMotor.writeMicroseconds(1500);
  delay(1000);

  servo_LeftMotor.writeMicroseconds(1350);
  servo_RightMotor.writeMicroseconds(1500);
  delay(700);

  servo_LeftMotor.writeMicroseconds(1500);
  servo_RightMotor.writeMicroseconds(1500);
  
  PivotMotor.attach(12);
  PivotMotor.write(160);
  delay(1000);
  
  LAPyramid.attach(13);
  LAPyramid.write(140);
  delay(1000);
  
  PivotMotor.attach(12);
  PivotMotor.write(110);
  delay(1000);
  
  LAPyramid.attach(13);
  LAPyramid.write(0);
  delay(1000);
  //retract linear actuator
  //LAPyramid.write(0);


  CubeMotor.write(180);
  PivotMotor.attach(12);
  PivotMotor.write(50);
  Serial.println("Cube Dropped");
    servo_LeftMotor.writeMicroseconds(1350);
  servo_RightMotor.writeMicroseconds(1350);
  delay(700);

    servo_LeftMotor.writeMicroseconds(1500);
  servo_RightMotor.writeMicroseconds(1500);
 PivotMotor.write(150);
 Serial.print("\n\n\nIt's over.");
 delay(200000);
 
  //Drop Pyramid on cube
  LAPyramid.write(0);
  
}

