#ifndef PHASE_H_
#define PHASE_H_
#include <array>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>
#include "../game.h"
#include "infogroupa.h"
struct FrameMetric {
	double ballGoalDist, forwardBallGoalDist;
	int marked;
	double cumClosestFive;
	double timeToEnd;
};
struct PlayerWithDist{
	double dist;
	Player* player;
};
class Phase {
	private:
		int home;
		int goalx,goaly;
		std::vector<FrameMetric> frameMetrics;
	public:
		Phase(int pframeLength, std::vector<Frame*>::iterator pframe,int phome, int ppitchx, int ppitchy);
		void writeCumClosest(std::ostream & os);		
};

#endif
