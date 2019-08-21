#include "frame.h"
#include "../helpers/functions.h"
#include "game.h"
#include <cmath>

Frame::Frame() :firstFrame{ false }{}
Frame::~Frame(){
	for(int i = 0; i < players.size();i++){
		delete players[i];
	}
	delete ball;
}
void Frame::addPlayer(Player* player){
	players.push_back(player);
}
void Frame::addBall(Ball* pball){
	ball = pball;
}
void Frame::objectInfo(std::ostream & os,int type) {
	os << fid;
	(*ball).infoToText(os,type);
	for(std::vector<Player*>::iterator playerit=players.begin();playerit<players.end();++playerit){
		os << fid;
		(*(*playerit)).infoToText(os,type);
	}	
}
void Frame::addFid(int pfid) {
	fid = pfid;
}
void Frame::setFirstFrame() {
	firstFrame = true;
}
bool Frame::getFirstFrame() {
	return firstFrame;
}
int Frame::getFid() {
	return fid;
}
Ball* Frame::getBall() {
	return ball;
}
std::vector<Player*>& Frame::getPlayers() {
	return players;
}
Player* Frame::findNum(int num, int team) {
	bool foundNum{ true };
	int j = 0;
	while (foundNum && j < players.size()) {
		if (players[j]->getNum() == num&&players[j]->getTeam()==team) {
			return players[j];
		}
		else j++;
	}
	std::cout << "Error, number not found" << std::endl;
	return nullptr;
}
void Frame::evalSquares(int xsquare, int ysquare){
	int poo;
}

void Frame::closestPlayers(){
	for (auto playerit = players.begin() ; playerit < players.end();++playerit){
		double closestDist = 1000;
		Player* closestPlay;
		for (auto playeritb = players.begin() ; playeritb < players.end();++playeritb){

			if((*playeritb)->getTeam()!=(*playerit)->getTeam()){
				double tdistance = distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
				if(tdistance<closestDist){
					closestDist = tdistance;
					closestPlay = *playeritb;
				}
			}
		}
		(*playerit)->setClosestDist(closestDist);
		(*playerit)->setClosestPlay(closestPlay);
	}
}

double Frame::getVoronoi(double pitchx, double pitchy){
	int home{0};
	int away{0};
	int v = (int) pitchx;
	for (double xt = 0; xt < (int) (pitchx * 2); xt++){
			double x =xt/2;
			for (double yt = 0; yt < (int) (pitchy * 2); yt++){
						double y =yt/2;
						double closestDist=999;
						Player* closestPlayer;
						for (auto playerit = players.begin() ; playerit < players.end();++playerit){
							if (distance(x,(*playerit)->getPos()[0],y,(*playerit)->getPos()[1])<closestDist){
								closestDist = distance(x,(*playerit)->getPos()[0],y,(*playerit)->getPos()[1]);
								closestPlayer = *playerit;
							}
						}
						if(closestPlayer->getTeam() == 0){
							home++;
						}
						else{away++;}
			}
	}
	return home/away;
}

void Frame::computeScalar(int posid){
	//put ball at (0,0)
	//centred attack

	std::vector<std::array<double,2>> centredAttack;
	std::array<double,2> ballPos = ball->getPos();
	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		if ((*playerit)->getTeam()==posid){
			std::array<double, 2> tpos;
			for (int i =0; i< 1;i++){
				tpos[i] = (*playerit)->getPos()[i] - ballPos[i];
			}
			centredAttack.push_back(tpos);
		}
	}
	//transfor to polars
	for(auto playerit = centredAttack.begin(); playerit < centredAttack.end();++playerit){
		double theta = atan2((*playerit)[1], (*playerit)[0]);
		double r = pow(pow((*playerit)[0],2)+pow((*playerit)[1],2),0.5);
		(*playerit)[0] = theta;
		(*playerit)[1] = r;
	}
	//add gaussian
	for(int i = 0;i<720;i++){
		for(auto playerit = centredAttack.begin(); playerit < centredAttack.end();++playerit){
			anglePotential[i] += exp(-(pow(i/2-(*playerit)[0],2)/(2*pow((*playerit)[1],2))));
		}

	}
}
bool Frame::writeScalar(std::ostream & os){
    for(int i=0;i<720;i++){
        os <<anglePotential[i] << "\t";
    }
    os << std::endl;
    return true;
}

