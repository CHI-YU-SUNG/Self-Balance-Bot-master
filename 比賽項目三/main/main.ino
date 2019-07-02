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

//bool flag = false;
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
float HI;
int flag = 0;
int c = 0;
float cP = 0.025;
int level = 0;
int shape = 0, color = 0;
int look=0;
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
  //執行完此次任務後就往打開相機接收下一個任務
  if (Serial.available() && look == 1) {
    Serial.println(look);
    color = Serial.read();
    shape = Serial.read();
    look = 0;
  }
  if (shape == 1) {     //三角形
    if (color == 1) {   //藍色
      turn(1, 90);   //向左旋90度
      delay(1000);
      Addpsi(-180, 1000, 1); //前進180單位，delay1000，第一個case
    }
    else if (color == 2) { //紅色
      turn(1, 90);   //向左旋90度
      delay(1000);
      Addpsi(-60, 1000, 1); //前進60單位，delay1000，第一個case
    }
    else if (color == 3) { //綠色
      turn(1, 90);   //向左旋90度
      delay(1000);
      Addpsi(-120, 1000, 1); //前進120單位，delay1000，第一個case
    }
  }
  else if (shape == 2) { //矩形
    if (color == 1) { //藍色
      turn(0, 90); //向右轉90度 為(0,90)
      delay(1000);
      Addpsi(-180, 1000, 1); //前進180單位，delay1000，第一個case
    }
    else if (color == 2) { //紅色
      turn(0, 90); //向右轉90度 為(0,90)
      delay(1000);
      Addpsi(-60, 1000, 1); //前進60單位，delay1000，第一個case
    }
    else if (color == 3) { //綠色
      turn(0, 90); //向右轉90度 為(0,90)
      delay(1000);
      Addpsi(-120, 1000, 1); //前進120單位，delay1000，第一個case
    }
  }
  else if (shape == 3) {  //圓形
    if (color == 1) {   //藍色
      delay(3000);
      posController.SetPID(0, 0, 0);
      psiController.SetPID(0, 0, 0);
    }
    else if (color == 2) { //紅色
      delay(1000);
      posController.SetPID(0, 0, 0);
      psiController.SetPID(0, 0, 0);
    }
    else if (color == 3) { //綠色
      delay(2000);
      posController.SetPID(0, 0, 0);
      psiController.SetPID(0, 0, 0);
    }
  }
  look=1;//接收下一個指令


  //安全措施
  if (HI > 45 || HI < -45) {
    psiController.SetPID(0, 0, 0);
  }
}
