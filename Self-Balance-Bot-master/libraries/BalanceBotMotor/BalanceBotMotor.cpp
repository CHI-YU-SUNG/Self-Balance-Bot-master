#include "BalanceBotMotor.h"

// constructor
BalanceBotMotor::BalanceBotMotor(){}

// set function
void BalanceBotMotor::SetPin(const int pwmPin,
                             const int directionPinA,
                             const int directionPinB,
                             const int standbyPin){
    this->pwmPin = pwmPin;
    pinMode(pwmPin, OUTPUT);
    
    this->directionPinA = directionPinA;
	this->directionPinB = directionPinB;
	pinMode(this->directionPinA, OUTPUT);
	pinMode(this->directionPinB, OUTPUT);
	
	this->standbyPin = standbyPin;
    pinMode(this->standbyPin, OUTPUT);
}

// others
void BalanceBotMotor::Rotate(float voltage){
    // direction: -1-> clockwise, 1-> counter-clockwise
    boolean pin1 = LOW;   //initial rotate direction: clockwise
    boolean pin2 = HIGH;
    if(voltage < 0){
        pin1 = HIGH;
        pin2 = LOW;
        voltage = -voltage;
    }
    
    digitalWrite(standbyPin, HIGH);
    digitalWrite(directionPinA, pin1);
    digitalWrite(directionPinB, pin2);
    
    // pwm: 25~255
    analogWrite(pwmPin, (int)(voltage/12.0*230.0 + 25) );
}
