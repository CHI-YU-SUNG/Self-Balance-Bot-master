#ifndef BALANCEBOTENCODER_H
#define BALANCEBOTENCODER_H

#include <Arduino.h>
#define PPR 390

class BalanceBotEncoder{
  private:
    int motorSide;    // left: -1, right: 1
    
    // pin
    int interruptPin;
    int directionPin;

    // others
    int ppr;           // wheel angle (PPR)
    float angle;       // wheel angle (rad)
    float speed;       // wheel speed (rad/s)
    
   
  public:
    // constructor
    BalanceBotEncoder();

    // set funciton
    void SetMotorSide(const int side);
    void SetPin(const int interruptPin, const int directionPin);

    // get function
    int GetInterruptPin();
    float GetAngle();
    float GetSpeed();
    
    // update fuction
    void UpdatePPR();   // interrupt 
    void Update(const float samplingTime);
    
    // reset function
    void Reset();       // clear state 
};

#endif //BALANCEBOTENCODER_H
