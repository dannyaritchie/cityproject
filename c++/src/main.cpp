#include "models/game.h"
#include "models/briantable/allclosest.h"
#include "models/briantable/pressureprocessor.h"
#include "helpers/functions.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include<fstream>
#include <vector>
#include <array>
double distanceToGoal(std::vector<Frame*>::iterator frameit,std::vector<Frame*>::iterator endit, char homeSide){
	int attacking = (*frameit)->getAttacking();
	std::array<double,2> ballPos = (*frameit)->getBall()->getPos();
	int goalx;
	if(attacking == 0){
		if(homeSide == 'L'){
			goalx = 5250;
		}
		else{goalx = -5250;}
	}else{
		if(homeSide == 'L'){
			goalx = -5250;
		}else{goalx = 5250;}
	}
	double currentDist = distance(ballPos[0],goalx, ballPos[1],0);
	return currentDist;
}
double distanceToGoald(std::vector<Frame*>::iterator frameit,std::vector<Frame*>::iterator endit, char homeSide){
	int attacking = (*frameit)->getAttacking();
	std::array<double,2> ballPos = (*frameit)->getBall()->getPos();
	int goalx;
	if(attacking == 0){
		if(homeSide == 'L'){
			goalx = 5250;
		}
		else{goalx = -5250;}
	}else{
		if(homeSide == 'L'){
			goalx = -5250;
		}else{goalx = 5250;}
	}
	double currentDist = distance(ballPos[0],goalx, ballPos[1],0);
	std::vector<Frame*>::iterator advit = frameit;
	if(endit-frameit>32){
		std::advance(advit, 30);
	}
	else{
		std::advance(endit, -1);
		advit = endit;
	}
	std::array<double,2> newPos = ((*advit)->getBall())->getPos();
	double finalDist = distance(newPos[0],goalx, newPos[1],0);
	return finalDist-currentDist;
}
std::array<double,100> distanceToGoalda(std::vector<Frame*>::iterator frameit,std::vector<Frame*>::iterator endit, char homeSide){
	int attacking = (*frameit)->getAttacking();
	std::array<double,2> ballPos = (*frameit)->getBall()->getPos();
	int goalx;
	if(attacking == 0){
		if(homeSide == 'L'){
			goalx = 5250;
		}
		else{goalx = -5250;}
	}else{
		if(homeSide == 'L'){
			goalx = -5250;
		}else{goalx = 5250;}
	}
	std::array<std::vector<Frame*>::iterator,100> advit;
	double currentDist = distance(ballPos[0],goalx, ballPos[1],0);
	std::array<std::array<double,2>,100> newPos;
	std::array<double,100> fdistances;
	for(int i=0;i<100;i++){
		advit[i] = frameit;
		if(endit-frameit>i+2){
			std::advance(advit[i], i);
		}
		else{
			std::advance(endit, -1);
			advit[i] = endit;
		}
		newPos[i] = ((*advit[i])->getBall())->getPos();
		double finalDist = distance(newPos[i][0],goalx, newPos[i][1],0);
		fdistances[i] = finalDist-currentDist;
	}
	return fdistances;
}
int main() {
	int distanceThreshold = 15;
	PressureProcessor signalProcess;
	signalProcess.openStreams();
	std::vector<int> mids;
/*	
	for (auto i = 987592;i<987972;++i){
		mids.push_back(i);
	}
	for (auto i = 918893;i<919273;++i){
		mids.push_back(i);
	}
	for (auto i = 1059702;i<1059760;++i){
		mids.push_back(i);
	}*/
	mids.push_back(1059702);
	std::string rempath = "/pc2014-data1/lah/data_msgpk/";
	for (int mid : mids){
		Game * tgame = new Game(mid, "../idata/newmsgpk/");
		if(tgame->readNewFile()==true){
			char homeSide = tgame->getHomeSide();
			std::vector<Frame*> aframes = tgame->getFrames();
		//	tgame->addVelocities();
			AllClosest * tallClosest = new AllClosest(distanceThreshold, tgame->getMap(),tgame->getMapLength(), tgame->getHomeSide());
	//		tallClosest->openStreams(mid,tgame->getMap());
		/*	for (int j = 0;j<2;j++){
				for(int i = 0; i<1;i++){
					std::string filename = "../data/pressuretimehistory/" + std::to_string(mid) + "_" + std::to_string(i) + ".txt";
					os[i].open(filename);
					os[i] << "pressure,time_till_possession_change" << std::endl;
					os[i] << std::setprecision(4)<<std::fixed;
				}
			}*/
			int previousAttacking = -2;
			int previousFid = -1;
			int previousTimeTillPossessionChange = 0;
			double previousDistToGoal;
	//	std::array<double, 100> previousDistToGoal;
			bool found = false;
			std::vector<Frame*>::iterator it;
			std::vector<Frame*>::iterator bit;
			for(auto frameit = aframes.begin();frameit<aframes.end();++frameit){
				double fid = (*frameit)->getFid();
				double time = fid/5;
				std::vector<Player*> players = (*frameit)->getPlayers();
			//	for(auto playerit = players.begin(); playerit < players.end();++playerit){
			//		std::cout << (*playerit)->getPos()[0]<< std::endl;
			//	}
			//	std::cout << "A";
				std::array<double,100> framePressure = tallClosest->addPlayers(frameit, previousFid, previousAttacking);
				double attacking = previousAttacking;
				double dfid = previousFid;
				double gdistance = distanceToGoal(frameit,aframes.end(),tgame->getHomeSide());
				std::array<double,4> temp = {dfid,framePressure[0],attacking,gdistance};
				signalProcess.addPressure(temp);
				previousAttacking = (*frameit)->getAttacking();
		/*		if(!found){
					it = frameit;
					bool finding = true;
					while(finding){
						if(it!=aframes.end()-1){	
							std::advance(it,1);
							if((*it)->getAttacking()!=previousAttacking){
								finding = false;
								previousTimeTillPossessionChange = it-frameit;
							}
						}
						else{finding = false;}
					}
					found = true;
				}
				else{
					if(previousTimeTillPossessionChange > 1){
						previousTimeTillPossessionChange -= 1;
					}
					else{found = false;}
				}*/
				previousFid = (*frameit)->getFid();			
				previousDistToGoal = distanceToGoald(frameit,aframes.end(),tgame->getHomeSide());
			}
		//	for (int j=0;j<2;j++){
			//	for(int i = 0; i < 1; i++){
			//		os[i].close();
			//	}
		//	}
		//tallClosest->closeStreams();
			delete tallClosest;
			delete tgame;
			signalProcess.addFinalPressure();
			std::array<int,2> result = signalProcess.lengthThreshold(4,0,true,true,true);
			std::cout << result[0] << " " << result[1] <<std::endl;
			signalProcess.calcPressure();
			signalProcess.clearPhases();
			}
			else{std::cout << "stinky" << std::endl;}
		}
	signalProcess.closeStreams();
	return 0;
}

