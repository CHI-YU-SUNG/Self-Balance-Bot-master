#include "BalanceBotStateFeedbackController.h"

// constructor
BalanceBotStateFeedbackController::BalanceBotStateFeedbackController(){
	k1 = 0.0;
	k2 = 0.0;
	k3 = 0.0;
	k4 = 0.0;
}

// set function
void BalanceBotStateFeedbackController::SetK(float k1, float k2, float k3, float k4){
	this->k1 = k1;
	this->k2 = k2;
	this->k3 = k3;
	this->k4 = k4;
}

// update function
float BalanceBotStateFeedbackController::Update(float psi, float psid, float theta, float thetad){
	return (k1 * psi + k2 * psid + k3 * theta + k4 * thetad);
}