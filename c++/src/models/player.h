#ifndef PLAYER_H_
#define PLAYER_H_
#include"frameobjects.h"
#include <ostream>
struct playerplayerinfo {
	double distance;
	double velocity;
	playerplayerinfo():distance{0},velocity{0}{}
};
class Player : public FrameObjects{
	private:
		int home;
		double mnum;
		Player* closestPlay;
		double closestDist;
		std::array<double,2> ballCentredPolar;
		int angControlScore;
		double distanceInFrontOfBall;
		std::array<playerplayerinfo,28> playerInfo;
		//static int maxAcceleration;
	//	static int maxDeceleration;
	//	static int maxVelocity;
	public:
//		Player(int, double, double, int);
		Player(int, double, double, int, int);
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
		void setDistanceInFrontOfBall(double distance);
		double getDistanceInFrontOfBall();
		void setPlayerPlayerVelocity(double distance, int pid);
		double getPlayerPlayerVelocity(int pid);
		void setPlayerPlayerDistance(double distance, int pid);
		double getPlayerPlayerDistance(int pid);
		bool getShortestTime(double&time);
};
#endif
