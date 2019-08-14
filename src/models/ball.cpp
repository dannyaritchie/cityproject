#include "ball.h"

Ball::Ball(double px, double py, double pz) : z(pz) {
	x = px;
	y = py;
	num = 0;
}
void Ball::infoToText(std::ostream & os,int type) {
	//sets data and returns reference to it in format b,$x,$y,$z,
	switch(type){
	case 0:
		os << "\t" << num<< "\t" << x << "\t" <<  y << "\n";
		break;
	case 1:
		os << "\t" << num<< "\t" << velocity[0] << "\t" <<  velocity[1] << "\n";
	}

}

