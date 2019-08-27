#include "infogroupa.h"
bool InfoGroupA::loadData(){
	PossessionFileMaker * tposFiles= new PossessionFileMaker();
	if(tposFiles->readFile()==true){
		std::vector<PossessionFile*> posFiles = tposFiles->getPosFiles();
		for(auto fileit = posFiles.begin();fileit<posFiles.end();fileit++){
			int mid = tposFiles->getMid();
			Game * tgame = new Game(mid,"../idata/"); 
			if(tgame->readFile()==true&&tgame->storeMdata()==true){
				std::vector<Frame*> frames = tgame->getFrames();
				std::vector<PossessionFileLine*> lines = (*fileit)->getPosFilLin();
				std::vector<PossessionFileLine*>::iterator lineit = lines.begin();
				std::vector<PossessionFileLine*>::iterator lineend = lines.end();
				int prevFid;
				int fid;
				bool prevFidBool;
				std::array<std::array<std::array<double,5>,35>,2> prevPlayerstat;
				for(auto frameit = frames.begin;frameit<frames.end;frames++){
					bool loopPosFilLin = true;
					int fid = (*frameit)->getFid();					
					while(loopPosFilLin){
						std::array<int, 2> frameRange= (*lineit)->getFrameRange();
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
				std::vector<Player*> players = (*frameit)->getPlayers();
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
					write(mid);
				}
			}
			else{
				std::cout << "could not read game data for" << mid << std::endl;
			}		
		delete tgame;
		}	
	}
	else{
		std::cout << "could not open possession file" << std::endl;
		return false;
	}
}

void InfoGroupA::closestPlayers(std::vector<Player*> & players){
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
double InfoGroupA::velocity(int team, int num){
		return distance(dtdata[team][num][0][0],dtdata[team][num][2][0],dtdata[team][num][0][1],dtdata[team][num][2][1])/0.2;
}
double InfoGroupA::closestVelocity(int team, int num){
	return (dtdata[team][num][2][2] - dtdata[team][num][0][2])/0.2;
}
void InfoGroupA::write(int mid){
	for (int i=0;i<2;i++){
		for (int j=0;j<35;j++){
			if(playerstats[i][j].size()>0){
				std::string filename = "../data/infogroupa/" + std::to_string(mid) + "-" + std::to_string(i) + "_" + std::to_string(j) +".txt"
				ofstream os;
				os.open(filename);
				for (auto it = playerstats[i][j].begin(); it< playerstats[i][j].end(); ++it){
					for(int k=0;k<5;k++){
       						os << (*it)[k] << "\t";						
					}
					os << std::endl;
				}
				os.close();
			}
		}
	}
}
