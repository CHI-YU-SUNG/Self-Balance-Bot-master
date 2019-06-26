// Goal: Select the control mode

void SetController(){
    encoderA.Reset();
    encoderB.Reset();
    String s = "";
    switch( (int)data[2] ){
        case 0:  // nothing
            break;
        case 1:  // psi
            psiController.SetPID(data[3], data[4], data[5]);
            psiController.SetReference(data[6]);
            break;
    }
}
