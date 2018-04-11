/*
 * FindPyramid, logic for locating pyramid
 * Authors: Daniel Zadagen, Sean Troy, Alexander Noens
 * Date: 3/28/2017
 */

void FindPyramid()
{
  int lightSensor_int;

  //Go in cirlces until it detects the pyramid
  //if (PyramidFound == false){

  if(Once){
    LAPyramid.attach(13);
    LAPyramid.write(80);
    delay(1000);
    LAPyramid.detach();

  //Set up pivot motor
    PivotMotor.attach(12);
    PivotMotor.write(154);
    delay(1000);

    Once = 0;
  }
if(digitalRead(A0) == 0){


      Ping();
  //Serial.println("PING");
  CmSide = EchoTimeSide/58;
  //delay(100);
  if(CmSide < 3){
    CmSide = 0;
    SamplesSide --;
  }

  if(CmSide > 0){   
    //SideAvg = (((SideAvg*(SamplesSide-1)) + CmSide)/(SamplesSide));

    SideAvg = CmSide;
    if(SamplesSide == 1){
      SamplesSide = 1;
    }
    
    //FrontAvg = (FrontAvg + CmFront)/2;
    //Serial.println("Side:");
    //Serial.println(SideAvg);
    //Serial.println(SamplesSide);
  }
  SamplesSide++;
  //--------------------------------------------------------------------------------
  
  //Front Sensor is too slow, use immediate values, cant average due to sample speed
  //---------Get values from Front US sensor----------------------------------------------------
  PingFront();
  CmFront = EchoTimeFront/58;
  
  if(CmFront < 3){
    CmFront = 0;
    SamplesFront --;
  }

  if(CmFront > 0){    
    //FrontAvg = (((FrontAvg*(SamplesFront-1)) + CmFront)/(SamplesFront));
    FrontAvg = CmFront;
    if(SamplesFront == 1){
      SamplesFront = 1;
    }
    //FrontAvg = (FrontAvg + CmFront)/2;
    //Serial.println("Front:");
    //Serial.println(FrontAvg);
    //Serial.println(SamplesFront);
  }
  SamplesFront++; 
  //--------------------------------------------------------------------------------
  
  //Follow wall until corner is reached
  if(SideAvg >= 3){
    SideReady = true;   
  }

  if(FrontAvg >= 3){
    FrontReady = true;
  }



  

      
      lightSensor_int = analogRead(IRFlag);
      Serial.print(FrontAvg);
      Serial.print("       ");
      Serial.print(SideAvg);



if(FrontAvg < 25) pyramidWall = 1;
if(FrontAvg > 25 || SideAvg < 10) pyramidWall = 0;
      
if(pyramidWall == 1) {
      servo_LeftMotor.writeMicroseconds(1400);
      servo_RightMotor.writeMicroseconds(1750);
      Serial.println("WALL"); 
      
      }

 if(pyramidWall == 0) {
  
    servo_LeftMotor.writeMicroseconds(1675);
    servo_RightMotor.writeMicroseconds(1775);
    Serial.println("LOCATING"); 
   
  }
 }

    //When pyramid is found, robot stops, PyramidFound flag is raised
    if (digitalRead(A0)){
      servo_LeftMotor.writeMicroseconds(1650);
      servo_RightMotor.writeMicroseconds(1650);
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


