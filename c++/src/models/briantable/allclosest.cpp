#include "allclosest.h"
#include "../possessionfilereader/possessionfilemaker.h"
#include "../../helpers/functions.h"
#include <string>
#include <fstream>
#include <iostream>
#include "../idmap.h"
#include <iomanip>
#include <cmath>
#include <algorithm>
void findiftro::addInfo(std::array<double,6> info,int pid){
	std::array<double,7> temp;
	for(int i = 0;i<6;i++){
		temp[i] = info[i];
	}
	temp[6] = 0;
	if(closest[pid].size()==3){
		closest[pid].erase(closest[pid].begin());
	}
	closest[pid].push_back(temp);
}
std::array<double,8> findiftro::passPair(int pidv, int dead){
	std::array<double,8> temp;
	if(consecutive[pidv][1] == 0){
		temp = {-1,-1,-1,-1,-1,-1,-1,-1};
	}
	else{
		if(consecutive[pidv][0] == 0||consecutive[pidv][2] == 0){
			temp = {closest[pidv][1][0],0,0,0,0,0,0,0};
		}
		else{
			if(dead > 2){
				temp ={closest[pidv][1][0], (closest[pidv][0][0]-closest[pidv][2][0])/0.4,0,0,0,0,0,0};
			}
			else{
				temp = {closest[pidv][1][0],0,0,0,0,0,0,0};
			}
		}
		for(int i =1;i<7;i++){
			temp[i+1] = closest[pidv][1][i];
		}
	}
	return temp;
}
void AllClosest::addDWeights(int z){
	std::array<std::vector<std::array<double,2>>,28> sorter;
	for(auto homeit  = attackPlayers.begin();homeit<attackPlayers.end();++homeit){
		double homepid = (*homeit)->getMappedPid();
		double awayIndex = 0;
		std::array<std::vector<int>,28>::iterator consecMarker = allPlayers[homepid].trolley.consecutive.begin();
		for(auto awayit = allPlayers[homepid].trolley.closest.begin();awayit<allPlayers[homepid].trolley.closest.end();++awayit){
			if((*consecMarker)[(*consecMarker).size() - 1]==1){
				double d = (*awayit)[(*awayit).size()-1][0];
				std::array<double,2> temp  = {homepid,d};
				sorter[awayIndex].push_back(temp);
			}
			++consecMarker;
			awayIndex++;
		}
	}
	for(int i = 0;i<28;i++){
		std::sort(sorter[i].begin(),sorter[i].end(), [](const std::array<double,2> &a,const std::array<double,2> &b)
		{
				return a[1] < b[1];
		});
	}
	int u = 0;
	for(auto it  = attackPlayers.begin();it<attackPlayers.end();++it){
		int homepid = (*it)->getMappedPid();
		for(int awayit = 0;awayit<allPlayers[homepid].trolley.closest.size();++awayit){
			for(int distanceRank = 0;distanceRank<sorter[awayit].size();distanceRank++){
				if( sorter[awayit][distanceRank][0] == homepid){
				       allPlayers[homepid].trolley.closest[awayit][allPlayers[homepid].trolley.closest[awayit].size()-1][6] = distanceRank;	
				}
			}
		}
	}
}
void findiftro::addWeights(int z){
	std::vector<std::array<double,2>> sorter;
	double i = 0;
	std::array<std::vector<int>,28>::iterator itb = consecutive.begin();
	for(auto ita = closest.begin();ita<closest.end();++ita){
		if(consecutive[i][(*itb).size() - 1]==1){
			double d = (*ita)[(*ita).size()-1][0];
			std::array<double,2> temp  = {i,d};
			sorter.push_back(temp);
		}
		++itb;
		i++;
	}
	std::sort(sorter.begin(),sorter.end(), [](const std::array<double,2> &a,const std::array<double,2> &b)
	{
			return a[1] < b[1];
	});
	int u = 0;
	for(auto it = sorter.begin();it<sorter.end();it++){	
		double index = (*it)[0];
		closest[index][closest[index].size()-1][6] = u;
		u++;
	}
}
/*
void findiftro::sortBall(){
	std::vector<std::array<double,2>> sorter;
	double i = 0;
	std::array<int,28>::iterator itb = consecutive.begin();
	for(auto ita = closest.begin();ita<closest.end();++ita){
		if(*itb > 0){
			double d = (*ita)[(*ita).size()-1][2];
			std::array<double,2> temp  = {i,d};
			sorter.push_back(temp);
		}
		++itb;
		i++;
	}
	std::sort(sorter.begin(),sorter.end(), [](const std::array<double,2> &a,const std::array<double,2> &b)
	{
		return a[1] < a[2];
	});
	int u = 0;
	for(auto it = sorter.begin();it<sorter.end();it++){	
		double index = (*it)[0];
		closest[index][closest[index].size()-1][4] = u;
		u++;
	}
}*/
void findiftro::addConsec(int i, int pid){
	consecutive[pid].push_back(i);
	consecutive[pid].erase(consecutive[pid].begin());
}
		

std::array<double,8> closeplayer::pullPair(int pid){
	std::array<double,8> temp = trolley.passPair(pid, true);
	tempInfo.framePlayerPressures.push_back(temp); 	
	return temp;
}
std::array<double,8> closeplayer::pullWritePassPair(int pid){
	if (trolley.closest[pid].size() > 2){
		std::array<double,8> temp = trolley.passPair(pid, true);
		playerPressures << std::setprecision(0) <<temp[3] << std::setprecision(2) <<"," << temp[0] << ","<< temp[1] << "\t";
		return temp;
	}
	else{
		return {-1,-1,-1,-1,-1,-1,-1,-1};
	}
}
std::array<double,8> closeplayer::pullPassInfo(int pid, int dead){
	if (trolley.closest[pid].size() > 2){
		std::array<double,8> temp = trolley.passPair(pid,dead);
		return temp;
	}
	else{
		return {-1,-1,-1,-1,-1,-1,-1,-1};
	}
}
void closeplayer::writePair(std::array<double,8> temp){
	if (temp[2] != -1){
	playerPressures << std::setprecision(0) << temp[2] << std::setprecision(2) <<"," << temp[0] << ","<< temp[1]<<"\t";
	}
}

pressuresum::pressuresum(double tgp,double tdp,double tdw,double tbp,double tvp,double tdvp,double tra,double tddg,double tdpv,double at, int tinterestingTeam):gp{tgp},dp{tdp},dw{tdw},bp{tbp},vp{tvp},dvp{tdvp},ra{tra},ddg{tddg}, dpv{tdpv}, angleThreshold{at}, interestingTeam{tinterestingTeam}{
        for(int i = 0;i<100;i++){
                pressure[i] = 0;
        }
}
void pressuresum::callPresures(std::vector<int> pressureTypes, std::array<double, 8> temp,int tip){
	for (auto it = pressureTypes.begin();it< pressureTypes.end();it++){
		switch(*it){
			case 0:
				addPressureA(temp, 0);
				break;
			case 1:
				addPressureB(temp, 10);
				break;
			case 2:
				addPressureC(temp, 20);
				break;
			case 3:
				addPressureD(temp, 30);
				break;
			case 6:
				addPressureA(temp, 40);
				break;
			case 7:
				addPressureA(temp, 50);
				break;
			case 8:
				addPressureU(temp, 60,tip);
				break;
		
		}
	}
}
void pressuresum::addPressureBallGoalWeight(std::array<double,8> temp){
        double goalDist = temp[2];
	double ballDist = temp[3];
	double goalMult;
	double ballMult;
	std::array<double,8> notgg = {-1,-1,-1,-1,-1,-1,-1,-1};
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
void pressuresum::addPressureA(std::array<double,8> temp, int pos){
        double goalDist = temp[2];
	double ballDist = temp[3];
	std::array<double,8> notgg = {-1,-1,-1,-1,-1,-1,-1,-1};
	if (temp!=notgg){
		if(temp[1] < 0){
			temp[1] = 0;
		}
		for(int i = 0;i<10;i++){
//		for(int j = 0;j<3;j++){
//		for(int k = 0;k<3;k++){
//		for(int l = 0;l<3;l++){

		//	if(temp[5]<7){
				pressure[pos+i] += pow(pow(goalDist,0) + 1,-1)*(i* pow( pow(10,0)*pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],4));

				//std::cout << temp[0] << ">" << temp[1] << ">" <<  temp[2] << ">" << temp[3] << std::endl;
	//		}
	//		else{
	//			pressure[pos+i] += 0.1*pow(pow(goalDist,0) + 1,-1)*(i* pow( pow(10,i)*pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],4));
		//	}
		}
	}

}
void pressuresum::addPressureU(std::array<double,8> temp, int pos,int tip){
	double goalRank;
/*	if (interestingTeam ==1||interestingTeam == 0){
		if (tip==interestingTeam){
			pressure[2] = -1;
			return;
		}
	}*/
	double vel,dist;
	if(temp[1] < 0.1){
		if (temp[1] >=-1){
			vel = 0.1;
		}
		else{
			if(dist<2){
				pressure[0] +=3;
			}
		}
	}
	else{
		vel = temp[1];
	}
	if(temp[0] < 0.1){
		dist =0.1;
	}
	else{dist = temp[0];}

	if(ra == 0){
		goalRank = 1;
	}
	else{goalRank = ra+temp[7];}
	//std::cout << temp[6] <<std::endl;
	std::array<double,8> notgg = {-1,-1,-1,-1,-1,-1,-1,-1};
	if (temp!=notgg){

//		if(temp[1] > 1&&temp[2]<30*3.1419/180){
		for(int i = 0;i<1;i++){
//pressure[0] += pow(pow(temp[2],ddg)+1,-1)*pow(temp[6],dpv)*pow(pow(temp[4],bp) + 1,-1)*(pow(10,dw)*pow( pow(temp[0],dp) +1, -1) + pow(pow(temp[0],dvp)+1,-1)*pow(temp[1],vp))/goalRank;//(1+temp[4]);

pressure[0] +=vel/dist;//(1+temp[4]);
		}
//		}
	}
}

void pressuresum::addPressureB(std::array<double,8> temp, int pos){
        double goalDist = temp[2];
	double ballDist = temp[3];
	std::array<double,8> notgg = {-1,-1,-1,-1,-1,-1,-1};
	if (temp!=notgg){
		if(temp[1] < 0){
			temp[1] = 0;
		}
		for(int i = 0;i<3;i++){
	//		if(temp[5]<7){
				pressure[pos+i] += pow(pow(goalDist,0) + 1,-1)*(pow( pow(10,i)*pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2));
	//		}
	//		else{
	//			pressure[pos+i] += 0.1*pow(pow(goalDist,0) + 1,-1)*(i* pow( pow(10,i)*pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2));
	//		}
		}
	}
}
void pressuresum::addPressureC(std::array<double,8> temp, int pos){
        double goalDist = temp[2];
	double ballDist = temp[3];
	std::array<double,8> notgg = {-1,-1,-1,-1,-1,-1,-1};
	if (temp!=notgg){
		if(temp[1] < 0){
			temp[1] = 0;
		}
		for(int i = 1;i<3;i++){
	//		if(temp[5]<7){
				pressure[pos+i] += pow(pow(goalDist,0) + 1,-1)*(pow(pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],i));
	//		}
	//		else{
	//			pressure[pos+i] += 0.1*pow(pow(goalDist,0) + 1,-1)*(i* pow( pow(10,i)*pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2));
	//		}
		}
	}
}
void pressuresum::addPressureD(std::array<double,8> temp, int pos){
        double goalDist = temp[2];
	double ballDist = temp[3];
	std::array<double,8> notgg = {-1,-1,-1,-1,-1,-1,-1};
	if (temp!=notgg){
		if(temp[1] < 0){
			temp[1] = 0;
		}
		pressure[pos+0] += pow(pow(goalDist,0) + 1,-1)*(pow(10,0)*pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],3));
		pressure[pos+1] += pow(pow(goalDist,0) + 1,-1)*(pow(10,0)*pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2));
		pressure[pos+2] += pow(pow(goalDist,0) + 1,-1)*(pow(10,0)*pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2))/(temp[4]+1);
		pressure[pos+3] += pow(pow(goalDist,0) + 1,-1)*(pow(10,0)*pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2))/(temp[4]+1);
		pressure[pos+4] += pow(pow(goalDist,0) + 1,-1)*(pow(10,0)*pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2))/(temp[4]+3);
		pressure[pos+5] += pow(pow(goalDist,0) + 1,-1)*(pow(10,0)*pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2))/(temp[4]+3);
	}
}
void pressuresum::addPressureZ(std::array<double,8> temp, int pos){
        double goalDist = temp[2];
	double ballDist = temp[3];
	std::array<double,8> notgg = {-1,-1,-1,-1,-1,-1,-1};
	if (temp!=notgg){
		if(temp[1] < 0){
			temp[1] = 0;
		}
		for(int i = 0;i<10;i++){
			if(temp[5]<i){
				pressure[pos+i] += pow(pow(goalDist,0) + 1,-1)*(1* pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2))/(i/4+temp[4]);
			}
			else{
				pressure[pos+i] += 0.1*pow(pow(goalDist,0) + 1,-1)*(1* pow( pow(temp[0],5) +1, -1) + pow(temp[0]+1,-1)*pow(temp[1],2))/(i/4+temp[4]);
			}
		}
	}
	else{pressure[pos] +=1;}
}



AllClosest::AllClosest(int pdist, Idmap pidmap, int playerSize, char phomeSide, double tgoal_pow,double tball_pow,double dist_weight,double tdist_pow,double tvel_pow,double tdistvel_pow,double tranking,double change_in_dtog, double defensive_player_velocity, double tangleThreshold, int tmid): mappedIds{pidmap}, distanceThreshold{pdist}, homeSide{phomeSide}, gp{tgoal_pow}, bp{tball_pow}, dw{dist_weight}, dp{tdist_pow}, vp{tvel_pow},dvp{tdistvel_pow},ra{tranking},ddg{change_in_dtog},dpv{defensive_player_velocity}, angleThreshold{tangleThreshold}, mid{tmid}
{
	allPlayers.resize(playerSize);
	for(int i = 0;i<allPlayers.size();i++){
		for(int j = 0;j<28;j++){
			for(int k = 0;k<3;k++)
			allPlayers[i].trolley.consecutive[j].push_back(0);
		}
	}

}
std::array<double,100> AllClosest::addPlayers(std::vector<Frame*>::iterator frameit, int previousFid,int prevAttackingTeam,int interestingTeam){
//***
//a member function to add player distances below a set threshhold to a container of 2-arrays
//also creates a differential class
	pressuresum pressureCalc(gp,dp,dw,bp,vp,dvp,ra,ddg,dpv,angleThreshold,interestingTeam);
	int attackingTeam = (*frameit)->getAttacking();
	if((*frameit)->getFid()==previousFid + 1){
		dead_before += 1;
	}else{dead_before= 0;}
	(*frameit)->getPlayersSplit(attackPlayers, defensePlayers);
/*	if((*frameit)->getFid() > 3500 && (*frameit)->getFid() < 3600){
	for (auto playerit = attackPlayers.begin() ; playerit < attackPlayers.end();++playerit){
		int pid = (*playerit)->getMappedPid();
		std::cout << pid << ":" ;
	}
	std::cout << std::endl;
		for (auto playeritb = defensePlayers.begin();playeritb < defensePlayers.end();++playeritb){
			int pidb = (*playeritb)->getMappedPid();
		std::cout << pidb << ":" ;
		}
		std::cout << std::endl;
	for (aut playerit = attackPlayers.begin() ; playerit < attackPlayers.end();++playerit){
		int pid = (*playerit)->getMappedPid();
		if(pid == 15){
		for (auto playeritb = defensePlayers.begin();playeritb < defensePlayers.end();++playeritb){
			double tdistance = 0.01* distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
	//		std::cout << tdistance << ":";
			int pidb = (*playeritb)->getMappedPid();
			if(pidb == 10){
				std::cout << (*frameit)->getFid() << ":" <<  (*playerit)->getPos()[0] << ":" << (*playerit)->getPos()[1] << ":" << (*playeritb)->getPos()[0] << ":" << (*playeritb)->getPos()[1] << std::endl;
			if(tdistance<distanceThreshold){
				double avel, bvel, anum, bnum;
	//			anum = (*playerit)->getNum();
	//			bnum = (*playeritb)->getNum();
	//			std::cout << anum << ":" << bnum <<std::endl;
			//	std::array<double,2> info = {tdistance,getBallDist(frameit,(*playerit)->getPos())};
				//std::array<double,3> info = {tdistance,getGoalDist((*playerit)->getPos(),prevAttackingTeam), getBallDist(frameit,(*playerit)->getPos())};
			//	allPlayers[pid].trolley.addInfo(info, pidb);
			//	allPlayers[pid].trolley.addConsec(1,pidb);
			}
			else{
			//	allPlayers[pid].trolley.addConsec(0,pidb);
			}
		}
		}
	//	std::cout << std::endl;
	}
i	} */
//	std::cout << std::endl;

//	}
//	if(phaseSelector.prepreAttacking == prevAttackingTeaam){
//		framesInPos ++;
//	}
//	else{framesInPos = 0;}
//	phaseSelector.prepreAttacking = prevAttackingTeam
	if(previousFid!=-1){
	//	for (auto allPlayerit = allPlayers.begin();allPlayerit < allPlayers.end();++allPlayerit){
	//		(*allPlayerit).playerPressures << previousFid<<"\t" << prevAttackingTeam << "\t";	
	//	}
		pressureCalc.pressure[1] =0;
		if(prevAttackingTeam == 0){
			for (auto playerit = defensePlayers.begin(); playerit<defensePlayers.end();++playerit){
				double vel = (*playerit)->getVelocity();
				if(vel>6){
					pressureCalc.pressure[1] +=1;
				}
			}

		}
		else{
			for (auto playerit = attackPlayers.begin(); playerit<attackPlayers.end();++playerit){
				double vel = (*playerit)->getVelocity();
				if(vel>6){
					pressureCalc.pressure[1] +=1;
				}
			}
		}
	}

	for (auto playerit = attackPlayers.begin() ; playerit < attackPlayers.end();++playerit){
		int pid = (*playerit)->getMappedPid();
		for (auto playeritb = defensePlayers.begin();playeritb < defensePlayers.end();++playeritb){
			double tdistance = 0.01* distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
			int pidb = (*playeritb)->getMappedPid();
			if(tdistance<distanceThreshold){
				double avel, bvel, anum, bnum;
	//			anum = (*playerit)->getNum();
	//			bnum = (*playeritb)->getNum();
	//			std::cout << anum << ":" << bnum <<std::endl
			//	std::array<double,2> info = {tdistance,getBallDist(frameit,(*playerit)->getPos())};
				std::vector<Player*>::iterator ait;	
				std::vector<Player*>::iterator dit;	
				if(prevAttackingTeam != 0){
					 dit = playerit;
					 ait = playeritb;
				}
				else{
					dit = playeritb;
					ait = playerit;
				}	
				double defendingVel = (*dit)->getVelocity();
				double goalX = getGoalX(attackingTeam);
                                double inPosition = dInPosition((*dit)->getPos(),(*ait)->getPos(),angleThreshold,{goalX,0});
				double distanceToGoal = getGoalDist((*frameit)->getBall()->getPos(),goalX);
				std::array<double,6> info = {tdistance,inPosition, getBallDist(frameit,(*dit)->getPos()), getBallDist(frameit,(*ait)->getPos()),defendingVel,distanceToGoal};
			//	std::array<double,3> info = {tdistance,pid,pidb};
				allPlayers[pid].trolley.addInfo(info, pidb);
				allPlayers[pid].trolley.addConsec(1,pidb);
			}
			else{
				allPlayers[pid].trolley.addConsec(0,pidb);
			}
		}
	}
	if (prevAttackingTeam == 0){
		for(auto it  = attackPlayers.begin();it<attackPlayers.end();++it){
			int pid = (*it)->getMappedPid();
			allPlayers[pid].trolley.addWeights(4);
		}
	}
	else{
		addDWeights(4);
	}

	for(auto it  = attackPlayers.begin();it<attackPlayers.end();++it){
		int pid = (*it)->getMappedPid();
		//allPlayers[pid].trolley.sortBall();
		//allPlayers[pid].trolley.addWeights();
		for (auto itb = defensePlayers.begin();itb < defensePlayers.end();++itb){
			int pidb = (*itb)->getMappedPid();
			std::vector<int> pressureTypes = {8};
			pressureCalc.callPresures(pressureTypes,allPlayers[pid].pullPassInfo(pidb,dead_before), prevAttackingTeam);
		}
	}

	std::vector<Player*>::iterator attackStartIt, defenseStartIt, attackEndIt, defenseEndIt;
        if (attackingTeam == 0){
                attackStartIt = attackPlayers.begin();
                attackEndIt = attackPlayers.end();
                defenseStartIt = defensePlayers.begin();
                defenseEndIt = defensePlayers.end();
	}
        else{
                defenseStartIt = attackPlayers.begin();
                defenseEndIt = attackPlayers.end();
                attackStartIt = defensePlayers.begin();
                attackEndIt = defensePlayers.end();
        }
	/*
        for(auto defenseIt = defenseStartIt;defenseIt < defenseEndIt;defenseIt++){
                int defensePid = (*defenseIt)->getMappedPid();
                int defNum = (*defenseIt)->getNum();
                for(auto attackIt = attackStartIt;attackIt<attackEndIt;++attackIt){
			int attackNum = (*attackIt)->getNum();
                        int attackPid = (*attackIt)->getMappedPid();
                        int homePid, awayPid;
                        if(attackingTeam == 0){
                                homePid = attackPid;
                                awayPid = defensePid;
                        }
                        else{
                                homePid = defensePid;
                                awayPid = attackPid;
                        }
                        std::array<double,8> pairInfo = allPlayers[homePid].pullPassInfo(awayPid,dead_before);
			std::array<double,8> notThis = {-1,-1,-1,-1,-1,-1,-1,-1};
			if (pairInfo != notThis){
				brianNew << mid <<"," << previousFid << "," << prevAttackingTeam << "," << defNum << "," << attackNum << std::setprecision(2) << "," << pairInfo[5] << "," << pairInfo[1] << "," << pairInfo[0] << "," << pairInfo[2] << "," << pairInfo[7] << "," << pairInfo[4] << "," << pairInfo[3] << "," << pairInfo[6] << "," <<pressureCalc.pressure[0] << std::endl;;
			}
		}
	}
	*/


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
double AllClosest::dInPosition(std::array<double,2> defender, std::array<double,2> attacker, double threshold, std::array<double,2> goal){
        std::array<double,2> goalVec;
        std::array<double,2> defenderVec;
        for(int i = 0;i< 2;i++){
                defenderVec[i] = defender[i] - attacker[i];
                goalVec[i] = goal[i] - attacker[i];
        }
        double defenderLength{0}, goalLength{0}, dot{0};
        for(int i = 0;i<2;i++){
                defenderLength += pow(defenderVec[i],2);
                goalLength += pow(goalVec[i],2);
                dot += defenderVec[i]*goalVec[i];
        }
        defenderLength = pow(defenderLength,0.5);
        goalLength = pow(goalLength,0.5);
        double a = dot/(goalLength*defenderLength);
        if(a<-1||a>1){
                std::cout << "uhoha" << std::endl;
		std::cout << a << std::endl;
        }
        double angle = acos(a);
	return angle;
	/*
        if(abs(angle)<3.14*threshold/180){
                return 1;
        }
        else{
                return 0;
        }*/
}
double AllClosest::getGoalDist(std::array<double,2> playerPos, double goalx){
                return 0.01*distance(goalx,playerPos[0],0,playerPos[1]);
}

double AllClosest::getGoalX(int attacking){
        if(attacking == 0){
                if(homeSide == 'L'){
                        return 5250;
                }
                else{
                        -5250;
                }
        }
        else{
                if(homeSide == 'L'){
                        return -5250;
                }
                else{
                        return 5250;
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
void AllClosest::openBrianNew(std::string fileloc){
        brianNew.open(fileloc);
        brianNew << "Match ID, Frame ID, Team in possession,Player shirt number (TOP), Player shirt number(TIP), Velocity (POP), Relative velocity(POP), Distance, Defensive position, Rank (By distance), Distance to ball (PIP), Distance to ball (POP) , Distance of ball to goal, Pressure metric" <<std::endl;
}
void AllClosest::closeBrianNew(){
        brianNew.close();
}
