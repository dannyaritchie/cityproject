#include "game.h"
#include <stdlib.h>
#include "frame.h"
#include "ball.h"
#include "player.h"
#include <fstream>
#include <iostream>
#include<sstream>
#include<string>
#include<array>

//Game::Game(){
//	filepath = "/pc2014-data1/lah/data/data_files/987881";
//}
Game::~Game(){
	for(int i = 0;i<frames.size();i++){
		delete frames[i];
	}
}
Game::Game(int pmid): mid{pmid}{};
void Game::addFrame(Frame* pframe){
	frames.push_back(pframe);
}
bool Game::readFile(){
	std::ifstream inFile;
	std::string filename = "/pc2014-data1/lah/data/data_files/" + std::to_string(mid) + ".csv";
	inFile.open(filename);
	if (!inFile) {
		std::cout << "Unable to open file" << std::endl;
		return false;
	}
	std::string bin;
	std::getline(inFile, bin);
	std::string lineSave;
	bool lineSaveFlag=true;
	int fid = 0;
	while (inFile) {
		std::string pline;
		if (lineSaveFlag) {
			std::getline(inFile, pline);
		}
		else {
			pline = lineSave;
			lineSaveFlag = true;
		}
		std::stringstream sline(pline);
		std::size_t found = pline.find("Ball");
		if (found != std::string::npos) {
			Frame* tframe = new Frame;
			bool corFrame = true;
			//ball line so create frame
			double ballx, bally;
			sline >> bin >> fid;
			tframe->addFid(fid);
			for (int i = 0; i < 4; i++) {
				sline >> bin;
			}
			sline >> ballx >> bally;

			Ball* tball = new Ball(ballx, bally, 0);
			tframe->addBall(tball);
			while (corFrame) {
				if (inFile) {
					std::getline(inFile, pline);
					std::stringstream sline(pline);
					std::size_t found = pline.find("Ball");
					if (found == std::string::npos) {
						double playx, playy;
						int num, home;
						for (int i = 0; i < 5; i++) {
							sline >> bin;
						}
						sline >> num >> playx >> playy;

						found = pline.find("Home");
						if (found != std::string::npos) {
							home = 1;
						}
						else { home = 0; }
						Player* tplayer = new Player(num, playx, playy, home);
						tframe->addPlayer(tplayer);

					}
					else {
						lineSave = pline;
						addFrame(tframe);
						corFrame = false;
						lineSaveFlag = false;
					}
				}
				else {
					corFrame = false;
				}
			}
		}
	}
	inFile.close();
	std::cout << "Read file: " << frames.size() << std::endl;
	return true;
}

void Game::writeFile(std::ofstream & os,int type){
	int count = 0;
	for(std::vector<Frame*>::iterator frameit = frames.begin();frameit<frames.end();++frameit){
		(*frameit)->objectInfo(os,type);
		count++;
	}
	std::cout << "line wrote" << count << std::endl;
}

void Game::findPhases() {
	//find the breaks in game data
	frames[0]->setFirstFrame();
	int previousFid = frames[0]->getFid();
	for (std::vector<Frame*>::iterator frameit = frames.begin(); frameit < frames.end(); ++frameit) {
		if (((*frameit)->getFid()) != previousFid + 1) {
			(*frameit)->setFirstFrame();
		}
		previousFid = ((*frameit)->getFid());
	}
}

void Game::addVelocities(){
	//add velocity data to frames
	//uses current +-2 frames so does not use frames close to new phases
	std::array<std::array<std::vector<std::array<double,2>>,50>,2> positions;
	//for each team for each player a vector of 2-array co-ordinates
	for (std::vector<Frame*>::iterator frameit = frames.begin(); frameit < frames.end(); ++frameit) {
		if ((*frameit)->getFirstFrame() == true) {
			for (int i = 0; i < 50; i++) {
				for(int k=0;k<2;k++){
					positions[k][i].clear();
				}
			}
		}
		positions[0][0].push_back((*frameit)->getBall()->getPos());
		if (positions[0][0].size() > 5) {
			positions[0][0].erase(positions[0][0].begin());
		}
		if (positions[0][0].size() == 5) {
			std::array<double,2> velocity;
			for (int j = 0; j < 2; j++) {
				velocity[j] = ((positions[0][0][0][j]) - 8 * (positions[0][0][1][j])
					+ 8 * (positions[0][0][3][j]) - (positions[0][0][4][j])) / (12 * 0.2);
			}
			(*(frameit - 2))->getBall()->setVelocity(velocity);
		}
		for (auto playerit = ((*frameit)->getPlayers()).begin(); playerit < ((*frameit)->getPlayers()).end();
			++playerit) {
			if((*playerit)->getNum()>34){
				std::cout << "Error player number higher than 34" << std::endl;
			}
			int teamb = (*playerit)->getTeam();
			int numb = (*playerit)->getNum();
			std::array<double,2> posb = (*playerit)->getPos();
			positions[teamb][numb].push_back(posb);
			if (positions[teamb][numb].size() > 5) {
				positions[teamb][numb].erase(positions[teamb][numb].begin());
			}
			if (positions[teamb][numb].size() == 5) {
				std::array<double,2> velocity;
				for (int j = 0; j < 2; j++) {
					velocity[j] = ((positions[teamb][numb][0][j]) - 8 * (positions[teamb][numb][1][j])
						+ 8 * (positions[teamb][numb][3][j]) - (positions[teamb][numb][4][j])) / (12 * 0.2);
				}
				(*(frameit - 2))->findNum(numb,teamb)->setVelocity(velocity);
			}
		}
	}

}
void Game::setClosestPlayers(){
	for(auto frameit = frames.begin();frameit<frames.end();++frameit){
		(*frameit)->closestPlayers();
	}
}
void Game::writeClosestPlayers(int game){
	for(auto frameit = frames.begin();frameit<frames.end();++frameit){
		if( (*frameit)->getFid()== 33787){
			int poo;
			poo ++;
		}
		const std::vector<Player*>&	playersFrame = (*frameit)->getPlayers();
		for(auto playerit = playersFrame.begin();playerit<playersFrame.end();++playerit){
			std::array<double,2> tinfo;
			tinfo[0] = (*frameit)->getFid();
			tinfo[1] = (*playerit)->getClosestDist();
			int tnum = (*playerit)->getNum();
			int tteam = (*playerit)->getTeam();
			bool found = true;
			for(auto playerGameit = playersGame.begin();playerGameit<playersGame.end();++playerGameit){
				if(found){
					if((*playerGameit)->getNum() == tnum && (*playerGameit)->getTeam() == tteam){
						(*playerGameit)->addDist(tinfo);
						found = false;
					}
				}
			}
			if (found){
					WholePlayer* tWholePlayer = new WholePlayer(tnum, tteam);
					tWholePlayer->addDist(tinfo);
					playersGame.push_back(tWholePlayer);
			}
		}
	}
	for(auto playerit = playersGame.begin();playerit<playersGame.end();++playerit){
		std::vector<std::array<double,2>> stencil;
		std::vector<std::array<double,2>> velocity;
		double tvelocity;
		bool inVec{true};
		int i = 0;
		int previousFid{0};
		while(inVec){
			try{
				std::array<double,2> temp = (*playerit)->getClosestDistance(i);
				if (temp[0] == previousFid + 1){
					stencil.push_back(temp);
					previousFid = temp[0];
					if (stencil.size()>3){
						stencil.erase(stencil.begin());
					}
					if (stencil.size() == 3){
						tvelocity = (stencil[2][1]-stencil[0][1])/0.4;
						std::array<double,2> tvelar = {stencil[1][0], tvelocity};
						velocity.push_back(tvelar);
					}
				}
				else{
					stencil.clear();
					stencil.push_back(temp);
				}
				i++;
			}
			catch (std::out_of_range& e){
				inVec = false;
			}

		}
	}
	for(auto playerit = playersGame.begin();playerit<playersGame.end();++playerit){
		std::string filename = "../../data/playerclosestdist/" + std::to_string(game) + "_" + std::to_string((*playerit)->getTeam()) + "-" + std::to_string((*playerit)->getNum()) +".txt";
		std::ofstream os;
		os.open(filename);
		os << "Frame" << "\t" << "Closesplayer" << std::endl;
		int i = 0;
		bool stop = true;
		while (stop){
			try{
				(*playerit)->getClosestDistance(i);
				os << (*playerit)->getClosestDistance(i)[0] << "\t" << (*playerit)->getClosestDistance(i)[1] << std::endl;
				i++;
			}
			catch (std::out_of_range& e){
							stop = false;
				
		}
		os.close();
	}
}
void Game::storeMdata(){
	std::ifstream inFile;
	inFile.open("987715.mdata");
	if (!inFile) {
		std::cout << "Unable to open file";
		return;
	}
	int bin;
	inFile >> bin >> pitchx >>pitchy >> homeNum >> awayNum;
}
std::vector<double> Game::calcVoronoi(){
	std::vector<double> ratios;
	for(auto frameit=frames.begin();frameit<frames.end();++frameit){
		ratios.push_back((*frameit)->getVoronoi(pitchx,pitchy));
	}
	return ratios;
}
void Game::writeVoronoi(){
	std::ofstream os;
	std::string filename = "data/voronoi/" + std::to_string(mid) + ".txt";
	os.open(filename);
	std::vector<double> ratios = calcVoronoi();
	for(auto i = 0; i<ratios.size();i++){
		os << ratios[i] << std::endl;
	}
	os.close();
}
void Game::splitFrames(){
	std::string filename;
	filename = "posdata/" + std::to_string(mid) + ".txt";
	std::ifstream inFile;
	inFile.open(filename);
	if (!inFile) {
		std::cout << "Unable to open possession file for: " << filename << std::endl;
		return;
	}
	while(inFile){
		int startFrame, endFrame, tid;
		inFile >> tid >> startFrame >> endFrame;
		bool stopPlace{true};
		int count{0};
		while(stopPlace&&count<frames.size()){
			if (frames[count]->getFid()>=startFrame && frames[count]->getFid()<=endFrame){
				std::cout << frames[count] << std::endl;
				if (tid ==homeNum){
					homeFrames.push_back(frames[count]);
					count ++;
				}
				else {
					awayFrames.push_back(frames[count]);
					count ++;
				}
			}
			if (frames[count]->getFid()>endFrame){
				stopPlace = false;

			} else{count ++;}
		}
	}
	std::cout << "Frame save ratio: " << (homeFrames.size()+awayFrames.size())/frames.size() << std::endl;
}

void Game::getScalar(){
	for (auto frameit = frames.begin();frameit<frames.end();++frameit){
		(*frameit)->computeScalar(0);
	}
}
