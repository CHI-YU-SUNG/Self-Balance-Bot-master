//設定左右輪PID和左右馬達微調

void PID(){
 
    // update the current state
    HI=GetPsi();
    psi = ((float)HI)/180*PI;
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
    desire_psi = -(posController.Update(y, samplingTime)+ReferencePsi);
    psiController.SetReference(desire_psi);
    output = -psiController.Update(psi, samplingTime);
    
    
    //move car 驅動馬達+微調馬達
    OutputCalibration( 0 );    //output +微調不可以超過255    

    // record the previous time
    previousTime = currentTime;
}

float OutputCalibration(float Calibration){   //微調左右馬達
    float outputA = output + TurnL ;
    float outputB = output + Calibration + TurnR ;
    // move the car
    motorA.Rotate( outputA );
    motorB.Rotate( outputB );
  
}
