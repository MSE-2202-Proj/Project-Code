/*

  MSE 2202 MSEBot base code for Labs 3 and 4
  Language: Arduino
  Authors: Michael Naish and Eugen Porter
  Date: 16/01/17

  Rev 1 - Initial version
  Rev 2 - Update for MSEduino v. 2

*/

#include <Servo.h>
#include <EEPROM.h>
#include <uSTimer2.h>
#include <CharliePlexM.h>
#include <Wire.h>
#include <I2CEncoder.h>

//#include <SoftwareSerial.h> //Not sure where to get this if you don't already have it.

Servo servo_RightMotor;
Servo servo_LeftMotor;


Servo CubeMotor;    //for cube grabbing motor
Servo ArmMotor;     // arm for cube grabbing motor
Servo LAPyramid;    //motor for linear actuator to collect pyramid
Servo PivotMotor;   //drop down arm to grab and pivot puramid

I2CEncoder encoder_RightMotor;
I2CEncoder encoder_LeftMotor;

//SoftwareSerial IRSensor(A3, A3);


// Uncomment keywords to enable debugging output

//#define DEBUG_MODE_DISPLAY
//#define DEBUG_MOTORS
//#define DEBUG_ENCODERS
//#define DEBUG_ULTRASONIC
//#define DEBUG_MOTOR_CALIBRATION


boolean bt_Motors_Enabled = true;
//LED STATUS STRING

//port pin constants


const int ci_Charlieplex_LED1 = 4;
const int ci_Charlieplex_LED2 = 5;
const int ci_Charlieplex_LED3 = 6;
const int ci_Charlieplex_LED4 = 7;
const int ci_Mode_Button = 7;


const int ci_Motor_Enable_Switch = 12;

const int ci_Light_Sensor = A3;
const int ci_I2C_SDA = A4;         // I2C data = white
const int ci_I2C_SCL = A5;         // I2C clock = yellow

// Charlieplexing LED assignments
const int ci_Heartbeat_LED = 1;
const int ci_Indicator_LED = 4;


//constants

// EEPROM addresses
const int ci_Left_Motor_Offset_Address_L = 12;
const int ci_Left_Motor_Offset_Address_H = 13;
const int ci_Right_Motor_Offset_Address_L = 14;
const int ci_Right_Motor_Offset_Address_H = 15;

const int ci_Left_Motor_Stop = 1500;        // 200 for brake mode; 1500 for stop
const int ci_Right_Motor_Stop = 1500;
const int ci_Grip_Motor_Open = 140;         // Experiment to determine appropriate value
const int ci_Grip_Motor_Closed = 70;        //  "
const int ci_Arm_Servo_Retracted = 55;      //  "
const int ci_Arm_Servo_Extended = 120;      //  "
const int ci_Display_Time = 500;
const int ci_Motor_Calibration_Cycles = 3;
const int ci_Motor_Calibration_Time = 5000;

//variables
byte b_LowByte;
byte b_HighByte;

unsigned int ui_Motors_Speed = 1600;        // Default run speed

unsigned long ul_3_Second_timer = 0;
unsigned long ul_Display_Time;
unsigned long ul_Calibration_Time;
unsigned long ui_Left_Motor_Offset;
unsigned long ui_Right_Motor_Offset;

unsigned int ui_Cal_Count;
unsigned int ui_Cal_Cycle;

unsigned int  ui_Robot_State_Index = 0;
//0123456789ABCDEF
unsigned int  ui_Mode_Indicator[6] = {
  0x00,    //B0000000000000000,  //Stop
  0x00FF,  //B0000000011111111,  //Run
  0x0F0F,  //B0000111100001111,  //Calibrate line tracker light level
  0x3333,  //B0011001100110011,  //Calibrate line tracker dark level
  0xAAAA,  //B1010101010101010,  //Calibrate motors
  0xFFFF   //B1111111111111111   //Unused
};

unsigned int  ui_Mode_Indicator_Index = 0;

//display Bits 0,1,2,3, 4, 5, 6,  7,  8,  9,  10,  11,  12,  13,   14,   15
int  iArray[16] = {
  1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 65536
};
int  iArrayIndex = 0;

boolean bt_Heartbeat = true;
boolean bt_3_S_Time_Up = false;
boolean bt_Do_Once = false;
boolean bt_Cal_Initialized = false;

//---------------------------- VARIABLES AND PIN ASSINGMENTS ------------------------------------------------
const int ci_Ultrasonic_Ping = 2;       //input plug
const int ci_Ultrasonic_Data = 3;       //output plug
const int ci_Ultrasonic_PingFront = 4;  //input plug
const int ci_Ultrasonic_DataFront = 5;  //output plug

const int LimitSwitch = 6;

const int ci_Right_Motor = 8;
const int ci_Left_Motor = 9;

const int IRFlag = A0;

unsigned int ui_Left_Motor_Speed;
unsigned int ui_Right_Motor_Speed;
long l_Left_Motor_Position;
long l_Right_Motor_Position;

//Robot Variables--------------------------------------------------------------------------
float EchoTimeSide;
float EchoTimeFront;
bool Start = false;
bool CubeGrabbed = false;
bool CubeDetected = false;
int StageCounter = 1;
int CubeStep = 0;
bool Corner = false;
bool PyramidFound = false;
int ArmLocation1 = 90;

//VARIABLES FOR PING----------------------------------------------------------------------
unsigned int SamplesFront = 1;
float CmFront = 0;
float FrontAvg = 0;
float TotFront = 0;
bool FrontReady = false;

unsigned int SamplesSide = 1;
float CmSide = 0;
float SideAvg = 0;
float TotSide = 0;
bool SideReady = false;
//-----------------------------------------------------------------------------

//IR pinouts-----------------------------------------------------------------------------


void setup() {
  Wire.begin();        // Wire library required for I2CEncoder library
  Serial.begin(9600);

  CharliePlexM::setBtn(ci_Charlieplex_LED1, ci_Charlieplex_LED2,
                       ci_Charlieplex_LED3, ci_Charlieplex_LED4, ci_Mode_Button);

  //******************************** ROBOT SETUP ********************************************//
  // set up side ultrasonic
  pinMode(ci_Ultrasonic_Ping, OUTPUT);
  pinMode(ci_Ultrasonic_Data, INPUT);

  //set up front ultrasonic
  pinMode(ci_Ultrasonic_PingFront, OUTPUT);
  pinMode(ci_Ultrasonic_DataFront, INPUT);

  // set up drive motors
  pinMode(ci_Right_Motor, OUTPUT);
  servo_RightMotor.attach(ci_Right_Motor);
  pinMode(ci_Left_Motor, OUTPUT);
  servo_LeftMotor.attach(ci_Left_Motor);

  //Set up IR sense
  pinMode(A0, INPUT);
  digitalWrite(A0,LOW);
  
  //set up limit switch
  pinMode(LimitSwitch, INPUT);
  pinMode(A1, INPUT);
  digitalWrite(A0, HIGH);
  /*Code for IR micro cont.
  IRSensor.begin(2400);
  pinMode(A3, INPUT); //Use these in set up. It's important
  */
  
  //Set up Cube Grabbing servo
  CubeMotor.attach(10);
  CubeMotor.write(180);
  delay(1000);
  CubeMotor.detach();

  //Set up cube grabbing arm servo
  ArmMotor.attach(11);
  ArmMotor.write(120);  
  delay(1000);
  ArmMotor.detach();

  //Set up LAMotor for pyramid
  LAPyramid.attach(13);
  LAPyramid.write(0);
  delay(1000);
  LAPyramid.detach();

  //Set up pivot motor
  PivotMotor.attach(12);
  PivotMotor.write(110);
  delay(1000);
  //PivotMotor.detach();

  // set up encoders. Must be initialized in order that they are chained together,
  // starting with the encoder directly connected to the Arduino. See I2CEncoder docs
  // for more information
  encoder_LeftMotor.init(1.0 / 3.0 * MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_LeftMotor.setReversed(false);  // adjust for positive count when moving forward
  encoder_RightMotor.init(1.0 / 3.0 * MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_RightMotor.setReversed(true);  // adjust for positive count when moving forward

  // read saved values from EEPROM
  b_LowByte = EEPROM.read(ci_Left_Motor_Offset_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Motor_Offset_Address_H);
  ui_Left_Motor_Offset = word(b_HighByte, b_LowByte);
  b_LowByte = EEPROM.read(ci_Right_Motor_Offset_Address_L);
  b_HighByte = EEPROM.read(ci_Right_Motor_Offset_Address_H);
  ui_Right_Motor_Offset = word(b_HighByte, b_LowByte);

}

void loop(){
  
  if (CharliePlexM::ui_Btn){
    if (bt_Do_Once == false){
      bt_Do_Once = true;
      Start = true;
    }
  }
  else{
    bt_Do_Once = LOW;
  }

  if(Start){

    //Serial.println("Stage Pos.");
    //Serial.println(StageCounter);
    
    switch(StageCounter){
    
    case 1:
      
      Serial.println("Stage 1");
      //delay(500);
      //if cube it not detected...follow wall and find cube
      if (CubeStep == 0){
        FollowWall();
        Cube();
      }
  
      //if cube is detected...get cube
      if (CubeStep >= 1){
        Serial.println("Cube flag UP");
        Cube();
      }
    
    break;
    
    case 2:
      Serial.println("Stage 2");
      FindPyramid();
    break;

    case 3:
       Serial.println("Stage 2");
      GetPyramid();
    break;
    }  
  }
  
}


// ******************************* LOOP **********************************************

/*
void loop()
{
  if ((millis() - ul_3_Second_timer) > 3000)
  {
    bt_3_S_Time_Up = true;
  }

  // button-based mode selection
  if (CharliePlexM::ui_Btn)
  {
    if (bt_Do_Once == false)
    {
      bt_Do_Once = true;
      ui_Robot_State_Index++;
      ui_Robot_State_Index = ui_Robot_State_Index & 7;
      ul_3_Second_timer = millis();
      bt_3_S_Time_Up = false;
      bt_Cal_Initialized = false;
      ui_Cal_Cycle = 0;

    }
  }
  else
  {
    bt_Do_Once = LOW;
  }

  // check if drive motors should be powered
  bt_Motors_Enabled = digitalRead(ci_Motor_Enable_Switch);

  // modes
  // 0 = default after power up/reset
  // 1 = Press mode button once to enter. Run robot.
  // 2 = Press mode button twice to enter. Calibrate line tracker light level.
  // 3 = Press mode button three times to enter. Calibrate line tracker dark level.
  // 4 = Press mode button four times to enter. Calibrate motor speeds to drive straight.
  switch (ui_Robot_State_Index)
  {
    case 0:    //Robot stopped
      {

        Serial.println(EchoTimeSide / 58.0);
        Ping();
        // PingFront();
        servo_LeftMotor.writeMicroseconds(ci_Left_Motor_Stop);
        servo_RightMotor.writeMicroseconds(ci_Right_Motor_Stop);
        CubeMotor.write(180);
        ArmMotor.write(90);
        encoder_LeftMotor.zero();
        encoder_RightMotor.zero();
        ui_Mode_Indicator_Index = 0;
        break;
      }

    //***********************************************************************************
    //****************************** ROBOT CODE *****************************************
    //***********************************************************************************

    case 1:    //Robot Run after 3 seconds
      {

        //Find Cube
        if (StageCounter == 1)
        {
          //if cube it not detected...follow wall and find cube
          if (CubeDetected == false)
          {
            FollowWall();
            Cube();
          }

          //if cube is detected...get cube
          if (CubeDetected == true)
          {
            Serial.println("Cube flag UP");
            Cube();

          }

        }

        //Find Pyramid
        if (StageCounter == 2)
        {
          FindPyramid();
        }

        //Get Pyramid + Tilt it
        if (StageCounter == 3)
        {
          GetPyramid();
        }

        //Put cube inside pyramid
        if (StageCounter == 4)
        {
          FinalStep();
        }







#ifdef DEBUG_ENCODERS
        l_Left_Motor_Position = encoder_LeftMotor.getRawPosition();
        l_Right_Motor_Position = encoder_RightMotor.getRawPosition();

        Serial.print("Encoders L: ");
        Serial.print(l_Left_Motor_Position);
        Serial.print(", R: ");
        Serial.println(l_Right_Motor_Position);
#endif

        // set motor speeds
        ui_Left_Motor_Speed = constrain(ui_Motors_Speed + ui_Left_Motor_Offset, 1550, 2100);
        ui_Right_Motor_Speed = constrain(ui_Motors_Speed + ui_Right_Motor_Offset, 1550, 2100);



#ifdef DEBUG_MOTORS
        Serial.print("Motors enabled: ");
        Serial.print(bt_Motors_Enabled);
        Serial.print(", Default: ");
        Serial.print(ui_Motors_Speed);
        Serial.print(", Left = ");
        Serial.print(ui_Left_Motor_Speed);
        Serial.print(", Right = ");
        Serial.println(ui_Right_Motor_Speed);
        //Serial.println(ledint);
        Serial.println(mtrspd);
#endif
        ui_Mode_Indicator_Index = 1;
      }
      break;






    case 4:    //Calibrate motor straightness after 3 seconds.
      {
        if (bt_3_S_Time_Up)
        {
          if (!bt_Cal_Initialized)
          {
            bt_Cal_Initialized = true;
            encoder_LeftMotor.zero();
            encoder_RightMotor.zero();
            ul_Calibration_Time = millis();
            servo_LeftMotor.writeMicroseconds(ui_Motors_Speed);
            servo_RightMotor.writeMicroseconds(ui_Motors_Speed);
          }
          else if ((millis() - ul_Calibration_Time) > ci_Motor_Calibration_Time)
          {
            servo_LeftMotor.writeMicroseconds(ci_Left_Motor_Stop);
            servo_RightMotor.writeMicroseconds(ci_Right_Motor_Stop);
            l_Left_Motor_Position = encoder_LeftMotor.getRawPosition();
            l_Right_Motor_Position = encoder_RightMotor.getRawPosition();
            if (l_Left_Motor_Position > l_Right_Motor_Position)
            {
              // May have to update this if different calibration time is used
              ui_Right_Motor_Offset = 0;
              ui_Left_Motor_Offset = (l_Left_Motor_Position - l_Right_Motor_Position) / 4;
            }
            else
            {
              // May have to update this if different calibration time is used
              ui_Right_Motor_Offset = (l_Right_Motor_Position - l_Left_Motor_Position) / 4;
              ui_Left_Motor_Offset = 0;
            }

#ifdef DEBUG_MOTOR_CALIBRATION
            Serial.print("Motor Offsets: Left = ");
            Serial.print(ui_Left_Motor_Offset);
            Serial.print(", Right = ");
            Serial.println(ui_Right_Motor_Offset);
#endif
            EEPROM.write(ci_Right_Motor_Offset_Address_L, lowByte(ui_Right_Motor_Offset));
            EEPROM.write(ci_Right_Motor_Offset_Address_H, highByte(ui_Right_Motor_Offset));
            EEPROM.write(ci_Left_Motor_Offset_Address_L, lowByte(ui_Left_Motor_Offset));
            EEPROM.write(ci_Left_Motor_Offset_Address_H, highByte(ui_Left_Motor_Offset));

            ui_Robot_State_Index = 0;    // go back to Mode 0
          }
#ifdef DEBUG_MOTOR_CALIBRATION
          Serial.print("Encoders L: ");
          Serial.print(encoder_LeftMotor.getRawPosition());
          Serial.print(", R: ");
          Serial.println(encoder_RightMotor.getRawPosition());
#endif
          ui_Mode_Indicator_Index = 4;
        }
        break;
      }
  }

  if ((millis() - ul_Display_Time) > ci_Display_Time)
  {
    ul_Display_Time = millis();

#ifdef DEBUG_MODE_DISPLAY
    Serial.print("Mode: ");
    Serial.println(ui_Mode_Indicator[ui_Mode_Indicator_Index], DEC);
#endif
    bt_Heartbeat = !bt_Heartbeat;
    CharliePlexM::Write(ci_Heartbeat_LED, bt_Heartbeat);
    digitalWrite(13, bt_Heartbeat);
    Indicator();
  }
}
*/

// set mode indicator LED state
void Indicator()
{
  //display routine, if true turn on led
  CharliePlexM::Write(ci_Indicator_LED, !(ui_Mode_Indicator[ui_Mode_Indicator_Index] &
                                          (iArray[iArrayIndex])));
  iArrayIndex++;
  iArrayIndex = iArrayIndex & 15;
}





// measure distance to target using ultrasonic sensor in the SIDE
void Ping()
{
  //Ping Ultrasonic
  //Send the Ultrasonic Range Finder a 10 microsecond pulse per tech spec
  digitalWrite(ci_Ultrasonic_Ping, HIGH);
  delayMicroseconds(10);  //The 10 microsecond pause where the pulse in "high"
  digitalWrite(ci_Ultrasonic_Ping, LOW);
  //use command pulseIn to listen to Ultrasonic_Data pin to record the
  //time that it takes from when the Pin goes HIGH until it goes LOW
  EchoTimeSide = pulseIn(ci_Ultrasonic_Data, HIGH, 10000);

  // Print Sensor Readings
#ifdef DEBUG_ULTRASONIC
  Serial.print("Time (microseconds): ");
  Serial.print(EchoTimeSide, DEC);
  Serial.print(", Inches: ");
  Serial.print(EchoTimeSide / 148); //divide time by 148 to get distance in inches
  Serial.print(", cm: ");
  Serial.println(EchoTimeSide / 58); //divide time by 58 to get distance in cm
#endif
}


// measure distance to target using ultrasonic sensor in the FRONT
void PingFront()
{
  //Ping Ultrasonic
  //Send the Ultrasonic Range Finder a 10 microsecond pulse per tech spec
  digitalWrite(ci_Ultrasonic_PingFront, HIGH);
  delayMicroseconds(10);  //The 10 microsecond pause where the pulse in "high"
  digitalWrite(ci_Ultrasonic_PingFront, LOW);
  //use command pulseIn to listen to Ultrasonic_Data pin to record the
  //time that it takes from when the Pin goes HIGH until it goes LOW
  EchoTimeFront = pulseIn(ci_Ultrasonic_DataFront, HIGH, 10000);

  // Print Sensor Readings
#ifdef DEBUG_ULTRASONIC
  Serial.print("Time (microseconds): ");
  Serial.print(EchoTimeFront, DEC);
  Serial.print(", Inches: ");
  Serial.print(EchoTimeFront / 148); //divide time by 148 to get distance in inches
  Serial.print(", cm: ");
  Serial.println(EchoTimeFront / 58); //divide time by 58 to get distance in cm
#endif
}







