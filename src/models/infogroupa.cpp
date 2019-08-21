#include "infogroupa.h"
bool ingoGroupA::loadData(){
	int home, mid, tid, fid;
	Game* tgame;
	std::vector<Frame*> frames;
	std::vector<PossessionFileLine*> lines;
	std::vector<PossessionFileLine*>::iterator lineit;
	std::vector<PossessionFileLine*>::iterator lineend;
	std::vector<PossessionFile*> posFiles;
	bool loopPosFilLin;
	PossessionFileMaker * tposFiles= new PossessionFileMaker();
	if(tposFiles->readFile()==true){
		posFiles = tposFiles->getPosFiles();
		loopPosFiles = true;
		for(auto fileit = posFiles.begin();fileit<posFiles.end();fileit++){
			mid = tposFiles->getMid();
			Game * tgame = new Game(mid,"../idata/"); 
			if(tgame->readFile()==true&&tgame->storeMdata()==true){
				frames = tgame->getFrames();
				lines = (*posFilit)->getPosFilLin();
				lineit = lines.begin();
				lineend = lines.end();
				int prevFid;
				bool prevFidBool;
				std::array<std::array<std::array<double,5>,35>,2> prevPlayerstat;
				for(auto frameit = frames.begin;frameit<frames.end;frames++){
					loopPosFilLin = true;
					fid = (*frameit)->getFid();					
					while(loopPosFilLin){
						frameRange= (*lineit)->getFrameRange();
						if(fid>=frameRange[0]){
							if(fid<=frameRange[1]){
								loopPosFilLin = false;
								if((*lineit)->getTid() == tgame->getHome()){
									home = 1;
								}else{home =-1;}
							}
							else{
								if(lineit<lineend){
								++lineit;
								}
								else{
									loopPosFilLin = false;
									home = 0;
								}
							}
						}
						else{
							loopPosFilLin = false;
							home = 0;
						}
					}
				}
				std::vector<Plater*> players = (*frameit)->getPlayers();
				closestPlayers(players);
				std::array<std::array<std::array<double,5>,35>,2> playerstat;
				std::array<double,3> tinfo;
				std::array<double,3> dtinfo;
				int team, num;
				for(auto playerit = players.begin();playerit<players.end();++playerit){
					team = (*playerit)->getTeam();
					num = (*playerit)->getNum();
					playerstat[team][num][0] = fid;
					playerstat[team][num][1] = getClosestDist();
					playerstat[team][num][4] = home;
					dtinfo[0] = (*playerit)->getPos()[0];
					dtinfo[1] = (*playerit)->getPos()[1];
					dtinfo[2] = getClosestDist();
					if(fid != prevFid+1){
						dtdata[team][num].clear();
					}
					dtdata[team][num].push_back(dtinfo);
					if(dtdata[team][num].size()>3){
						dtdata[team][num].erase(dtdata[team][num].begin());
					}
					if(dtdata[team][num].size()==3){
						prevPlayerstat[team][num][2] = velocity(team,num);
						prevPlayerstat[team][num][3] = closestVelocity(team,num);		
					}
					playerstats[team][num].push_back(prevPlayerstat[team][num]);
					prevPlayerstat = playerstat;	
				}
			}
			else{
				std::cout << "could not read game data for" << mid << std::endl;
			}		//calculate frame metrics
		}	
	}
	else{
		std::cout << "could not open possession file" << std::endl;
		return false;
	}
}

void infoGroupA::closestPlayers(std::vector<Player*> & players){
		for (auto playerita = players.begin(); playerita < players.end();++playerita){
			double closestDist = 1000;
			Player* closestPlay;
			for (auto playeritb = players.begin() ; playeritb < players.end();++playeritb){
				if((*playeritb)->getTeam()!=(*playerita)->getTeam()){
					double tdistance = distance((*playerita)->getPos()[0],(*playeritb)->getPos()[0],(*playerita)->getPos()[1],(*playeritb)->getPos()[1]);
					if(tdistance<closestDist){
						closestDist = tdistance;
						closestPlay = *playeritb;
					}
				}
			}
			(*playerita)->setClosestDist(closestDist);
			(*playerita)->setClosestPlay(closestPlay);
		}
}
double infoGroupA::velocity(int team, int num){
		return distance(dtdata[team][num][0][0],dtdata[team][num][2][0],dtdata[team][num][0][1],dtdata[team][num][2][1])/0.2;
}
double infoGroupB::closestVelocity(int team, int num){
	return (dtdata[team][num][2][2] - dtdata[team][num][0][2])/0.2;
}
