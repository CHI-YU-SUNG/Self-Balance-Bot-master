// Goal: Select the control mode

void Addpsi(float distance){//克服最大靜摩擦
    posController.SetReference(distance);           // Reference
    while(distance != 0){  
      bool done=false;
      for(int i=0;i<20;i++){
          posController.SetSaturation(0.03, -0.03);
          delay(1);
      }
      posController.SetSaturation(0.02, -0.02);  
      do{  
      if (y==distance)
          done=true;
      }while(!done);
      delay(3000);//等車子到位再下一個動作
    }

}

void turn(int direct,float angle){
    bool done = false;
    do{
      
          if (direct==0 && angle==90){//右轉90
            for(int i=0;i<360;i++){
                TurnR =1;
                TurnL =0;
                delay(5);
            }
          }
          if (direct==1 && angle==90){//左轉90
            for(int i=0;i<360;i++){
                TurnR =0;
                TurnL =1;
                delay(5);
            }
          }
          if (direct==0 && angle==180){//右轉180
            for(int i=0;i<720;i++){
                TurnR =1;
                TurnL =0;
                delay(5);
            }
          }
          if (direct==1 && angle==180){//左轉180
            for(int i=0;i<720;i++){
                TurnR =0;
                TurnL =1;
                delay(5);
            }
          }
          if (direct==1 && angle==30){//左轉隨便角度
            for(int i=0;i<90;i++){
                TurnR =0;
                TurnL =1;
                delay(5);
            }
          }
            done=true;
          
         //Serial.println(TurnR); 
       
    }while(!done);
    TurnR=0.0;
    TurnL=0.0;
}

void curve(float distance){
    bool done = false;
    posController.SetReference(distance);           // Reference
    do{//調整曲度
        for(int i=0;i<40;i++){
            TurnR =1;
            TurnL =0;
            delay(25);
         }
        
        
        
        done=true;
    } while(!done);
    TurnR=0.0;
    TurnL=0.0;
  
}



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
