// Goal: Setup motor, encoder, controller

void SetupCar(){
/*-- pin setup ------------------------------------------------------------------*/
// If batterys are at backward & user see the car from the backward
// motor A(right motor) connected between A01 and A02
// motor B(left  motor) connected between B01 and B02
    const int PWMA = 5, PWMB = 9;               // Speed control 
    const int AIN1 = 6, BIN1 = 11;              // Direction +
    const int AIN2 = 4, BIN2 = 10;              // Direction -
    const int STBY = 7;                         // standby(停止)

    const int INTA = 2, INTB = 3;               // Encoder interrupt
    const int DIRA = A3, DIRB = 8;              // Encoder direction

    // left motor
    motorA.SetPin(PWMA, AIN1, AIN2, STBY);

    // right motor
    motorB.SetPin(PWMB, BIN1, BIN2, STBY);

    // left encoder
    encoderA.SetMotorSide(-1);
    encoderA.SetPin(INTA, DIRA);
    attachInterrupt(digitalPinToInterrupt(encoderA.GetInterruptPin()), EncoderAInterrupt, RISING);
    
    // right encoder
    encoderB.SetMotorSide(1);
    encoderB.SetPin(INTB, DIRB);
    attachInterrupt(digitalPinToInterrupt(encoderB.GetInterruptPin()), EncoderBInterrupt, RISING);

    // psi controller
    psiController.SetSaturation(12.0, -12.0);  // 12v ~ -12v
    psiController.SetToleratedError(0.02);     // 0.02rad
    psiController.SetPID(40, 105, 0.6);      // KP, KI, KD  30, 150, 0.6        //20,75,0.6
    psiController.SetReference(-0.02);           // Reference-0.02

    // pos Controller
    posController.SetSaturation(0.01, -0.01);  // 0.02 ~ -0.02rad
    posController.SetToleratedError(5);        // 5cm
    posController.SetPID(0.01, 0, 0.1);            // KP, KI, KD  0.001, 0.2 ,0
    posController.SetReference(Reference);           // Reference
}

void EncoderAInterrupt(){
    encoderA.UpdatePPR();
}

void EncoderBInterrupt(){
    encoderB.UpdatePPR();
}
