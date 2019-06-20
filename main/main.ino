# include <I2Cdev.h>
# include <MPU6050.h>
# include <MsTimer2.h>
# include <BalanceBotMotor.h>
# include <BalanceBotEncoder.h>
# include <BalanceBotController.h>
# include <Wire.h>
# include <Kalman.h>
# include <SoftwareSerial.h>

// constant
const float R = 3.25;
const float W = 18.7;

// Bluetooth
SoftwareSerial BTSerial(12, 13);    //tx, rx

// motor
BalanceBotMotor motorA;
BalanceBotMotor motorB;

// encoder
BalanceBotEncoder encoderA;
BalanceBotEncoder encoderB;

// controller
BalanceBotController psiController;
BalanceBotController posController;

// parameter
float psi;
float thetaL;
float thetaR;
float speedL;
float speedR;
float phi;
float x;
float y;
float xSpeed;
float ySpeed;

// sampling
unsigned long previousTime;
unsigned long currentTime;
float samplingTime=0.005;
float output;
float output_psi;
float output_pos;
float desire_psi;

bool flag = false;
bool SaturationAdd = false;

//set saturation
float Reference = 0;       //移動距離(負的定為往前)
int satcount= 0;

//set turn 
float TurnR =0.0;
float TurnL =0.0;
int TurnCount = 0;
float TurnRight=90; // 向右轉0度
float TurnLeft =0;  // 向左轉0度

int level=1;
void setup(){
    
    Serial.begin(115200);
    BTSerial.begin(57600);
    
    // initialization
    SetupCar();
    SetupMPU6050();
    SetupMsTimer();
    
    // initialize the previous time
    previousTime = millis();
}

// update controller
void loop(){
    switch(level){
      case 6:
          //Reference=10;
          Addpsi(-20);//克服最大靜摩擦，增加車傾角
          break;
      case 5:
          turn(1,90);
          break;
      case 2:
          delay(9000);
          break;
      case 3:
          turn(0,90);//向右轉90度 為(0,90)
          break;
      case 1:
          curve(-10);
          
      default: 
          break;
    }
      level++;
     //Addpsi(-50);//克服最大靜摩擦，增加車傾角
     //turn(0,90);//向右轉90度 0,90
     //Reference=-30;
     //posController.SetReference(-30);           // Reference
     //Addpsi(Reference);//克服最大靜摩擦，增加車傾角
    //Serial.println(TurnR);
    //Serial.println(TurnL);


    
    
    
    
    /*
    // compute the sampling time
    currentTime = millis();
    samplingTime = (currentTime - previousTime)/1000.0;
    // in case for samplingTime = 0
    if( samplingTime < 0.001 )
        samplingTime = 0.001;
     */  
    //ReceiveData();
    //SendData();

    
    
    
    
    
}
