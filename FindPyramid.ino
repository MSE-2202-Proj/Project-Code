void FindPyramid()
{
  int lightSensor_int;

  //Go in cirlces until it detects the pyramid
  //if (PyramidFound == false){
  
    while(digitalRead(A0) == 0){
      lightSensor_int = analogRead(IRFlag);
      Serial.println(lightSensor_int);

      servo_LeftMotor.writeMicroseconds(1600);
      servo_RightMotor.writeMicroseconds(1500);
      Serial.println("LOCATING");
    }

    //When pyramid is found, robot stops, PyramidFound flag is raised
    if (digitalRead(A0)){
      servo_LeftMotor.writeMicroseconds(1625);
      servo_RightMotor.writeMicroseconds(1625);
      //PyramidFound = true;
      Serial.println("FOUND");
    }
      
    //else{
      //PyramidFound = false;
    //}
  //}
  
  if(digitalRead(A1)==0){
    StageCounter++;
  }
}


