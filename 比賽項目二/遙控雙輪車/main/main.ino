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
float samplingTime = 0.004;
float output;
float output_psi;
float output_pos;
float desire_psi;

bool flag = false;
bool SaturationAdd = false;

//set saturation
float Reference = 0;       //移動距離(負的定為往前)
float ReferencePsi = 0;
int satcount = 0;

//set turn
float TurnR = 0.0;
float TurnL = 0.0;
int TurnCount = 0;
float TurnRight = 90; // 向右轉0度
float TurnLeft = 0; // 向左轉0度

int c = 0;
float cP=0.025;
int level = 0;
void setup() {

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
void loop() {
  ReceiveBTdata();


}

void ReceiveBTdata() {
  //Read PID from cellphone
  if (BTSerial.available()) {
    c = BTSerial.read();
    Serial.println(c);
    if(c>=10){
     cP=0.025+(c-10)*0.001;
    }  
    else if (c == 0){
      ReferencePsi = 0;
      TurnL = 0;
      TurnR = 0;
    }
    else if (c == 1)
      ReferencePsi = -cP;
    else if (c == 2)
      ReferencePsi = cP;
    else if (c == 3) {
      TurnL = 1;
      TurnR = -1;
    }
    else if (c == 4) {
      TurnL = -1;
      TurnR = 1;
    }


  }
}
