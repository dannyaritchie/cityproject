#include "frameobjects.h"
void FrameObjects::setVelocity(std::array<double,2> tvel) {
	velocity = tvel;
}
void FrameObjects::setVelBool() {
	velocityBool = true;
}
const std::array<double,2> FrameObjects::getPos() {
	std::array<double,2> pos;
	pos[0] = x;
	pos[1] = y;
	return pos;
}
FrameObjects::~FrameObjects(){
	std::cout << "why" << std::endl;
}

