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
	std::advance(frameit, -1);
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
double distanceToGoald(std::vector<Frame*>::iterator frameit,std::vector<Frame*>::iterator endit, char homeSide, int lookEndDist, int lookStartDist){
	int attacking = (*frameit)->getAttacking();
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
	std::vector<Frame*>::iterator advit = frameit;
	if(endit-frameit>lookEndDist+2){
		std::advance(advit, lookEndDist);
	}
	else{
		std::advance(endit, -1);
		advit = endit;
	}
	std::advance(frameit, lookStartDist);
	std::array<double,2> ballPos = (*frameit)->getBall()->getPos();
	double currentDist = distance(ballPos[0],goalx, ballPos[1],0);
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
int main(int argc, char** arg) {
//setting parameters
/*	double goal_pow = atoi(arg[1]);
	double ball_pow = 	atoi(arg[2]);
	double dist_weight= 	atoi(arg[3]);
	double dist_pow =  	atoi(arg[4]);
	double vel_pow =  	atoi(arg[5]);
	double distvel_pow =  	atoi(arg[6]);
	double ranking =  	atoi(arg[7]);
	double min_dis_toDg =  	atoi(arg[8]);
	double max_dis_toDg = 	atoi(arg[9]);
	double pressureLimit= 	atoi(arg[10]);
	double timeLimit =	atoi(arg[11]);
	double lookieLookie =	atoi(arg[12]);
	double pos_time_limit =	atoi(arg[13]);
	double frame_weighting =	atoi(arg[14]);
	double change_in_dtog =	atoi(arg[15]);
	double number_of_bins =	atoi(arg[16]);
	double distThreshold  =	atoi(arg[17]);
	double numberOfGames = atoi(arg[18]);
	std::string filePath = arg[19];
	double start_looking_distance = atoi(arg[20]);
	double looking_length = atoi(arg[21]);
	double defensive_player_velocity = atoi(arg[22]);
	double angleThreshold = atoi(arg[23]);
	*/

	double goal_pow = 0;
	double ball_pow = 1;
	double dist_weight= 2;
	double dist_pow =  5;
	double vel_pow = 3;
	double distvel_pow = 1;
	double ranking = 2;
	double min_dis_toDg =  1000;
	double max_dis_toDg = 9000;
	double pressureLimit= 	0;
	double timeLimit = 2;
	double lookieLookie = 5;
	double pos_time_limit =	5;
	double frame_weighting = 4;
	double change_in_dtog =	0;
	double number_of_bins =	4;
	double distThreshold  =	15;
	double numberOfGames = 1;
	std::string filePath = "../data/test/B";
	double start_looking_distance = 0;
	double looking_length = 8;
	double defensive_player_velocity = 0;
	double angle_threshold = 45;
	std::ofstream results;
	if(start_looking_distance + looking_length > timeLimit*5){
		std::cout << "Error - time limit too small" << std::endl;
		return 1;
	}
	results.open(filePath + "_results");
	results << "Parameters" << std::endl << "--------------------------" << std::endl << "Ball Power: " << ball_pow << std::endl << "Distance Weight: " << dist_weight << std::endl;
        results << "Distance Power: " << dist_pow << std::endl << "Velocity power: " << vel_pow << std::endl << "Distance Velocity Power: " << distvel_pow << std::endl;
	results << "Ranking: " << ranking << std::endl << "Minimum distance to defensive goal: " << min_dis_toDg << std::endl << "Maximum distance to defensive goal: " << max_dis_toDg << std::endl << "Pressure Limit: " << pressureLimit << std::endl << "Time Limit: " << timeLimit << std::endl << "lookieLookie: " << lookieLookie << std::endl << "Possession time limit: " << pos_time_limit << std::endl << "Frame weighting: " << frame_weighting << std::endl << "Angle weighting: " << change_in_dtog << std::endl << "number of bins: " << number_of_bins << std::endl << "distance threshold: " << distThreshold << std::endl << "number of games: " << numberOfGames << std::endl << "start looking distance: " << start_looking_distance << std::endl << "looking length: " << looking_length << std::endl << "defensive player velocity: " << defensive_player_velocity << std::endl << "-------------------------" << std::endl;
	PressureProcessor signalProcess(frame_weighting,filePath);
	signalProcess.setBins();
	signalProcess.openStreams();
	int coutg,coutu;
	std::vector<int> mids;
	for (auto i = 987592;i<987863;++i){
		mids.push_back(i);
	}
	for (auto i = 987864;i<987972;++i){
		mids.push_back(i);
	}
	for (auto i = 918893;i<918913;++i){
		mids.push_back(i);
	}
	for (auto i = 918914;i<918922;++i){
		mids.push_back(i);
	}
	for (auto i = 918923;i<918967;++i){
		mids.push_back(i);
	}
	for (auto i = 918968;i<919273;++i){
		mids.push_back(i);
	}
	for (auto i = 1059702;i<1059782;++i){
		mids.push_back(i);
	}
//	mids.push_back(1059705);
	std::string rempath = "/pc2014-data1/lah/data_msgpk/";
	std::array<double,2> fResults = {0,0};
	for (int i = 0;i<numberOfGames;i++){
		int mid;
		if (i<mids.size()){
			mid = mids[i];
		}
		else{std::cout << "not enough fames" << std::endl;
		return 0;
		}
		std::cout << mid << std::endl;
		Game * tgame = new Game(mid, rempath);
		if(tgame->readNewFile()==true){
			char homeSide = tgame->getHomeSide();
			std::vector<Frame*> aframes = tgame->getFrames();
			tgame->addVelocities();
			AllClosest * tallClosest = new AllClosest(distThreshold, tgame->getMap(),tgame->getMapLength(), tgame->getHomeSide(),goal_pow,ball_pow,dist_weight,dist_pow,vel_pow,distvel_pow,ranking,change_in_dtog,defensive_player_velocity,angle_threshold,mid);
			tallClosest->openBrianNew("../data/brianNew/a.csv");
	//		tallClosest->openStreams(mid,tgame->getMap());
			std::ofstream os;
			std::string name = "../data/phasepressure/" + std::to_string(mid) + ".txt";
			os.open(name);
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
			bool faffyPlay;
			for(auto frameit = aframes.begin()+1;frameit<aframes.end();++frameit){
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
				if(gdistance < 1){
					std::cout << "main gd <1"<<gdistance << std::endl;
				}
				if(!faffyPlay){
					if (distanceToGoal(frameit,aframes.end(),tgame->getHomeSide()) > min_dis_toDg && distanceToGoal(frameit,aframes.end(),tgame->getHomeSide()) < max_dis_toDg){
						std::array<double,5> temp = {dfid,framePressure[0],attacking,gdistance,framePressure[1]};
						signalProcess.addPressure(temp);
					}
				}	
			//	os << framePressure[0] << "," << time << std::endl;
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
								if(previousTimeTillPossessionChange < 5*pos_time_limit){
									faffyPlay = true;
								}
								else{faffyPlay = false;}
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
		//		previousDistToGoal = distanceToGoald(frameit,aframes.end(),tgame->getHomeSide(),start_looking_distance, end_looking_distance);
			}
			os.close();
		//	for (int j=0;j<2;j++){
			//	for(int i = 0; i < 1; i++){
			//		os[i].close();
			//	}
		//	}
		//tallClosest->closeStreams();
			tallClosest->closeBrianNew();
			delete tallClosest;
			delete tgame;
			
			signalProcess.addFinalPressure();
			std::array<int,2> result = signalProcess.lengthThreshold(timeLimit,pressureLimit,true,true,lookieLookie);
			std::cout << result[0] << " " << result[1] <<std::endl;
			fResults[0] += result[0];
			fResults[1] += result[1];
			signalProcess.calcPressure(false,false,true,false,start_looking_distance,looking_length);
			signalProcess.clearPhases();
			
		}
		else{std::cout << "stinky" << std::endl;}
	}
	std::cout << "Ratio" << fResults[0]/fResults[1] << std::endl;
	results << "Ratio: " << fResults[0]/fResults[1] << std::endl;
	
	signalProcess.autoBins(1+number_of_bins);
	signalProcess.printBinSize();
	std::vector<std::array<double,6>> stats = signalProcess.getStats(results);
	signalProcess.printBins();
	/*
	for (int i = 0;i<12;i++){
		std::cout << "For number of defenders below threshold:  " << i << std::endl;
		results << "For " << i << " defenders running above the threshold:" << std::endl;
		if(signalProcess.autoBins(2,i)==true){
			std::vector<std::array<double,6>> stats = signalProcess.getStats(i, results);
			results<< std::endl;
	//		signalProcess.printBins(i);
			}
	}
	*/
	signalProcess.closeStreams();
	results.close();
	return 0;
}

