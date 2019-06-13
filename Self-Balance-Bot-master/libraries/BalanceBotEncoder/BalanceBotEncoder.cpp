#include "BalanceBotEncoder.h"

// constructor
BalanceBotEncoder::BalanceBotEncoder(){
    ppr = 0;
    angle = 0;
    speed = 0;
}

// set function
void BalanceBotEncoder::SetMotorSide(const int side){
    motorSide = side;
}

void BalanceBotEncoder::SetPin(const int interruptPin, const int directionPin){
    this->interruptPin = interruptPin;
    
    this->directionPin = directionPin;
    pinMode(this->directionPin, INPUT);
}

// get function
int BalanceBotEncoder::GetInterruptPin(){
    return interruptPin;
}

float BalanceBotEncoder::GetAngle(){
    return angle * motorSide;
}

float BalanceBotEncoder::GetSpeed(){
    return speed * motorSide;
}

// update function
void BalanceBotEncoder::UpdatePPR(){  // interrupt
    digitalRead(directionPin) == HIGH ? ppr++ : ppr--;
}

void BalanceBotEncoder::Update(const float samplingTime){
    float newAngle = 2 * PI * ((float)ppr / (float)PPR);
    speed = (newAngle - angle) / samplingTime;

    angle = newAngle;
}

// reset funciton
void BalanceBotEncoder::Reset(){
    ppr = 0;
    angle = 0;
    speed = 0;
}
