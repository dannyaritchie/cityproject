#include "allclosest.h"
#include "../possessionfilereader/possessionfilemaker.h"
#include "../../helpers/functions.h"
#include <string>
#include <fstream>
#include <iostream>
#include "../idmap.h"
#include <iomanip>

void findiftro::addDist(double tdistance,double tballdist, int pid){
	if(closest[pid].size()==3){
		closest[pid].erase(closest[pid].begin());
 	}
	std::array<double,2> temp;
	temp[0] = tdistance;
	temp[1] = tballdist;
	closest[pid].push_back(temp);
}
std::array<double,2> findiftro::passPair(int pid){
	std::array<double,2> temp;
	if(consecutive[pid] > 2){
		temp ={closest[pid][1][0], (closest[pid][0][0]-closest[pid][2][0])/0.4};
	}else{temp = {closest[pid][1][0],0};}
	return temp;
}
double findiftro::passBallDist(int pid){
	return closest[pid][1][1];
}

std::array<double,2> closeplayer::pullPair(int pid){
	std::array<double,2> temp = trolley.passPair(pid);
	tempInfo.framePlayerPressures.push_back(temp); 	
	return temp;
}
std::array<double,2> closeplayer::pullWritePassPair(int pid){
	std::array<double,2> temp = trolley.passPair(pid);
//	playerPressures << temp[0] << " " << temp[1] << ",\t";
	return temp;
}
double closeplayer::pullWritePassBallDist(int pid){
	return trolley.passBallDist(pid);
}
void closeplayer::writePair(std::array<double,2> temp){
	playerPressures << temp[0] << " " << temp[1] << ",\t";
}
void closeplayer::passPair(std::array<double,2> temp){
	tempInfo.framePlayerPressures.push_back(temp);
}

pressuresum::pressuresum(){
	for(int i = 0;i<100;i++){
		pressure[i] = 0;
	}
}
void pressuresum::addPressurea(std::array<double,2> temp){
	if (temp[1] > 0){
		pressure[0] += temp[1]/temp[0];
	}
}
void pressuresum::addPressures(std::array<double,2> temp){
	if (temp[1] <= 0){
		if(temp[0] < 2){
			pressure[0]+=0.5;
		}
	}
	else{
		pressure[0]+=temp[1]/temp[0];
	}
}
void pressuresum::addPressureb(std::array<double,2> temp){
	if (temp[1] <= 0){
		if (temp[0] < 2){
			for(int i = 0; i<100; i++){
				pressure[i] += i*0.02;
			}
		}
	}
	else{
		for(int i = 0; i<100; i++){
			pressure[i] += temp[1]/temp[0];
		}
	}
}
void pressuresum::addPressurec(std::array<double,2> temp, double ballDist){
	double ballMult = ballDist;
	if (ballDist>0.01){
		ballMult = 1/ballDist;
	}else{ballMult = 100;}
	if (temp[1] <= 0){
		if (temp[0] < 2){
			pressure[0] += 1.5*ballMult;
		}
	}
	else{
		pressure[0] += ballMult*temp[1]/temp[0];
	}
}
void pressuresum::addPressured(std::array<double,2> temp){
	if (temp[0] < 2){
		pressure[0] += 1.5;
	}
	else{
		if(temp[1] >= 0){
			pressure[0] += temp[1]/temp[0];
		}
	}
}

AllClosest::AllClosest(int pdist, int playerSize): distanceThreshold{pdist}{
	allPlayers.resize(playerSize);
}
AllClosest::AllClosest(int pdist, int playerSize, double pitchx, double pitchy): distanceThreshold{pdist}{
	allPlayers.resize(playerSize);
	goaly = pitchy/2;
	agoalx = pitchx/2;
	hgoalx = -pitchx/2;
	std::cout << agoalx << "<" << hgoalx << "<" << goaly << std::endl;
}
std::array<double, 100> AllClosest::addPlayers(std::vector<Frame*>::iterator frameit, int previousFid,int prevAttackingTeam){
//***
//a member function to add player distances below a set threshhold to a container of 2-arrays
//also creates a differential class
	bool consec;
	pressuresum pressureCalc;
	if((*frameit)->getFid()==previousFid + 5){
		consec = true;
	}else{consec== false;}
	(*frameit)->getPlayersSplit(homePlayers, awayPlayers);
	Ball * ball = (*frameit)->getBall();
//	for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
//		(*allPlayerit).playerPressures << previousFid<<"\t|" << prevAttackingTeam << "\t|";	
//	}
	for (auto playerit = homePlayers.begin() ; playerit < homePlayers.end();++playerit){
		int pid = (*playerit)->getMappedPid();
		for (auto playeritb = awayPlayers.begin();playeritb < awayPlayers.end();++playeritb){
			double tdistance = distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
			int pidb = (*playeritb)->getMappedPid();
			if(tdistance<distanceThreshold){
				//find distancebetween attacking player and ball
				int attackingPid;
				Player * attackingPlayer;
				double goalx;
				if (prevAttackingTeam == 0){
					attackingPid = pid;
					attackingPlayer = (*playerit);
					goalx = hgoalx;
				}
				else{
					attackingPid = pidb;
					attackingPlayer = (*playeritb);
					goalx = agoalx;
				}
			//distancetoball	double distToBall = distance(attackingPlayer->getPos()[0],,attackingPlayer->getPos()[1],ball->getPos()[1]);
			//distancetogoal
				double distToBall = distance(attackingPlayer->getPos()[0],goalx,attackingPlayer->getPos()[1],goaly);
				allPlayers[pid].trolley.addDist(tdistance,distToBall, pidb);
				if(consec){
					allPlayers[pid].trolley.consecutive[pidb] +=1;
				}else{allPlayers[pid].trolley.consecutive[pidb] = 0;}
			//	allPlayers[pidb].writePair(allPlayers[pid].pullWritePassPair(pidb));
				pressureCalc.addPressures(allPlayers[pid].pullWritePassPair(pidb));	
			}
			else{allPlayers[pid].trolley.consecutive[pidb] = 0;}
		}
	}
	return pressureCalc.pressure;
//	for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
//		(*allPlayerit).playerPressures << std::endl;
//	}
}
void AllClosest::openStreams(int mid, Idmap idmapd){
	for(auto playerit = allPlayers.begin();playerit<allPlayers.end();++playerit){
		int playerid = playerit - allPlayers.begin();
		std::string pid = idmapd.getid(playerid);
		std::string filename = "../data/allplayerdistances/" + std::to_string(mid) + "_" + pid + ".txt";
		(*playerit).playerPressures.open(filename);
		(*playerit).playerPressures << std::setprecision(2)<<std::fixed;
		(*playerit).playerPressures << "FrameID" << "\t|" << "PosID" << "\t|" << "Distance_to_playeri Rate_of_change, ..." << std::endl;
	}
}
void AllClosest::closeStreams(){
	for(auto it = allPlayers.begin();it< allPlayers.end();++it){
		(*it).playerPressures.close();
	}
}
