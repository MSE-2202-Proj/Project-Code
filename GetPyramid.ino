void GetPyramid()
{
  
  //Move Linear Acuator forward
  PivotMotor.attach(12);
  PivotMotor.write(160);
  delay(1000);
  
  LAPyramid.attach(13);
  LAPyramid.write(140);
  delay(1000);
  
  PivotMotor.attach(12);
  PivotMotor.write(90);
  delay(1000);
  
  LAPyramid.attach(13);
  LAPyramid.write(0);
  delay(1000);
  //retract linear actuator
  //LAPyramid.write(0);

  PivotMotor.attach(12);
  PivotMotor.write(80);
  delay(1000);
  StageCounter++;
}

