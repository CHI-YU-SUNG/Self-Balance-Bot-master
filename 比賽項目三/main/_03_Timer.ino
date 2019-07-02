// Goal: Bluetooth

bool startReadData = false;
String rawData = "";
int len = 0;
float *data = new float[12];
int count = 0;

// sampling time
// max sampling time: 0.025s
// max sampling rate: 40Hz
void SetupMsTimer(){
    MsTimer2::set(samplingTime, TimerInterrupt);  // 0.005s update
    MsTimer2::start();
}

void TimerInterrupt(){
    sei();
    PID();
    
//    Serial.print(thetaL);
 //    Serial.print(" ");
 //      Serial.println(ReferencePsi);

}

// start: "~"
// end: "#"
void ReceiveData(){
    while(BTSerial.available()){
        char c = BTSerial.read();

        if(startReadData){
            if(c=='#'){
                //Serial.println(rawData);
                rawData += ',';
                ParseData();
            
                if(len>=2){
                    if( ((int)data[0]) == 1 ){
                        switch( (int)data[1] ){
                            case 1:
                                break;
                            case 2:
                                SetController();
                                break;
                            case 3:
                                break;
                        }
                    }
                    else if( ((int)data[0]) == 2 ){
                        SendParticularData((int)data[1]);
                    }
                }
                else{
                    BTSerial.println("");
                }
                
                startReadData = false;
                rawData = "";
                len = 0;
                break;
            }
            else{
                rawData += String(c);
            }
        }
        else if(c=='~'){
            startReadData = true;
        }
    }
}

void ParseData(){
    char *temp = new char[rawData.length()];
    rawData.toCharArray(temp, rawData.length());
    char *token;
    
    // get the first token 
    token = strtok(temp, ",");
    do{
        data[len++] = atof(token);
        token = strtok (NULL, ",");
    } while (token != NULL);
}

void SendData(){
    String data = "~1,1," + String(psi, 3) + "," + String(speedL, 3) + "," + String(speedR, 3) + "," + String(output, 3) + "," + String(x) + "," + String(y) + "," + String(samplingTime, 4) + "#";
    BTSerial.println(data);
}

void SendParticularData(int mode){
    String data = "";
    switch(mode){
        case 1:
            data = "~1,2," + psiController.GetInformation() + "#";
            break;
        case 2:
            data = "~1,2," + posController.GetInformation() + "#";
            break;
        case 3:
            break;
    }
    BTSerial.println(data);
}
