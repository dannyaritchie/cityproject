#include "infogroupa.h"
bool InfoGroupA::loadData(){
//	PossessionFileMaker * tposFiles= new PossessionFileMaker();
//	if(tposFiles->readFile()==true){
//		std::vector<PossessionFile*> posFiles = tposFiles->getPosFiles();
//		for(auto fileit = posFiles.begin();fileit<posFiles.end();fileit++){
		for(int mid = 800000;mid<1100000;mid++){
		//	int mid = (*fileit)->getMid();
			Game * tgame = new Game(mid,"/pc2014-data1/lah/data/data_files/"); 
			if(tgame->readFile()==true&&tgame->storeMdata()==true){
				std::vector<Frame*> frames = tgame->getFrames();
		//		std::vector<PossessionFileLine*> lines = (*fileit)->getPosFilLin();
		//		std::vector<PossessionFileLine*>::iterator lineit = lines.begin();
		//		std::vector<PossessionFileLine*>::iterator lineend = lines.end();
				int prevFid;
				int fid;
		//		int home;
				bool prevFidBool;
				std::array<std::array<std::array<double,6>,100>,2> prevPlayerstat;
				for(auto frameit = frames.begin();frameit<frames.end();++frameit){
					if(mid==803199){
						std::cout << (*frameit)->getFid()<< std::endl;
					}
				//	bool loopPosFilLin = true;
					int fid = (*frameit)->getFid();					
				/*	while(loopPosFilLin){
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
					}*/
					std::vector<Player*> players = (*frameit)->getPlayers();
					Ball* ball =(*frameit)->getBall();
					closestPlayers(players);
					int home = teamPossession(players, ball);
					if (home == 0 || home == 1){
						computeScalar(players, ball, home);
					}
					std::array<std::array<std::array<double,6>,100>,2> playerstat;
					std::array<double,3> dtinfo;
					int team, num;
					for(auto playerit = players.begin();playerit<players.end();++playerit){
						team = (*playerit)->getTeam();
						int possession;
						if(team == home){
							possession =1;
						}
						else { possession = 0;}
						if(home == -1){
							possession = -1;
						}
						if(home== -2){
							possession = -2;
						}
						num = (*playerit)->getNum();
						if (num>75){
							std::cout << "error: player number is too high" <<std::endl;
						}
						playerstat[team][num][0] = fid;
						playerstat[team][num][1] = (*playerit)->getClosestDist();
						playerstat[team][num][4] = possession;
						playerstat[team][num][5] = (*playerit)->getAngScore();
						dtinfo[0] = (*playerit)->getPos()[0];
						dtinfo[1] = (*playerit)->getPos()[1];
						dtinfo[2] = (*playerit)->getClosestDist();
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
					prevFid = fid;
				}
				write(mid);
			}
			else{
				std::cout << "could not read game data for" << mid << std::endl;
			}		
			delete tgame;

		}	
//		delete tposfiles;
		return true;
//	}
//		else{
//			std::cout << "could not open possession file" << std::endl;
//			return false;
//		}
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
		for (int j=0;j<75;j++){
			if(playerstats[i][j].size()>0){
				std::string filename = "../data/infogroupa/" + std::to_string(mid) + "-" + std::to_string(i) + "_" + std::to_string(j) +".txt";
				std::ofstream os;
				os.open(filename);
				os.precision(3);
				os << std::fixed;
				for (auto it = playerstats[i][j].begin(); it< playerstats[i][j].end(); ++it){
					for(int k=0;k<6;k++){
       						os << (*it)[k] << "\t";						
					}
					os << std::endl;
				}
				playerstats[i][j].clear();
				os.close();
			}
		}
	}
}
int InfoGroupA::teamPossession(std::vector<Player*>& players, Ball* ball){
	int contested;
	Player* closestHomePlay;
	Player* closestAwayPlay;
	double homedist = 200;
	double awaydist = 200;
	int ballx = ball->getPos()[0];
	int bally = ball->getPos()[1];
	for(auto playerit = players.begin();playerit<players.end();++playerit){
		if ((*playerit)->getTeam()==0){
			double tdist = distance(ballx,(*playerit)->getPos()[0],bally,(*playerit)->getPos()[1]);
			if(tdist<homedist){
				homedist = tdist;
				closestHomePlay = *playerit;
			}
		}
		else{
			double tdist = distance(ballx,(*playerit)->getPos()[0],bally,(*playerit)->getPos()[1]);
			if(tdist<awaydist){
				awaydist = tdist;
				closestAwayPlay = *playerit;
			}
		}
	}
	if (homedist < 1.5){
		if (awaydist < 1.5){
			contested = -1;
		}
		else{contested = 0;}
	}
	else{
		if(awaydist < 1.5){
			contested = 1;
		}
		else{contested = -2;}
	}
	return contested;
}
void InfoGroupA::computeScalar(std::vector<Player*> & players, Ball* ball, int posid){
	double pi = 3.14;
	std::vector<std::array<double,2>> centredAttack;
	std::vector<std::array<double,2>> centredDefense;
	std::array<double,2> ballPos = ball->getPos();
	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		std::array<double, 2> tpos;
		for (int i =0; i< 2;i++){
			tpos[i] = (*playerit)->getPos()[i] - ballPos[i];
		}
		double theta = atan2(tpos[1],tpos[0]);
		double r = pow(pow(tpos[0],2)+pow(tpos[1],2),0.5);
		std::array<double,2> playPol;
		playPol[0] = theta;
		playPol[1] = r;
		if ((*playerit)->getTeam()==posid){
			centredAttack.push_back(playPol);
		}else{	(*playerit)->setBallCentredPolar(playPol);}
	}
	for(auto playerdit = players.begin(); playerdit<players.end();++playerdit){
		if((*playerdit)->getTeam()!=posid){
			std::array<double, 2> tpos = (*playerdit)->getBallCentredPolar();
			if (tpos[1]<20){
				for(auto playerait = centredAttack.begin(); playerait<centredAttack.end();++playerait){
					if ((*playerait)[1]>tpos[1]&&(*playerait)[1]<25){
						if (tpos[0]>0.2-pi&&tpos[0]<pi-0.2){
							if((*playerait)[0] > tpos[0] - 0.2&&(*playerait)[0]<tpos[0]+0.2){
								(*playerdit)->plusAngScore();
							}
						}
						else{
							if(tpos[0]<=0.2-pi){
								if((*playerait)[0] < tpos[0]+0.2||(*playerait)[0]>-tpos[0]-0.2){
									(*playerdit)->plusAngScore();
								}
							}
							else{
								if((*playerait)[0] > tpos[0]-0.2||(*playerait)[0]<-tpos[0]+0.2){
									(*playerdit)->plusAngScore();
								}
							}
						}
					}
				}
			}
		}
	}
}
