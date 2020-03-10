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
void Frame::getPlayersSplit(std::vector<Player*>& attackPlay,std::vector<Player*>& defensePlay) {
	attackPlay.clear();
	defensePlay.clear();
	for(auto playerit = players.begin();playerit<players.end();++playerit){
//		std::cout << (*playerit)->getNum();
		if((*playerit)->getTeam()==0){
			attackPlay.push_back(*playerit);
		}else{defensePlay.push_back(*playerit);}
	}
}	
void Frame::addPlayer(Player* player){
	players.push_back(player);
}
void Frame::addPossession(int tpossession){
	possession = tpossession;
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
int Frame::getAttacking(){
	return possession;
}
Ball* Frame::getBall() {
	return ball;
}
std::vector<Player*> Frame::getPlayers() {
	return players;
}
Player* Frame::findPid(int pid) {
//	std::cout << std::endl << pid << ": ";
	bool foundNum{ true };
	int j{0};
	while (foundNum && j < players.size()) {
//		std::cout << j << "-" << players[j]->getMappedPid() << ",";
		if (players[j]->getMappedPid() == pid) {
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
void Frame::setGoalPos(char homeSide){
	if(possession == 0){
		if(homeSide == 'L'){
			goalX = 5250;
		}
		else{goalX = -5250;}
	}
	else{
		if(possession == 1){
			if(homeSide == 'L'){
				goalX = -5250;
			}else{goalX = 5250;}
		}
		else{std::cout <<  "Error setting goalX for frame " << fid<< ", undertermined possession" << std::endl; 
			goalX = 0;
		}
	}
}
double Frame::getGoalPos(){
	return goalX;
}

bool Frame::setAttackersInFrontOfBall(){
	double ballX = getBall()->getPos()[0];
	for(auto playerit = players.begin();playerit<players.end();++playerit){
//		std::cout << (*playerit)->getNum();
		if((*playerit)->getTeam()==possession){
		//	std::cout << (*playerit)->getTeam() << "," << possession << std::endl`;
			double playerX = (*playerit)->getPos()[0];
			if (goalX == 5250){
		//		std::cout << "G:" << goalX << "P:" << playerX << "B:" << ballX << std::endl;
				(*playerit)->setDistanceInFrontOfBall(playerX-ballX);
			}
			else{
				if(goalX == -5250){
		//			std::cout << "G:" << goalX << "P:" << playerX << "B:" << ballX << std::endl;
					(*playerit)->setDistanceInFrontOfBall(ballX-playerX);
				}
				else{
					std::cout << "Error in players in front of ball, goal position was not set" << std::endl;
					return false;
				}
			}
		}
	}
	return true;
}
int Frame::getAttackersInFrontOfBall(double threshold){
	int count{0};
	for(auto playerit = players.begin();playerit<players.end();++playerit){
		if((*playerit)->getTeam()==possession){
			if((*playerit)->getDistanceInFrontOfBall()>threshold){
				count++;
			}
		}
	}
	return count;
}
bool Frame::isInside(double min, double max){
	std::array<double,2> goal = {goalX,0};
	if(distance(getBall()->getPos(),goal)>min&&distance(getBall()->getPos(),goal)<max){
		return true;
	}else{return false;}
}

std::vector<Player*> Frame::attackersInRadius(double radius){
	std::vector<Player*> playerss;
	std::array<double,2> ballPos = getBall()->getPos();
	for(auto playerit = players.begin();playerit<players.end();++playerit){
		if((*playerit)->getTeam() == possession){
			if(distance((*playerit)->getPos(),ballPos)<radius){
				playerss.push_back(*playerit);
			}
		}
	}
	return playerss;
}

int Frame::attackersBallTime(double radius, std::vector<double> & times){
	int unmarkedPlayers{0};
	std::vector<Player*> playerss = attackersInRadius(radius);
	for(auto playerit = playerss.begin();playerit<playerss.end();++playerit){
		double ttime;
		if((*playerit)->getShortestTime(ttime) == true){
			times.push_back(ttime);
		}
		else{
			unmarkedPlayers++;
		}
	}
	return unmarkedPlayers;
}
		
void Frame::setPlayerDistance(double radius){
	for(auto playerita = players.begin();playerita<players.end();++playerita){
		if((*playerita)->getTeam() == possession){
			for(auto playeritd = players.begin();playeritd<players.end();++playeritd){
				if((*playeritd)->getTeam()!=possession){
		//			std::cout << "D" << distance((*playerita)->getPos(),(*playeritd)->getPos())<< std::endl;
					if(distance((*playerita)->getPos(),(*playeritd)->getPos())<radius){
						double pdistance = distance((*playerita)->getPos(),(*playeritd)->getPos());
						(*playerita)->setPlayerPlayerDistance(pdistance,(*playeritd)->getMappedPid());
					}
				}
			}
		}
	}
}	

int Frame::getRunningForward(double minVelocity){
	int count{0};
	for(auto playerit = players.begin();playerit<players.end();++playerit){
		if((*playerit)->getTeam()!=possession){
			if((*playerit)->getVelocity()>minVelocity){
				if((*playerit)->getVelocityVector()[0]*goalX<0){
					count ++;
				}
			}
		}
	}
	return count;
}
Player* Frame::closestPlayerToBall(){
	double minDist{9999};
	std::array<double,2> ballPos = getBall()->getPos();
	Player* closestPlayer;
	for(auto playerit = players.begin();playerit<players.end();++playerit){
		if((*playerit)->getTeam()==possession){
			double ballDistance = distance((*playerit)->getPos(),ballPos);
			if(ballDistance<minDist){
				minDist = ballDistance;
				closestPlayer = (*playerit);
			}
		}
	}
	return closestPlayer;
}
			
double Frame::getPressure(double radius){
	std::vector<double> times;
	double closestPlayerTime;
	double largestTimes{0};
	if(closestPlayerToBall()->getShortestTime(closestPlayerTime)==true){
		int unmarked = attackersBallTime(radius, times);
		std::sort(times.begin(),times.end());
		if(times.size()>2){
			for(auto it = times.end()-3;it<times.end();it++){
				largestTimes+= (*it);
			}
		}
		if(times.size()==2){
			largestTimes = times[0] + 2* times[1];
		}
		if(times.size()==1){
			largestTimes = 3*times[0];
		}
		if(times.size()==0){
			std::cout << "No players where in radius of ball" <<std::endl;
			largestTimes = 0.1;
		}
		return pow(closestPlayerTime*largestTimes,-1);
	}
	else{return 0;}

}

double Frame::getBallDistance(){
	std::array<double,2> ballPos = getBall()->getPos();
	std::array<double,2> goalPos = {goalX,0};
	return distance(ballPos,goalPos);
}

double Frame::getPressureB(double closePressure, std::array<double, 7> parameters, double playerRadius){
	double pressure{0};
	// components of parameters = {vel_pow, distvel_pow, ball_pow, dist_weight, dist_pow, ranking, frame_weighting}	

	// for all players in the attacking team
	for(auto playerita = players.begin();playerita<players.end();++playerita)
	{
		if((*playerita)->getTeam()==possession)
		{ // if the team is in possession, sort the distance of all the defending players
			std::vector<defenderdistance> sorter;
			for(auto playeritd = players.begin();playeritd<players.end();++playeritd)
			{
				if((*playeritd)->getTeam()!=possession)
				{
					double d = (*playerita)->getPlayerPlayerDistance((*playeritd)->getMappedPid());
					defenderdistance temp;
				       	temp.distance = d;
					temp.defender = *playeritd;
					sorter.push_back(temp);
				}
			}
			std::sort(sorter.begin(), sorter.end(), [](const defenderdistance &a, const defenderdistance &b)
			{
				return a.distance < b.distance;
			});

			// for all players in the defending team
			std::array<double, 2> playerAPos = (*playerita)->getPos(); // xy position of playerita
			std::array<double, 2> ballPos = ball->getPos(); // xy position of ball
			double relBallPos = distance(playerAPos, ballPos); //relative position of the ball to attacker

			for(auto playeritd = players.begin();playeritd<players.end();++playeritd){
				if((*playeritd)->getTeam()!=possession){ // if the team is NOT in possession
					// get the distance and relative velocity off the defender to the attacker
					double playerPlayerDistance = (*playerita)->getPlayerPlayerDistance((*playeritd)->getMappedPid());
					double playerPlayerVelocity = (*playerita)->getPlayerPlayerVelocity((*playeritd)->getMappedPid());

					if(playerPlayerDistance !=0 && playerPlayerDistance <= playerRadius)
					{
						//if(playerPlayerVelocity > 0 && playerPlayerDistance < 100)
						//{
						//	pressure += closePressure;
						//}
						if(playerPlayerVelocity > 0)// && playerPlayerDistance >= 100)
						{
							//ld pressure function
							std::vector<defenderdistance>::iterator it;
							int index = 0;
							bool found{false};
							it = sorter.begin();
							while(it != sorter.end() && !found)
							{	
								if((*it).defender == *playeritd)
								{
								       found = true;
								}
								index ++;
								++it;
						 	}	

							pressure +=  pow(pow(index + 1, parameters[5]), -1) * pow(pow(0.01 * relBallPos, parameters[2]), -1) * (pow(10, parameters[3]) / pow(playerPlayerDistance, parameters[4]) + 1) * (pow(playerPlayerVelocity, parameters[0]) / pow(playerPlayerDistance, parameters[1]));
						}
					}
				}
			}
		}
	}
	return pressure;
}
	
