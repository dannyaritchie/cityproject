#ifndef PLAYER_H_
#define PLAYER_H_
#include"frameobjects.h"
#include <ostream>
class Player : public FrameObjects{
	private:
		int home;
		Player* closestPlay;
		double closestDist;
		std::array<double,2> ballCentredPolar;
		int angControlScore;
		//static int maxAcceleration;
	//	static int maxDeceleration;
	//	static int maxVelocity;
	public:
		Player(int, double, double, int);
		void infoToText(std::ostream & os, int type);
		int getTeam();
		int getNum();
		int getMappedPid();
		void setClosestPlay(Player*);
		void setClosestDist(double);
		Player* getClosestPlay();
		const double& getClosestDist();
		void setBallCentredPolar(std::array<double,2> tpol);
		std::array<double,2> getBallCentredPolar();
		void plusAngScore();
		int getAngScore();
};
#endif
