#include "phase.h"
#include "../../helpers/functions.h"
#include<algorithm>

	
Phase::Phase(int pframeLength, std::vector<Frame*>::iterator pframe, int phome, int pitchx, int pitchy){
	home = phome;
	if (home == 0){
		goalx = -pitchx/2;
	}
	else{goalx = pitchx/2;}
	goaly= pitchy/2;
	int frames = 0;
	while(frames<pframeLength-75){
		FrameMetric tempMetric;
		//get five closest attackersto goal
		//set cumalitve marked
		std::vector<PlayerWithDist> closestFivePlayerWith;
		int marked = 0;
		for(auto playerit = (*pframe)->getPlayers().begin();playerit<(*pframe)->getPlayers().end();++playerit){
			if((*playerit)->getTeam() == phome){
				double dtoG = distance((*playerit)->getPos()[0],goalx,(*playerit)->getPos()[1],goaly);
				if(closestFivePlayerWith.size()<5){
					PlayerWithDist temp;
					temp.dist = dtoG;
					temp.player = (*playerit);
					closestFivePlayerWith.push_back(temp);
				}
				else{
					if(dtoG<closestFivePlayerWith[0].dist){
						PlayerWithDist temp;
						temp.dist = dtoG;
						temp.player=(*playerit);
						closestFivePlayerWith.erase(closestFivePlayerWith.begin());
						closestFivePlayerWith.push_back(temp);
						std::sort(closestFivePlayerWith.begin(),closestFivePlayerWith.end(),[](const PlayerWithDist & a, const PlayerWithDist & b)
							{
								return a.dist<b.dist;
							});
					}
				}
			}
			else{
				tempMetric.marked+=(*playerit)->getAngScore();
			}
		}
		//set their cumilative closest playerdist				
		for(auto cplayerit = closestFivePlayerWith.begin(); cplayerit < closestFivePlayerWith.end();++cplayerit){
			tempMetric.cumClosestFive+=(*cplayerit).player->getClosestDist();
		}
		tempMetric.ballGoalDist =	distance((*pframe)->getBall()->getPos()[0],goalx,(*pframe)->getBall()->getPos()[1],goaly);
		auto pv = std::next(pframe, 15);
		tempMetric.forwardBallGoalDist = distance((*pv)->getBall()->getPos()[0],goalx,(*pv)->getBall()->getPos()[1],goaly)-tempMetric.ballGoalDist;
		frameMetrics.push_back(tempMetric);
		frames++;
		++pframe;
	}
}

void Phase::writeCumClosest(std::ostream & os){
	for (auto it = frameMetrics.begin();it<frameMetrics.end();++it){
		os << (*it).ballGoalDist <<"\t"<< (*it).forwardBallGoalDist << "\t" <<(*it).cumClosestFive << std::endl;
	}
}
