#ifndef FRAME_H_
#define FRAME_H_

#include <vector>
#include<iostream>
#include "player.h"
#include "ball.h"
class Frame 
{
private:
	std::vector<Player*> players;
	Ball* ball;
	int fid;
	bool firstFrame;
	int attacking;
public:
	Frame();
	~Frame();

	void addPlayer(Player*);
	void addBall(Ball* pball);
	void objectInfo(std::ostream & os,int type);
	void addFid(int pfid);
	void setFirstFrame();
	bool getFirstFrame();
	int getFid();
	Ball* getBall();
	const std::vector<Player*>& getPlayers();
	Player* findNum(int num, int team);//returns pointer to player with number
	void evalSquares(int xsquare,int ysquare);
	//split pitch into xsquare*ysquare
	//for each square
	void closestPlayers();
	//for each player find distance to closest opposition player
	double getVoronoi(double pitchx,double pitchy);
	//***ANGULAR PASSING***///
	void computeScalar(int possessionTid);

};
#endif
    
