//#include "models/infogroups/infogroupa.h"
#include <vector>
#include <array>
#include <iterator>
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
	std::vector<PossessionFileLine*> linesa;
	std::vector<PossessionFileLine*> linesb;
	bool done = true;
	if(tposFiles->readFile()==true){
		std::vector<PossessionFile*> posFiles = tposFiles->getPosFiles();
		tposFiles->fileWriter();
		for(auto fileit = posFiles.begin();fileit<posFiles.end();fileit++){
			if((*fileit)->getMid() == 914033){	
				int mid = (*fileit)->getMid();
				Idmap idmapd;
				Game * tgame = new Game(mid,"../idata/"); 
				if(tgame->readFile(true,idmapd)==true&&tgame->storeMdata()==true){
					AllClosest * tallClosest = new AllClosest(distanceThreshold);	
					tallClosest->openStreams(mid,idmapd);
					std::vector<Frame*> frames = tgame->getFrames();
					std::vector<PossessionFileLine*> lines = (*fileit)->getPosFilLin();
					std::cout << lines.size()<<std::endl;
					std::vector<PossessionFileLine*>::iterator lineit = lines.begin();
					std::vector<PossessionFileLine*>::iterator lineend = lines.end();
					int prevFid;
					int fid;
					int prevAttackingTeam;
					int attackingTeam;
					bool prevFidBool;
					std::array<std::array<std::array<double,6>,100>,2> prevPlayerstat;
					for(auto frameit = frames.begin();frameit<frames.end();++frameit){
						bool loopPosFilLin = true;
						int fid = (*frameit)->getFid();					
						while(loopPosFilLin){
							std::array<int, 2> frameRange= (*lineit)->getFrameRange();
							if(fid>=frameRange[0]){
								if(fid<=frameRange[1]){
									loopPosFilLin = false;
									if((*lineit)->getTid() == tgame->getHome()){
										attackingTeam = 0;
									}else{attackingTeam =1;}
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
						tallClosest->addPlayers(frameit, prevFid, prevAttackingTeam);
						prevAttackingTeam = attackingTeam;
						prevFid = fid;
					}
					tallClosest->closeStreams();
					delete tallClosest;
				}
				delete tgame;
			}
		}
	}
	delete tposFiles;
}
