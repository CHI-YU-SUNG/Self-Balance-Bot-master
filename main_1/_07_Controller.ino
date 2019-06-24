// Goal: Select the control mode

void Addpsi(float distance, float wait, float flag) { //克服最大靜摩擦
  posController.SetReference(distance);           // Reference
  posController.SetPID(3.5, 3.5, 1.55);
  timercount=0;
  ReferencePsi = 0;
  //while(distance != 0){
  bool done = false;
  //do{
  if (flag == 1) {
      for (int i = 0; i < 10; i++) {
      posController.SetSaturation(0.015, -0.015);
      delay(1);
      }
      posController.SetSaturation(0.015, -0.015);
      //ReferencePsi = 0.0105;
  } else if (flag == 2) {
    for (int i = 0; i < 1000; i++) {
      posController.SetSaturation(0.025, -0.025);
      ReferencePsi = 0.004;
      delay(5);
    }
    posController.SetSaturation(0.01, -0.01);
  } else if (flag == 3) {
    for (int i = 0; i < 20; i++) {
      ReferencePsi = -0.005;
      posController.SetSaturation(0.039, -0.039);
      delay(1);
    }
    posController.SetSaturation(0.011, -0.011);
  }else if(flag ==4){
      for (int i = 0; i < 10; i++) {
      posController.SetSaturation(0.04, -0.04);
      delay(1);
    }
      posController.SetSaturation(0.015, -0.015);
      
    }


  //   if (abs(y-distance)<10)
  //     done=true;
  // }while(!done);
  Calibration = 0;
     ReferencePsi = 0;
  delay(wait);//等車子到位再下一個動作

  //while(timercount>wait-2000&&distance-y>10){
   //     ReferencePsi=-0.002;
   //   }

  //}

}

void turn(int direct, float angle , float flag) {
  bool done = false;
  posController.SetSaturation(0.015, -0.015);
  //  do{

  if (direct == 0 && angle == 90) { //右轉90
    posController.SetPID(0, 0, 0);
    posController.SetReference(0);
    posController.SetSaturation(0.025, -0.025);
    for (int i = 0; i < 520; i++) {
      if (i >= 400) {
        TurnR = 1;
        TurnL = 0;

      } else {
        TurnR = 1;
        TurnL = 0;
        delay(5);
      }
    }
    encoderA.Reset();
    encoderB.Reset();
    posController.SetPID(0.5, 0.5, 0.01);
    ReferencePsi = 0.01;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (direct == 1 && angle == 90) { //左轉90
    if (flag == 1) {
      posController.SetPID(0, 0, 0);
      posController.SetReference(0);
      posController.SetSaturation(0.025, -0.025);
      for (int i = 0; i < 110; i++) {
        if (i == 200) {
          TurnR = -2;
          TurnL = 2;

        } else {
          TurnR = -2;
          TurnL = 2;
          delay(5);
        }
      }
      encoderA.Reset();
      encoderB.Reset();
      posController.SetPID(0.5, 0.5, 0.01);
      ReferencePsi = 0.02;
    }
    if (flag == 2) {
      posController.SetPID(0, 0, 0);
      posController.SetReference(0);
      posController.SetSaturation(0.02, -0.02);
      for (int i = 0; i < 110; i++) {
        if (i == 270) {
          TurnR = -2;
          TurnL = 2;

        } else {
          TurnR = -2;
          TurnL = 2;
          delay(5);
        }
      }
      encoderA.Reset();
      encoderB.Reset();
      posController.SetPID(0.5, 0.5, 0.01);
      ReferencePsi = 0.01;
    }

  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (direct == 0 && angle == 180) { //右轉180
    posController.SetPID(0, 0, 0);
    posController.SetReference(0);
    posController.SetSaturation(0.025, -0.025);
    for (int i = 0; i < 365; i++) {
      if (i == 365) {
        TurnR = 1;
        TurnL = -1;
      } else if (i > 180) {
        ReferencePsi = 0.01;
        TurnR = 1;
        TurnL = -1;
        delay(5);
      } else
      {
        ReferencePsi = -0.003;
        TurnR = 1;
        TurnL = -1;
        delay(5);
      }
    }
    encoderA.Reset();
    encoderB.Reset();
    posController.SetPID(0.5, 0.2, 0.01);

  }
  if (direct == 1 && angle == 180) { //左轉180
    for (int i = 0; i < 720; i++) {
      TurnR = 0;
      TurnL = 1;
      delay(5);
    }
  }
  if (direct == 0 && angle == 15) { //右轉15
    posController.SetPID(0, 0, 0);
    posController.SetReference(0);
    posController.SetSaturation(0.025, -0.025);
    for (int i = 0; i < 90; i++) {
      if (i == 89) {
        TurnR = 1;
        TurnL = -1;
      } else  {
        //ReferencePsi = -0.01;
        TurnR = 1;
        TurnL = -1;
        delay(5);
      }      
    }
    encoderA.Reset();
    encoderB.Reset();
    posController.SetPID(0.5, 0.2, 0.01);
  }
  if (direct == 0 && angle == 5) { //右轉5
    posController.SetPID(0, 0, 0);
    posController.SetReference(0);
    posController.SetSaturation(0.025, -0.025);
    for (int i = 0; i < 30; i++) {
      if (i == 29) {
        TurnR = 1;
        TurnL = -1;
      } else  {
        //ReferencePsi = -0.01;
        TurnR = 1;
        TurnL = -1;
        delay(5);
      }      
    }
    encoderA.Reset();
    encoderB.Reset();
    posController.SetPID(0.5, 0.2, 0.01);
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
  // posController.SetSaturation(0.01, -0.01);
  for (int i = 0; i < 100; i++) {
    //   if (i <= 5) {
    TurnR = 0;
    TurnL = -5;
    ReferencePsi = -0.035;
  }
  /*
    } else if ( i > 20 & i < 50) {
    TurnR = 0;
    TurnL = 0;
    ReferencePsi = -0.035;
    delay(25);
    } else if (i >= 80 & i <= 100) {

    TurnR = 0;
    TurnL = 0;
    ReferencePsi = -0.035;
    delay(25);
    } else {
    ReferencePsi = -0.025;
    delay(25);
    TurnR = 3;
    TurnL = 2;
    delay(25);
    TurnR = 3;
    TurnL = 2;
    delay(25);
    }


    }
  */
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
