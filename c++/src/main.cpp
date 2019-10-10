#include "models/game.h"
#include "models/briantable/allclosest.h"
#include "helpers/functions.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include<fstream>
#include <vector>
#include <array>
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
	for (int mid = 1059703; mid < 1059704;mid++){
		Game * tgame = new Game(mid, "../idata/TrackingData_MSGPK");
		tgame->readNewFile();
		char homeSide = tgame->getHomeSide();
		std::vector<Frame*> aframes = tgame->getFrames();
	//	tgame->addVelocities();
		AllClosest * tallClosest = new AllClosest(distanceThreshold, tgame->getMap(),tgame->getMapLength(), tgame->getHomeSide());
//		tallClosest->openStreams(mid,tgame->getMap());
		std::ofstream os[100];
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
		PressureProcessor
		for(auto frameit = aframes.begin();frameit<aframes.end();++frameit){
			double time = (*frameit)->getFid()/5;
			std::vector<Player*> players = (*frameit)->getPlayers();
		//	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		//		std::cout << (*playerit)->getPos()[0]<< std::endl;
		//	}
		//	std::cout << "A";
			std::array<double,100> framePressure = tallClosest->addPlayers(frameit, previousFid, previousAttacking);
		//	if(previousAttacking == 0 || previousAttacking == 1){ 
				for(int i = 0;i<1;i++){
//					os[i] << framePressure[i] << "," << time << "," << previousAttacking << std::endl;
					std::array<double,2> temp = {previousFid,framePressure[0]};
					signalProcess.addPressure(temp);
				}
		//	}
			previousAttacking = (*frameit)->getAttacking();
			if(!found){
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
			}
			previousFid = (*frameit)->getFid();			
			previousDistToGoal = distanceToGoald(frameit,aframes.end(),tgame->getHomeSide());
		}
	//	for (int j=0;j<2;j++){
			for(int i = 0; i < 1; i++){
				os[i].close();
			}
	//	}
	//tallClosest->closeStreams();
		delete tallClosest;
		delete tgame;
	}
	return 0;
}

