//設定左右輪PID和左右馬達微調

void PID(){
      
    // update the current state
    psi = ((float)GetPsi())/180*PI;
    encoderA.Update(samplingTime);
    encoderB.Update(samplingTime);
    thetaL = encoderA.GetAngle();
    thetaR = encoderB.GetAngle();
    speedL = encoderA.GetSpeed();
    speedR = encoderB.GetSpeed();
    phi = R/W*(thetaL - thetaR);//
    x = R*(thetaL + thetaR)*sin(phi)/2;
    y = -R*(thetaL + thetaR)*cos(phi)/2;
    xSpeed = R*(speedL + speedR)*sin(phi)/2;
    ySpeed = -R*(speedL + speedR)*cos(phi)/2;

    //Serial.print(thetaL);
    //Serial.print(" ");
    //Serial.println(thetaR);
 

    
    // update the controller
    desire_psi = -posController.Update(y, samplingTime);
    psiController.SetReference(desire_psi);
    output = -psiController.Update(psi, samplingTime);
    
    
    // move the car
    motorA.Rotate( output );
    motorB.Rotate( output );


    // record the previous time
    previousTime = currentTime;
}

float OutputCalibration(float IntiOutput){//微調

  
}

