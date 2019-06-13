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
