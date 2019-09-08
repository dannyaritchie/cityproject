#include "allclosest.h"
#include "../possessionfilereader/possessionfilemaker.h"
#include "../../helpers/functions.h"
#include <string>
#include <fstream>
#include <iostream>
#include "../idmap.h"
std::array<double,2> closeplayer::pullPair(int pid){
	std::array<double,2> temp = trolley.passPair(pid);
	tempInfo.framePlayerPressures.push_back(temp); 	
	return temp;
}
std::array<double,2> closeplayer::pullWritePassPair(int pid){
	std::array<double,2> temp = trolley.passPair(pid);
	playerPressures << temp[0] << "\t" << temp[1] << ",\t";
	return temp;
}
void closeplayer::writePair(std::array<double,2> temp){
	playerPressures << temp[0] << "\t" << temp[1] << ",\t";
}
void findiftro::addDist(double tdistance,int pid){
	if(closest[pid].size()==3){
		closest[pid].erase(closest[pid].begin());
 	}
	closest[pid].push_back(tdistance);
}
std::array<double,2> findiftro::passPair(int pid){
	std::array<double,2> temp;
	if(consecutive[pid] > 2){
		temp ={closest[pid][1], closest[pid][0]-closest[pid][2]};
	}else{temp = {closest[pid][1],0};}
	return temp;
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
	for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
		(*allPlayerit).playerPressures << previousFid<<"\t" << prevAttackingTeam << ":\t";	
	}
	for (auto playerit = homePlayers.begin() ; playerit < homePlayers.end();++playerit){
		int pid = (*playerit)->getMappedPid();
		double closestDist = 15;
		for (auto playeritb = awayPlayers.begin();playeritb < awayPlayers.end();++playeritb){
			double tdistance = distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
			if(tdistance<closestDist){
				int pidb = (*playeritb)->getMappedPid();
				allPlayers[pid].trolley.addDist(tdistance, pidb);
				if(consec){
					allPlayers[pid].trolley.consecutive[pidb] +=1;
				}else{allPlayers[pid].trolley.consecutive[pidb] = 0;}
				allPlayers[pidb].writePair(allPlayers[pid].pullWritePassPair(pidb));
			}
		}
	}
	for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
		(*allPlayerit).playerPressures << std::endl;
	}
}
void AllClosest::openStreams(int mid, Idmap idmapd){
	for(auto playerit = allPlayers.begin();playerit<allPlayers.end();++playerit){
		int playerid = playerit - allPlayers.begin();
		std::string pid = idmapd.getid(playerid);
		std::string filename = "../data/allplayerdistances/" + std::to_string(mid) + "_" + pid + ".txt";
		(*playerit).playerPressures.open(filename);
	}
}
void AllClosest::closeStreams(){
	for(auto it = allPlayers.begin();it< allPlayers.end();++it){
		(*it).playerPressures.close();
	}
}
