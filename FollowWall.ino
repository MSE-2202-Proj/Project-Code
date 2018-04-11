/*
 * FollowWall, logic for following the wall and turning corners
 * Authors: Daniel Zadagen, Sean Troy, Alexander Noens
 * Date: 3/28/2017
 */

void FollowWall()
{

  //---------Get values from Side US sensor----------------------------------------------------
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
      Serial.print(FrontAvg);
      Serial.print("       ");
      Serial.print(SideAvg);
      Serial.print("\n");
  //Wait until there is a usable values for the front and side sensors, then follow the wall
  if ((Corner == false)&&(FrontReady)&&(SideReady)){
    
    //Side Sensor, if to far turn right
    if ((6.8 <= SideAvg)&&(SideAvg <= 7.2)){
      servo_LeftMotor.writeMicroseconds(1675);
      servo_RightMotor.writeMicroseconds(1675);
      //Serial.print("STRAIGHT");
      //Serial.println(SideAvg);
    }

      //Side Sensor, if too close turn left
      else if (SideAvg < 6.8){
        servo_LeftMotor.writeMicroseconds(1300);
        servo_RightMotor.writeMicroseconds(1700);
        //Serial.print("LEFT");
        //Serial.println(SideAvg); 
      }

        //side sensor, go straight
        else{
          servo_LeftMotor.writeMicroseconds(1700);
          servo_RightMotor.writeMicroseconds(1300);
          //Serial.print("RIGHT");
          //Serial.println(SideAvg);
        }

    //Check for corner, Front Sensor
    if ((FrontAvg) < 15){
      servo_LeftMotor.writeMicroseconds(1500);
      servo_RightMotor.writeMicroseconds(1500);
      //Serial.println("STOP");

      Corner = true;
    }
  }
  
 if(Corner == true){
  //ArmMotor.write(180);  
  //delay(1000);
  servo_LeftMotor.writeMicroseconds(1500);
  servo_RightMotor.writeMicroseconds(1750);
  //Serial.println("CRNR");
  if(FrontAvg > 45)
    Corner = false;
  //Corner Turning Command
  //Serial.println("STOP");
  }
}

