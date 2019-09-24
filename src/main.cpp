//#include "models/infogroups/infogroupa.h"
#include <vector>
#include <iomanip>
#include <array>
#include <iterator>
#include <string>
#include <cmath>
#include "models/game.h"
#include "models/possessionfilereader/possessionfilemaker.h"
#include "models/briantable/allclosest.h"
#include "models/idmap.h"
int main() {
	/*
*** uses infogroupa
	InfoGroupA * test = new InfoGroupA();
	test->loadData();
	std::cout<<"yay" << std::endl;
	return 0;*/
	int distanceThreshold = 15;
	PossessionFileMaker * tposFiles= new PossessionFileMaker();
//	std::vector<PossessionFileLine*> linesa;
//	std::vector<PossessionFileLine*> linesb;
//	bool done = true;
//	if(tposFiles->readFile()==true){
//		std::vector<PossessionFile*> posFiles = tposFiles->getPosFiles();
//		tposFiles->fileWriter();
//		for(auto fileit = posFiles.begin();fileit<posFiles.end();fileit++){
		PossessionFile * posFile = tposFiles->fileSpecificReader(855206); 
		int mid = posFile->getMid();
		if(mid == 855206){
			std::cout << mid << std::endl;
			Idmap idmapd;
			Game * tgame = new Game(mid,"../idata/"); 
			if(tgame->readFile(true,idmapd)==true&&tgame->storeMdata()==true){
				AllClosest * tallClosest = new AllClosest(distanceThreshold,idmapd.getSize());	
				std::ofstream os;
				std::string filename = "../data/pressuredata/" + std::to_string(mid) + ".txt";
				os.open(filename);
				os << "pressure,time_till_possession_change" << std::endl;
				os << std::setprecision(2)<<std::fixed;
				std::vector<Frame*> frames = tgame->getFrames();
				std::vector<PossessionFileLine*> lines = posFile->getPosFilLin();
				std::vector<PossessionFileLine*>::iterator lineit = lines.begin();
				std::vector<PossessionFileLine*>::iterator lineend = lines.end();
				int prevFid = -1;
				int fid;
				int prevAttackingTeam = -1;
				double prevTimeTillPosChange = 0;
				int attackingTeam;
				bool prevFidBool;
				std::array<std::array<std::array<double,6>,100>,2> prevPlayerstat;
				for(auto frameit = frames.begin();frameit<frames.end();++frameit){
					bool loopPosFilLin = true;
					int fid = (*frameit)->getFid();					
					std::cout<< fid <<std::endl;
					double timeTillPosChange;
					while(loopPosFilLin){
						std::array<int, 2> frameRange= (*lineit)->getFrameRange();
						if(fid>=frameRange[0]){
							if(fid<=frameRange[1]){
								loopPosFilLin = false;
								if((*lineit)->getTid() == tgame->getHome()){
									attackingTeam = 0;
								}else{attackingTeam =1;}
								timeTillPosChange = (frameRange[1]-fid)*0.04;
							}
							else{
								if(lineit<lineend){
								++lineit;
								}
								else{
									loopPosFilLin = false;
									attackingTeam = -1;
								}
							}
						}
						else{
							loopPosFilLin = false;
							attackingTeam = -1;
						}
					}
					double framePressure = tallClosest->addPlayers(frameit, prevFid, prevAttackingTeam);
					os << framePressure << "," << prevTimeTillPosChange << std::endl;
					prevAttackingTeam = attackingTeam;
					prevFid = fid;
					prevTimeTillPosChange = timeTillPosChange;
				}
				os.close();
				delete tallClosest;
			}
			delete tgame;
			std::cout << "done" << std::endl;
			}
	//	}
//	}
	delete tposFiles;
}
