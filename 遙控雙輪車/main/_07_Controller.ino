// Goal: Select the control mode

void Addpsi(float distance, float wait, float flag) { //克服最大靜摩擦
  posController.SetReference(distance);           // Reference
  posController.SetPID(0.7, 0.2, 0.01);
  ReferencePsi = 0;
  //while(distance != 0){
  bool done = false;
  //do{
  if (flag == 1) {
    for (int i = 0; i < 10; i++) {
      posController.SetSaturation(0.03, -0.03);
      delay(1);
    }
  } else if (flag == 2) {
    for (int i = 0; i < 20; i++) {
      posController.SetSaturation(0.03, -0.03);
      delay(1);
    }
  }

  posController.SetSaturation(0.01, -0.01);
  //   if (abs(y-distance)<10)
  //     done=true;
  // }while(!done);
  delay(wait);//等車子到位再下一個動作

  //}

}

void turn(int direct, float angle) {
  bool done = false;
  int flag = 0;
  posController.SetSaturation(0.01, -0.01);
  //  do{

  if (direct == 0 && angle == 90) { //右轉90
    for (int i = 0; i < 330; i++) {
      TurnR = 1;
      TurnL = 0;
      delay(5);
    }
  }
  if (direct == 1 && angle == 90) { //左轉90

    if (direct == 1 && angle == 90) { //左轉90
      posController.SetPID(0, 0, 0);
      posController.SetReference(0);
      posController.SetSaturation(0.025, -0.025);
      for (int i = 0; i < 360; i++) {
        if (i == 360) {
          TurnR = 0;
          TurnL = 1;

        } else {
          TurnR = 0;
          TurnL = 1;
          delay(5);
        }

      }
      encoderA.Reset();
      encoderB.Reset();
      posController.SetPID(0.5, 0.2, 0.01);
      ReferencePsi = 0.025;
    }


  }
  if (direct == 0 && angle == 180) { //右轉180
    for (int i = 0; i < 720; i++) {
      TurnR = 1;
      TurnL = 0;
      delay(5);
    }
  }
  if (direct == 1 && angle == 180) { //左轉180
    for (int i = 0; i < 720; i++) {
      TurnR = 0;
      TurnL = 1;
      delay(5);
    }
  }
  if (direct == 1 && angle == 30) { //左轉隨便角度
    for (int i = 0; i < 90; i++) {
      TurnR = 0;
      TurnL = 1;
      delay(5);
    }
  }
  done = true;

  //Serial.println(TurnR);
  delay(5);
  SetupCar();
  //  }while(!done);
  TurnR = 0.0;
  TurnL = 0.0;

}

void curve(float distance) {
  bool done = false;
  posController.SetPID(0, 0, 0);
  posController.SetReference(0);
  //   posController.SetReference(distance);           // Reference
  //do{//調整曲度
  
  for (int i = 0; i < 100; i++) {
    if (i <= 5) {
      TurnR = 1;
      TurnL = 1;
      ReferencePsi = -0.035;
    }
    ReferencePsi = -0.025;
    delay(25);
    TurnR = 3;
    TurnL = 2;
    delay(25);
    TurnR = 3;
    TurnL = 2;
    delay(25);
    if (i >= 80) {

      TurnR = 0;
      TurnL = 0;
      ReferencePsi = 0.025;
      delay(25);
    }
  }
  encoderA.Reset();
  encoderB.Reset();




  delay(5);
  SetupCar();






  done = true;
  //} while(!done);
  TurnR = 0.0;
  TurnL = 0.0;

}



void SetController() {
  encoderA.Reset();
  encoderB.Reset();
  String s = "";
  switch ( (int)data[2] ) {
    case 0:  // nothing
      break;
    case 1:  // psi
      psiController.SetPID(data[3], data[4], data[5]);
      psiController.SetReference(data[6]);
      break;
  }
}
