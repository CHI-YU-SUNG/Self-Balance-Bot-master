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
float ReferencePsi = 0; 
int satcount= 0;
float Calibration =0;

//set turn 
float TurnR =0.0;
float TurnL =0.0;
int TurnCount = 0;
float TurnRight=90; // 向右轉0度
float TurnLeft =0;  // 向左轉0度
//calculate time
float timercount =0;

int level=0;
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
      case 0:
          delay(2000);
          break;
      
      case 1:
          y=0;timercount=0;
          Calibration=-0.1;
          Addpsi(-60,8000,1);//克服最大靜摩擦，增加車傾角,//ms,//flag       
          break;
      case 2:
          turn(1,90,1);
          delay(1000);
          break;
      case 3:
          y=0;timercount=0;
  //        Calibration=-0.15;
          Addpsi(-85,10000,2);//克服最大靜摩擦，增加車傾角
          break;
      case 4:
          turn(1,90,2);//向轉90度 為(0,90)
          delay(2000);
          break;
      case 5:
          Calibration=-0.15;
          Addpsi(-60,15000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          break;
      case 6:
          turn(0,180,1);//向右轉90度 為(0,90)
          delay(3000);   
          break;
      

      case 7:
          //Calibration=-0.1;
          Addpsi(-15,3000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          turn(0,5,1);
          Addpsi(-20,4000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          turn(0,15,1);
          
          Addpsi(-20,4000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          turn(0,5,1);
          Addpsi(-20,3000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          turn(0,5,1);
          Addpsi(-20,2000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          //delay(10000);
          break;
      case 8:
          turn(0,90,1);        
          Addpsi(-15,2000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          turn(0,15,1);
          Addpsi(-15,3000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          
          delay(10000); 
          turn(0,5,1);
          Addpsi(-20,2000,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          delay(10000);  
      default: 
          break;
    }
           /*
      case 1:
          Calibration=-0.15;
          Addpsi(-75,8500,3);//克服最大靜摩擦，增加車傾角,//ms,//flag
          break;
      case 2:
          turn(0,180,1);//向右轉90度 為(0,90)   
          break;
      case 3:
          curve(-100);
          delay(5000);
          break;
      */
    if(level==8){
        level=0;
    }
    else
        level++;
    

    
   
    
    
    
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
