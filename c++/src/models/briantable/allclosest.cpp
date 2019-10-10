#include "allclosest.h"
#include "../possessionfilereader/possessionfilemaker.h"
#include "../../helpers/functions.h"
#include <string>
#include <fstream>
#include <iostream>
#include "../idmap.h"
#include <iomanip>
#include <cmath>
	void findiftro::addInfo(std::array<double,3> info,int pid){
		if(closest[pid].size()==3){
			closest[pid].erase(closest[pid].begin());
		}
		closest[pid].push_back(info);
	}
	std::array<double,4> findiftro::passPair(int pid){
		std::array<double,4> temp;
		if(consecutive[pid] > 2){
			temp ={closest[pid][1][0], (closest[pid][0][0]-closest[pid][2][0])/0.4,0,0};
		}else{temp = {closest[pid][1][0],0,0,0};}
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
		return {0,0,-1,0};
	}
}
std::array<double,4> closeplayer::pullPassInfo(int pid){
	if (trolley.closest[pid].size() > 2){
		std::array<double,4> temp = trolley.passPair(pid);
		return temp;
	}
	else{
		return {-1,-1,-1,-1};
	}
}
void closeplayer::writePair(std::array<double,4> temp){
	if (temp[3] != -1){
	playerPressures << std::setprecision(0) << temp[2] << std::setprecision(2) <<"," << temp[0] << ","<< temp[1]<<"\t";
	}
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
void pressuresum::addPressures(std::array<double,4> temp){
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
void pressuresum::addPressurec(std::array<double,3> temp){
        double ballDist = temp[2];
	double ballMult;
        if (ballDist>0.1){
                ballMult = 1/ballDist;
        }else{ballMult = 10;}
	for(int i = 0;i<100;i++){
		if (temp[1] <= 0){
			if (temp[0] < 2){
				pressure[i] += 0.22*ballMult;
			}
		}
		else{
			pressure[i]+= pow(ballMult,0.45)*temp[1]/temp[0];
		}
	}
}
void pressuresum::addPressureGoalWeight(std::array<double,4> temp){
        double goalDist = temp[2];
	double goalMult;
        if (goalDist>0.1){
                goalMult = 1/goalDist;
        }else{goalMult = 10;}
	for(int i = 0;i<100;i++){
		if (temp[1] <= 0){
			if (temp[0] < 1.2){
				pressure[i] += 2.4*goalMult;
			}
		}
		else{
			pressure[i]+= pow(goalMult,0.02*i)*pow(temp[1],5.6)/temp[0];
		}
	}
}
void pressuresum::addPressureBallWeight(std::array<double,4> temp){
      double ballDist = temp[3];
	double ballMult;
        if (ballDist>0.1){
                ballMult = 1/ballDist;
        }else{ballMult = 10;}
	for(int i = 0;i<100;i++){
		if (temp[1] <= 0){
			if (temp[0] < 1.2){
				pressure[i] += (2.4)*ballMult;
			}
		}
		else{
			pressure[i]+= pow(ballMult,4)*pow(temp[1],2)/temp[0];
		}
	}
}
void pressuresum::addPressureBallGoalWeight(std::array<double,4> temp){
        double goalDist = temp[2];
	double ballDist = temp[3];
	double goalMult;
	double ballMult;
	std::array<double,4> notgg = {-1,-1,-1,-1};
	if (temp!=notgg){
		if (goalDist>0.1){
			goalMult = 1/goalDist;
		}else{goalMult = 10;}
		if (ballDist>0.1){
			ballMult = 1/ballDist;
		}else{ballMult = 10;}
		for(int i = 0;i<100;i++){
			if (temp[0] <= 1.2){
				pressure[i] += goalMult*ballMult*2.4;
			}
			else{
				if(temp[1]>0){
					pressure[i]+= pow(goalMult,0)*pow(ballMult,0)*pow(temp[1],1)/temp[0];
				}
			}
		}
	}
}

AllClosest::AllClosest(int pdist, Idmap pidmap, int playerSize, char phomeSide): mappedIds{pidmap}, distanceThreshold{pdist}, homeSide{phomeSide} {
	allPlayers.resize(playerSize);
}
std::array<double,100> AllClosest::addPlayers(std::vector<Frame*>::iterator frameit, int previousFid,int prevAttackingTeam){
//***
//a member function to add player distances below a set threshhold to a container of 2-arrays
//also creates a differential class
	bool consec;
	pressuresum pressureCalc;
	if((*frameit)->getFid()==previousFid + 1){
		consec = true;
	}else{consec== false;}
	(*frameit)->getPlayersSplit(homePlayers, awayPlayers);
	/*if(previousFid!=-1){
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
	}*/
	for (auto playerit = homePlayers.begin() ; playerit < homePlayers.end();++playerit){
		int pid = (*playerit)->getMappedPid();
		for (auto playeritb = awayPlayers.begin();playeritb < awayPlayers.end();++playeritb){
			double tdistance = 0.01* distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
			int pidb = (*playeritb)->getMappedPid();
			if(tdistance<distanceThreshold){
				double avel, bvel, anum, bnum;
	//			anum = (*playerit)->getNum();
	//			bnum = (*playeritb)->getNum();
	//			std::cout << anum << ":" << bnum <<std::endl;
			//	std::array<double,2> info = {tdistance,getBallDist(frameit,(*playerit)->getPos())};
				std::array<double,3> info = {tdistance,getGoalDist((*playerit)->getPos(),prevAttackingTeam), getBallDist(frameit,(*playerit)->getPos())};
				allPlayers[pid].trolley.addInfo(info, pidb);
				if(consec){
					allPlayers[pid].trolley.consecutive[pidb] +=1;
				}else{allPlayers[pid].trolley.consecutive[pidb] = 0;}
				pressureCalc.addPressureBallGoalWeight(allPlayers[pid].pullPassInfo(pidb));
			}
			else{allPlayers[pid].trolley.consecutive[pidb] = 0;}
		}
	}
//	if(previousFid!=-1){
//		for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
//			(*allPlayerit).playerPressures << std::endl;
//		}
//	}
	return pressureCalc.pressure;
}
double AllClosest::getBallDist(std::vector<Frame*>::iterator frameit, std::array<double,2> playerPos){
	Ball * tball = (*frameit)->getBall();
	return 0.01*distance(tball->getPos()[0],playerPos[0],tball->getPos()[1],playerPos[1]);
}
double AllClosest::getGoalDist(std::array<double,2> playerPos, int attacking){
	if(attacking == 0){
		if(homeSide == 'L'){
		return 0.01*distance(5250,playerPos[0],0,playerPos[1]);
		}
		else{
		return 0.01*distance(-5250,playerPos[0],0,playerPos[1]);
		}
	}else{
		if(homeSide == 'L'){
		return 0.01*distance(-5250,playerPos[0],0,playerPos[1]);
		}else{
		return 0.01*distance(5250,playerPos[0],0,playerPos[1]);
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
