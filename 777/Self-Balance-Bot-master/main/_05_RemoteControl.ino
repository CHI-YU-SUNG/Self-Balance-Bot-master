float maxPowerRemap = 255/100;

// robust remoteControl
// Sometimes the bluetooth will not receive all data, it may contain noise, such as garbled data.
// if data is lost, then use the previous data
//void RemoteControl(String data){
//  int commaAt = -1;
//  int cmdLen = data.length();
//  for(int i=0; i<cmdLen; ++i){
//    if(data[i] == ','){
//      commaAt = i;
//      break;
//    }
//  }
//
//  if(commaAt != -1){
//    float Power = data.substring(0, commaAt).toFloat();
//    float Angle = data.substring(commaAt+1).toFloat();
//    float Sin = sin(Angle * DEG_TO_RAD);
//    float Cos = cos(Angle * DEG_TO_RAD);
//    float forward = Sin * Sin * (Sin<0?-1.0:1.0);
//    float diff = Cos * Cos / 2.0 * (Cos<0?-1.0:1.0) * (forward<0?-1.0:1.0);
//    float lRatio = forward + diff;
//    float rRatio = forward - diff;
//
//    //give command to wheels
//    motor_A.Rotate((int)lRatio * Power * maxPowerRemap);
//    motor_B.Rotate((int)rRatio * Power * maxPowerRemap);
//  }
//  data = "";
//}

void RemoteControl(){
  motorA.Rotate(data[1]);
  motorB.Rotate(data[2]);
}
