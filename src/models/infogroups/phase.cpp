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
	while(frames<pframeLength-175){
		FrameMetric tempMetric;
		//get five closest attackersto goal
		//set cumalitve marked
		std::vector<PlayerWithDist> closestFivePlayerWith;
		int marked = 0;
		std::vector<Player*> players = (*pframe)->getPlayers();
		closestPlayers(players);
		tempMetric.marked = 0;
		for(auto playerit = players.begin();playerit<players.end();++playerit){
			std::cout<<(*playerit)->getTeam() << ":" <<phome <<std::endl;
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
		tempMetric.cumClosestFive = 0;
		for(auto cplayerit = closestFivePlayerWith.begin(); cplayerit < closestFivePlayerWith.end();++cplayerit){
			Player* tplayer = (*cplayerit).player;
			double dist = tplayer->getClosestDist();
			std::cout <<"N"<< dist <<std::endl;
			tempMetric.cumClosestFive+=dist;
		}
		tempMetric.ballGoalDist =	distance((*pframe)->getBall()->getPos()[0],goalx,(*pframe)->getBall()->getPos()[1],goaly);
		std::vector<Frame*>::iterator pv = std::next(pframe, 15);
		Ball* ball= (*pv)->getBall();
		int u = ball->getPos()[0];
		tempMetric.forwardBallGoalDist = distance((*pv)->getBall()->getPos()[0],goalx,(*pv)->getBall()->getPos()[1],goaly)-tempMetric.ballGoalDist;
		tempMetric.ballGoalDist =	distance((*pframe)->getBall()->getPos()[0],goalx,(*pframe)->getBall()->getPos()[1],goaly);
		tempMetric.timeToEnd = pframeLength- frames;
		if(tempMetric.ballGoalDist<40){
		frameMetrics.push_back(tempMetric);
		}
		frames=frames+5;
		++pframe;
	}
}

void Phase::writeCumClosest(std::ostream & os){
	for (auto it = frameMetrics.begin();it<frameMetrics.end();++it){
		os << (*it).marked <<"\t"<< (*it).forwardBallGoalDist << "\t" <<(*it).cumClosestFive << std::endl;
	}
}
