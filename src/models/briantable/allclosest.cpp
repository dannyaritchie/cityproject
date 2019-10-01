#include "allclosest.h"
#include "../possessionfilereader/possessionfilemaker.h"
#include "../../helpers/functions.h"
#include <string>
#include <fstream>
#include <iostream>
#include "../idmap.h"
#include <iomanip>
void findiftro::addInfo(std::array<double,3> info,int pid){
	if(closest[pid].size()==3){
		closest[pid].erase(closest[pid].begin());
 	}
	closest[pid].push_back(info);
}
std::array<double,4> findiftro::passPair(int pid){
	std::array<double,4> temp;
	if(consecutive[pid] > 2){
		temp ={closest[pid][1][0], (closest[pid][0][0]-closest[pid][2][0])/0.4};
	}else{temp = {closest[pid][1][0],0};}
	for(int i =1;i<3;i++){
		temp[i+1] = closest[pid][1][i];
	}
	return temp;
}

std::array<double,4> closeplayer::pullPair(int pid){
	std::array<double,4> temp = trolley.passPair(pid);
	tempInfo.framePlayerPressures.push_back(temp); 	
	return temp;
}
std::array<double,4> closeplayer::pullWritePassPair(int pid){
	if (trolley.closest[pid].size() > 2){
		std::array<double,4> temp = trolley.passPair(pid);
		playerPressures << std::setprecision(0) <<temp[3] << std::setprecision(2) <<"," << temp[0] << ","<< temp[1] << "\t";
		return temp;
	}
	else{
		return {0,0,0,-1};
	}
}
void closeplayer::writePair(std::array<double,4> temp){
	if (temp[3] != -1){
	playerPressures << std::setprecision(0) << temp[2] << std::setprecision(2) <<"," << temp[0] << ","<< temp[1]<<"\t";
	}
}

AllClosest::AllClosest(int pdist, Idmap pidmap, int playerSize): mappedIds{pidmap}, distanceThreshold{pdist} {
	allPlayers.resize(playerSize);
}
void AllClosest::addPlayers(std::vector<Frame*>::iterator frameit, int previousFid,int prevAttackingTeam){
//***
//a member function to add player distances below a set threshhold to a container of 2-arrays
//also creates a differential class
	bool consec;
	if((*frameit)->getFid()==previousFid + 5){
		consec = true;
	}else{consec== false;}
	(*frameit)->getPlayersSplit(homePlayers, awayPlayers);
	if(previousFid!=-1){
		for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
			(*allPlayerit).playerPressures << previousFid<<"\t" << prevAttackingTeam << "\t";	
		}
		for (auto playerit = homePlayers.begin(); playerit<homePlayers.end();++playerit){
			double vel = (*playerit)->getVelocity();
			double pid = (*playerit)->getMappedPid();
			allPlayers[pid].playerPressures << std::setprecision(2) << vel << ":\t";
		}
		for (auto playerit = awayPlayers.begin(); playerit<awayPlayers.end();++playerit){
			double vel = (*playerit)->getVelocity();
			double pid = (*playerit)->getMappedPid();
			allPlayers[pid].playerPressures << std::setprecision(2)<<vel << ":\t";
		}
	}
	for (auto playerit = homePlayers.begin() ; playerit < homePlayers.end();++playerit){
		int pid = (*playerit)->getMappedPid();
		for (auto playeritb = awayPlayers.begin();playeritb < awayPlayers.end();++playeritb){
			double tdistance = distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
			int pidb = (*playeritb)->getMappedPid();
			if(tdistance<distanceThreshold){
				double avel, bvel, anum, bnum;
				anum = (*playerit)->getNum();
				bnum = (*playeritb)->getNum();
				std::array<double,3> info = {tdistance,anum,bnum};
				allPlayers[pid].trolley.addInfo(info, pidb);
				if(consec){
					allPlayers[pid].trolley.consecutive[pidb] +=1;
				}else{allPlayers[pid].trolley.consecutive[pidb] = 0;}
				allPlayers[pidb].writePair(allPlayers[pid].pullWritePassPair(pidb));
			}
			else{allPlayers[pid].trolley.consecutive[pidb] = 0;}
		}
	}
	if(previousFid!=-1){
		for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
			(*allPlayerit).playerPressures << std::endl;
		}
	}
}
void AllClosest::openStreams(int mid, Idmap idmapd){
	for(auto playerit = allPlayers.begin();playerit<allPlayers.end();++playerit){
		int playerid = playerit - allPlayers.begin();
		std::string pid = idmapd.getid(playerid);
		std::string filename = "../data/allplayerdistances/" + std::to_string(mid) + "_" + pid + ".txt";
		(*playerit).playerPressures.open(filename);
		(*playerit).playerPressures << std::fixed;
		(*playerit).playerPressures << "Frame ID\tTeam in possession\tPlayer's velocity:\t(An opposition player number, Distance to this player, Rate of change of this distance)" << std::endl;
	}
}
void AllClosest::closeStreams(){
	for(auto it = allPlayers.begin();it< allPlayers.end();++it){
		(*it).playerPressures.close();
	}
}
