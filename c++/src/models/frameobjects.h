//abstract parent class for all frame objects
#ifndef FRAMEOBJECTS_H_
#define FRAMEOBJECTS_H_
#include <string>
#include <iostream>
#include <array>
class FrameObjects{
	protected:
		double x, y;
		int num;
		std::array<double,2> velocity;
		bool velocityBool;
	public:
		virtual ~FrameObjects();
		void setVelocity(std::array<double,2> tvel);
		void setVelBool();
		double getVelocity();
		std::array<double,2> getVelocityVector();
		const std::array<double,2> getPos();
};
#endif
