#ifndef BALANCEBOTSTATEFEEDBACKCONTROLLER_H
#define BALANCEBOTSTATEFEEDBACKCONTROLLER_H

class BalanceBotStateFeedbackController{
  private:
	float k1;
	float k2;
	float k3;
    float k4;
  	
  public:
    // constructor
    BalanceBotStateFeedbackController();
    
    // set function
    void SetK(float k1, float k2, float k3, float k4);
  	
    // update function
    float Update(float psi, float psid, float theta, float thetad);
};

#endif //BALANCEBOTSTATEFEEDBACKCONTROLLER_H