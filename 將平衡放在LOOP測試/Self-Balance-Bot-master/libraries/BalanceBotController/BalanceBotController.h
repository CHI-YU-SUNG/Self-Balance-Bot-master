#ifndef BALANCEBOTCONTROLLER_H
#define BALANCEBOTCONTROLLER_H

#include <Arduino.h>
#include <math.h>

class BalanceBotController{
  private:
    float MAX_OUTPUT;
    float MIN_OUTPUT;
    float toleratedError;
	float Kp, Ki, Kd;  // PID control
    float reference;   // desire output
    float integral;    // record the sum of previous error
    float preError;    // previous error
    bool steady;       // check the system is steady or not


  public:
    // constructor
    BalanceBotController();

    // set function
    void SetSaturation(const float max, const float min);
    void SetToleratedError(const float error);
    void SetPID(const float kp, const float ki, const float kd);
    void SetReference(const float reference);

    // get function
    float GetReference();
    String GetInformation();
    String GetErrorIntegral();

    // update
    float Update(const float feedback, const float dt);

    // chech
    bool IsSteady();
    
    // clear
    void ClearIntegral();
};

#endif //BALANCEBOTCONTROLLER_H
