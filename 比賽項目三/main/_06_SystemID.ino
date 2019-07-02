// Goal: system id need constant 5v input

void StableVoltage(const int voltage){
    // voltage>0  --> move forward
    // voltage<0  --> move backward
    motorA.Rotate( voltage );
    motorB.Rotate( voltage );
}
