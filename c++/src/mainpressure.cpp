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
		std::ofstream os[2][100];
		for (int j = 0;j<2;j++){
			for(int i = 0; i<100;i++){
				std::string filename = "../data/pressuredata/" + std::to_string(mid) + "_" +std::to_string(j) + "_" + std::to_string(i) + ".txt";
				os[j][i].open(filename);
				os[j][i] << "pressure,time_till_possession_change" << std::endl;
				os[j][i] << std::setprecision(4)<<std::fixed;
			}
		}
		int previousAttacking = -2;
		int previousFid = -1;
		int previousTimeTillPossessionChange = 0;
		double previousDistToGoal = 0;
//	std::array<double, 100> previousDistToGoal;
		bool found = false;
		std::vector<Frame*>::iterator it;
		std::vector<Frame*>::iterator bit;
	//	PressureProcessor signalProcess;
		bool faffyPlay{true};
		bool sum{true};
		int firstAmount=1;
		bool useFirstAmount;
		std::array<double,100> pressureSum;
		int frameCount{0};
		for(auto frameit = aframes.begin();frameit<aframes.end();++frameit){
				double afid = (*frameit)->getFid();
			double time = afid/5;
			std::vector<Player*> players = (*frameit)->getPlayers();
		//	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		//		std::cout << (*playerit)->getPos()[0]<< std::endl;
		//	}
		//	std::cout << "A";
			std::array<double,100> framePressure = tallClosest->addPlayers(frameit, previousFid, previousAttacking);
			//std::cout << afid << "<" << framePressure[0] << "IiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiI" << std::endl;
	//		if(useFirstAmount == true){
			//if(!faffyPlay){
				if (firstAmount > -1){
		//				sum = true;
					firstAmount --;
				}
		//			else{
		//				sum = false;
		//				useFirstAmount = false;
		//			}
				else{
					if (frameCount < 4){
						frameCount ++;
						sum = true;
					}
					else{
						frameCount = 0;
						sum = false;
					}
					if (sum == true){
						for(int i = 0;i<100;i++){
							pressureSum[i]+=framePressure[i]/5;
						}
					}
					else{
						for(int i = 0;i<100;i++){
							pressureSum[i] += framePressure[i]/5;
							if(faffyPlay){
								double previousTimeTillPossessionChangea = previousTimeTillPossessionChange/5;
								os[previousAttacking][i] << framePressure[i] << "," << previousTimeTillPossessionChangea << "," << previousDistToGoal << std::endl;
							}
							pressureSum[i] += 0;
						}
					}
				}
		//	}

			//if(faffyPlay){
			//	if(previousAttacking == 0 || previousAttacking == 1){ 
			//		for(int i = 0;i<100;i++){
			//			//os[i] << framePressure[i] << "," << time << "," << previousAttacking << std::endl;
			//		}
			//	}
			//}
	//		double fid = previousFid;
	//		double attacking = previousAttacking;
	//		std::array<double,3> temp = {fid,framePressure[0],attacking};
				//	signalProcess.addPressure(temp);
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
							if (previousTimeTillPossessionChange < 10){
								faffyPlay = false;
							}
							else{
								faffyPlay = true;
							}
							firstAmount = previousTimeTillPossessionChange%5;
							int amountCopy = firstAmount;
							useFirstAmount = true;
						}
					}
					else{finding = false;}
				}
				found = true;
			}
			else{
				previousTimeTillPossessionChange -= 1;
				if(previousTimeTillPossessionChange == 0){
					found = false;
				}
			}
			previousFid = (*frameit)->getFid();			
			previousDistToGoal = distanceToGoald(frameit,aframes.end(),tgame->getHomeSide());
		}
		for (int j=0;j<2;j++){
			for(int i = 0; i < 100; i++){
				os[j][i].close();
			}
		}
	//tallClosest->closeStreams();
		delete tallClosest;
		delete tgame;
//		signalProcess.addFinalPressure();
//		std::array<int,2> result = signalProcess.lengthThreshold(0,0,true,false);
//		std::cout << result[0] << " " << result[1] <<std::endl;
	}
	return 0;
}

