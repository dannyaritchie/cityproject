#include "models/game.h"
#include "models/briantable/allclosest.h"
#include "helpers/functions.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include<fstream>
#include <vector>
#include <array>
double distanceToGoald(std::vector<Frame*>::iterator frameit, std::array<double,2> ballPos, int attacking){
	int goalx;
	if(attacking == 0){
		goalx = -5250;
	}else{goalx = 5250;}
	double currentDist = distance(ballPos[0],goalx, ballPos[1],0);
	std::vector<Frame*>::iterator advit;
	std::advance(advit, 15);
	std::array<double,2> newPos = ((*advit)->getBall())->getPos();
	double finalDist = distance(newPos[0],goalx, newPos[1],0);
	return finalDist-currentDist;
	}
int main() {
	int distanceThreshold = 15;
	for (int mid = 1059703; mid < 1059704;mid++){
		Game * tgame = new Game(mid, "../idata/TrackingData_MSGPK");
		tgame->readNewFile();
		std::vector<Frame*> aframes = tgame->getFrames();
	//	tgame->addVelocities();
		AllClosest * tallClosest = new AllClosest(distanceThreshold, tgame->getMap(),tgame->getMapLength());
//		tallClosest->openStreams(mid,tgame->getMap());
		std::ofstream os[100];
		for(int i = 0; i<100;i++){
                        std::string filename = "../data/pressuredata/" + std::to_string(mid) + "_" + std::to_string(i) + ".txt";
                        os[i].open(filename);
                        os[i] << "pressure,time_till_possession_change" << std::endl;
                	os[i] << std::setprecision(2)<<std::fixed;
                }
		int previousAttacking = -2;
		int previousFid = -1;
		int previousTimeTillPossessionChange = 0;
		bool found = false;
		std::vector<Frame*>::iterator it;
		std::vector<Frame*>::iterator bit;
		for(auto frameit = aframes.begin();frameit<aframes.end();++frameit){
			std::cout << (*frameit)->getFid() << std::endl;
			std::vector<Player*> players = (*frameit)->getPlayers();
		//	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		//		std::cout << (*playerit)->getPos()[0]<< std::endl;
		//	}
			std::array<double,100> framePressure = tallClosest->addPlayers(frameit, previousFid, previousAttacking);
			for(int i = 0;i<100;i++){
	                        os[i] << framePressure[i] << "," << previousTimeTillPossessionChange << std::endl;
                        }
			previousAttacking = (*frameit)->getAttacking();
			if(!found){
				it = frameit;
				bool finding = true;
				while(finding){
					try{
						std::cout << aframes.size()
						std::advance(it,1);
					}
					catch(std::exception& e) {
						finding = false;
					}
					if((*it)->getAttacking()!=previousAttacking){
						finding = false;
						previousTimeTillPossessionChange = it-frameit;
					}
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
			
		}
		for(int i = 0; i < 100; i++){
			os[i].close();
		}
	//tallClosest->closeStreams();
		delete tallClosest;
	}
	return 0;
}

