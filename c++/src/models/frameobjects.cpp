#include "frameobjects.h"
#include <cmath>
void FrameObjects::setVelocity(std::array<double,2> tvel) {
	velocity = tvel;
}
void FrameObjects::setVelBool() {
	velocityBool = true;
}
double FrameObjects::getVelocity(){
	return pow(pow(velocity[0],2)+pow(velocity[1],2),0.5);
}
std::array<double,2> FrameObjects::getVelocityVector(){
	return velocity;
}
const std::array<double,2> FrameObjects::getPos() {
	std::array<double,2> pos;
	pos[0] = x;
	pos[1] = y;
	return pos;
}

FrameObjects::~FrameObjects(){}
