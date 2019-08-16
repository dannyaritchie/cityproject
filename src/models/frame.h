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
	double anglePotential[720] = {0};
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
    //computes a value for every 0.5 degrees around ball and stores in
    //anglepotential
    //value is sum of gaussians for every ally player i centred on theta i with
    //width ri
    bool writeScalar(std::ostream & os);
    //write team id on line one followed by anglepotential[i] on line i+1

};
#endif
    
