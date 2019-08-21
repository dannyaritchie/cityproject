#ifndef PLAYER_H_
#define PLAYER_H_
#include"frameobjects.h"
#include <ostream>
class Player : public FrameObjects{
	private:
		int home;
		Player* closestPlay;
		double closestDist;
		//static int maxAcceleration;
	//	static int maxDeceleration;
	//	static int maxVelocity;
	public:
		Player(int, double, double, int);
		void infoToText(std::ostream & os, int type);
		int getTeam();
		int getNum();
		int getPid();
		void setClosestPlay(Player*);
		void setClosestDist(double);
		Player* getClosestPlay();
		const double& getClosestDist();
};
#endif
