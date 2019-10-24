#ifndef BALL_H_
#define BALL_H_
#include "frameobjects.h"
#include <iostream>
class Ball : public FrameObjects{
	private:
		double z;
	public:
		Ball(double px,double py,double pz);
		void infoToText(std::ostream & os, int type);
};
#endif
