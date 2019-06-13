#ifndef BALANCEBOTMOTOR_H
#define BALANCEBOTMOTOR_H

#include <Arduino.h>

class BalanceBotMotor{
  private:
    // pin
    int pwmPin;
    int directionPinA;
    int directionPinB;
    int standbyPin;


  public:
    // constructor
    BalanceBotMotor();

    // set function
    void SetPin(const int pwmPin,
                const int directionPinA,
                const int directionPinB,
                const int standbyPin);

    // others
    void Rotate(float voltage);
};

#endif /* BALANCEBOTMOTOR_H */
