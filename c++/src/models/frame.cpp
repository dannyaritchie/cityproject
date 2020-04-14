#include <algorithm>
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

double Frame::computeScalar(double radius, bool attack, double coveredAngle){
	//put ball at (0,0)
	//centred attack
	std::cout << "in" << std::endl;

	std::vector<std::array<double,2>> centredAttack;
	std::array<double,2> ballPos = ball->getPos();
	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		if ((*playerit)->getTeam()==possession&&attack==true){
			if(distance((*playerit)->getPos(),ball->getPos())<radius){
				std::array<double, 2> tpos = (*playerit)->getPos();
				std::array<double,2> pos;
				for(int i = 0;i<2;i++){
					pos[i]=tpos[i]-ballPos[i];
					centredAttack.push_back(pos);
				}
			}
		}
		if ((*playerit)->getTeam()!=possession&&attack!=true){
			if(distance((*playerit)->getPos(),ball->getPos())<radius){
				std::array<double, 2> tpos = (*playerit)->getPos();
				std::array<double,2> pos;
				for(int i = 0;i<2;i++){
					pos[i]=tpos[i]-ballPos[i];
					centredAttack.push_back(pos);
				}
			}
		}
	}
	//transfor to polars
	std::vector<double> thetas;
	for(auto playerit = centredAttack.begin(); playerit < centredAttack.end();++playerit){
		double theta = atan2((*playerit)[1], (*playerit)[0]);
		thetas.push_back(180*theta/3.14 + 180);
		std::cout << "theta:" << 180*theta/3.14 + 180<< std::endl;
	}
	//add gaussian
	std::array<int,360> freeAngle;
	for(int i = 0;i<360;i++){
		freeAngle[i] = 1;
		for(auto theta : thetas){
			if(i>=theta-coveredAngle&&i<=theta+coveredAngle){
				freeAngle[i]=0;
			}
		}
	}
	int sum{0};
	for (auto i : freeAngle){
		sum += i;
	}
	std::cout << "sum:" << sum << std::endl;
	return sum;
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
	std::cout << playerss.size() << std::endl;
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
		return 1000*pow(closestPlayerTime*largestTimes,-1);
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
	double meanPressure{0};
	double oldPressureMetric{0};
	double pressureRanking{0};
	int i{0};
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

			// amy 26-03-2020 repeat readings for only attackers within 20m of the ball here
			if(relBallPos <= 2000){
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
								++it;//go to next element in container of iterator
						 	}	
							
							//amy 24-03-2020 changing the pressure to calculate the average instead of the sum
							i ++;

							//amy 24-03-2020 changing everything to the same units of metres
							//oldPressureMetric +=  pow(pow(index + 1, parameters[5]), -1) * pow(pow(0.01 * relBallPos, parameters[2]), -1) * (pow(10, parameters[3]) / pow(0.01 * playerPlayerDistance, parameters[4]) + 1) * (pow(0.01 * playerPlayerVelocity, parameters[0]) / pow(0.01 * playerPlayerDistance, parameters[1]));

							//amy 26-03-2020 change pressure metric to take into account everything 
							//directly proportionally, continue with metres, not cm
							//pressure += pow(0.01*playerPlayerVelocity, parameters[0]) * pow(0.01*playerPlayerDistance, parameters[1]) * pow(0.01*relBallPos, parameters[2]);
							//amy 26-03-2020 for use when nor using the mean - defender distance ranking
							pressureRanking += (pow(0.01*playerPlayerVelocity, parameters[0]) * pow(0.01*playerPlayerDistance, parameters[1]) * pow(0.01*relBallPos, parameters[2])) * pow(pow(index+1, parameters[5]), -1);

						}
					}
				}
			}
			//amy 26-03-2020 end of if statement below
			}
		}
	}
	//meanPressure = pressure / i;
	//amy 24-03-2020 return the meanPressure instead of the actual pressure
	//comment which one not in use
	//this will get rid of the dependence on the number of defenders within the radius
	//return oldPressureMetric;
	//return pressure;
	return pressureRanking;
	//return meanPressure;
}
/*B
std::vector<double> Frame::getPressureComponents(){
	std::vector<double> pressureComponents;
	std::array<double,7> parameters;
	for(int i =2;i<9;i=i+2){
		parameters = {-0.1*i,0,0,0,0,0,0};
		pressureComponents.push_back(getPressureB(0,parameters,1500));
	}
	parameters = {0,-0.5,0,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0.1,0,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0.5,0,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,1,0,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0,0.5,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0,1,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0,2,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,4,0,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0,0,0,0,0.5,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0,0,0,0,1,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0,0,0,0,2,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	parameters = {0,0,0,0,0,0,0};
	pressureComponents.push_back(getPressureB(0,parameters,1500));
	return pressureComponents;
}*/

	/*A
std::vector<double> Frame::getPressureComponents(){
	std::vector<double> pressureComponents;

	pressureComponents.push_back(getPressureB(0, {0.2,0,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0.5,0,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {1.0,0,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {1.5,0,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {2.0,0,0,0,0,1,0}, 1500));
	
	pressureComponents.push_back(getPressureB(0, {0,0.2,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,0.5,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,1.0,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,1.5,0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,2.0,0,0,0,1,0}, 1500));

	pressureComponents.push_back(getPressureB(0, {0,0,0.2,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,0,0.5,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,0,1.0,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,0,1.5,0,0,1,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,0,2.0,0,0,1,0}, 1500));

	pressureComponents.push_back(getPressureB(0, {0,0,0,0,0,0,0}, 1500));
	pressureComponents.push_back(getPressureB(0, {0,0,0,0,0,1,0}, 1500));
	
	return pressureComponents;
}*/
/*C
std::vector<double> Frame::getPressureComponents(){
	std::vector<double> pressureComponents;
	pressureComponents.push_back(dDA());
	for(int i = 500; i < 2501; i=i+1000){
		pressureComponents.push_back(numberOfPlayersInRadius(i,true));
		pressureComponents.push_back(numberOfPlayersInRadius(i,false));
	}
	pressureComponents.push_back(computeScalar(1500.0,true,5.0));
	pressureComponents.push_back(computeScalar(1500.0,false,5.0));
	pressureComponents.push_back(computeScalar(1500.0,true,15.0));
	pressureComponents.push_back(computeScalar(1500.0,false,15.0));
	return pressureComponents;
}*/
//D
std::vector<double> Frame::getPressureComponents(){
	std::vector<double> pressureComponents;
	std::vector<std::array<double,2>> sortedDM = getMDToClosestNPlayers(true);
	for (int i = 0; i<6;i++){
		pressureComponents.push_back(sortedDM[i][0]);
		pressureComponents.push_back(sortedDM[i][1]);
	}
	sortedDM = getMDToClosestNPlayers(false);
	for (int i = 0; i<6;i++){
		pressureComponents.push_back(sortedDM[i][0]);
		pressureComponents.push_back(sortedDM[i][1]);
	}
	std::vector<double> sortedTheta = computeScalarCombined(2500, 200, 4);
	for (int i = 0; i < 4; i ++){
		pressureComponents.push_back(sortedTheta[i]);
	}
//	sortedTheta = computeScalarCombined(2000, 100, 5);
//	for (int i = 0; i < 4; i ++){
//		pressureComponents.push_back(sortedTheta[i]);
//	}
	return pressureComponents;
}

	
double Frame::dBallNearestA(){
	double d{999999};
	for(auto it : players){
		if(it->getTeam()==possession){
			double td{distance(it->getPos(),ball->getPos())};
			if(td<d){
				d=td;
			}
		}
	}
	return d;
}
double Frame::dBallNearestD(){
	double d{999999};
	for(auto it : players){
		if(it->getTeam()!=possession){
			double td{distance(it->getPos(),ball->getPos())};
			if(td<d){
				d=td;
			}
		}
	}
	return d; 
}
double Frame::summedDistance(double radius){
	double summedDistance{0};
	for(auto p : players){
		if(p->getTeam() == possession){
			if(distance(p->getPos(),ball->getPos())<=radius*100){
				double smallestDistance{99999};
				for(auto dp : players){
					if(dp->getTeam()!=possession){
						double d{distance(dp->getPos(),p->getPos())};
						if(d<smallestDistance){
							smallestDistance = d;
						}
					}
				}
				summedDistance += smallestDistance;
			}
		}
	}
	return summedDistance;
}
double Frame::dDA(){
	std::array<double,2> apos;
	std::array<double,2> dpos;
	double minadist = 99999;
	double minddist = 99999;
	for(auto p : players){
		if(p->getTeam()==possession){
			double tdist = distance(p->getPos(),ball->getPos());
			if(tdist<minadist){
				minadist=tdist;
				apos=p->getPos();
			}
		}
		else{
			double tdist = distance(p->getPos(),ball->getPos());
			if(tdist<minddist){
				minddist=tdist;
				dpos=p->getPos();
			}
		}
	}
	return distance(apos,dpos);
}
std::vector<double> Frame::getDToClosestNPlayers(bool attacking){
	std::vector<double> distances;
	for(auto p : players){
		if(p->getTeam()==possession && attacking == true){
			distances.push_back(distance(p->getPos(),ball->getPos()));
		}
		if(p->getTeam()!=possession && attacking == false){
			distances.push_back(distance(p->getPos(),ball->getPos()));
		}
	}
	std::sort(distances.begin(),distances.end());
	return distances;
}
std::vector<std::array<double,2>> Frame::getMDToClosestNPlayers(bool attacking){
	std::vector<std::array<double,2>> distances;
	for(auto p : players){
		if(p->getTeam()==possession && attacking == true){
			double minDist = 999999;
			for(auto q : players){
				if(q->getTeam()!=possession){
					double tDist = distance(p->getPos(),q->getPos());
					if (tDist<minDist){
						minDist=tDist;
					}
				}
			}
			std::array<double,2> temp = {distance(p->getPos(),ball->getPos()),minDist};
			distances.push_back(temp);
		}
		if(p->getTeam()!=possession && attacking == false){
			double minDist = 999999;
			for(auto q : players){
				if(q->getTeam()==possession){
					double tDist = distance(p->getPos(),q->getPos());
					if (tDist<minDist){
						minDist=tDist;
					}
				}
			}
			std::array<double,2> temp = {distance(p->getPos(),ball->getPos()),minDist};
			distances.push_back(temp);
		}
	}
	std::sort(distances.begin(),distances.end(),[](const std::array<double,2>& a, const std::array<double,2>& b){
			return a[0] < b[0];
			});
	return distances;
}

			
double Frame::numberOfPlayersInRadius(double radius, bool attacking){
	double count{0};
	for(auto p : players){
		if(p->getTeam()==possession && attacking == true){
			if(distance(p->getPos(),ball->getPos())<radius){
				count ++;
			}
		}
		if(p->getTeam()!=possession && attacking == false){
			if(distance(p->getPos(),ball->getPos())<radius){
				count ++;
			}
		}
	}
	return count;
}
std::vector<double> Frame::computeScalarCombined(double radius, double markedDist,int number){
	//put ball at (0,0)
	//centred attack
	std::vector<std::array<double,2>> centredAttack;
	std::vector<std::array<double,2>> centredDefense;
	std::array<double,2> ballPos = ball->getPos();
	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		if ((*playerit)->getTeam()==possession){
			if(distance((*playerit)->getPos(),ball->getPos())<radius){
				std::array<double, 2> tpos = (*playerit)->getPos();
				std::array<double,2> pos;
				for(int i = 0;i<2;i++){
					pos[i]=tpos[i]-ballPos[i];
				}
				centredAttack.push_back(pos);
			}
		}
		if ((*playerit)->getTeam()!=possession){
			if(distance((*playerit)->getPos(),ball->getPos())<radius){
				std::array<double, 2> tpos = (*playerit)->getPos();
				std::array<double,2> pos;
				for(int i = 0;i<2;i++){
					pos[i]=tpos[i]-ballPos[i];
					centredDefense.push_back(pos);
				}
			}
		}
	}
	//transfor to polars
	std::vector<std::array<double,2>> rthetasA;
	std::vector<std::array<double,2>> rthetasD;
	for(auto playerit = centredAttack.begin(); playerit < centredAttack.end();++playerit){
		double theta = atan2((*playerit)[1], (*playerit)[0]);
		double r = pow(pow((*playerit)[0],2)+pow((*playerit)[1],2),0.5);
		std::array<double,2> temp = {r,180*theta/3.14 + 180};
		rthetasA.push_back(temp);
	}
	for(auto playerit = centredDefense.begin(); playerit < centredDefense.end();++playerit){
		double theta = atan2((*playerit)[1], (*playerit)[0]);
		double r = pow(pow((*playerit)[0],2)+pow((*playerit)[1],2),0.5);
		std::array<double,2> temp = {r,180*theta/3.14 + 180};
		rthetasD.push_back(temp);
	}
	std::vector<double> minThetas;
	for(auto i : rthetasA){
		double minthet = 180;
		for(auto j : rthetasD){
			if (j[0]<i[0]+markedDist){
				double ttheta = j[1] - i[1];
				
				if(ttheta<0){
					ttheta = ttheta*-1;
				}
				if(ttheta>180){
					ttheta = 360-ttheta;
				}
				if(ttheta < minthet){
					minthet=ttheta;
				}
			}
		}
		minThetas.push_back(minthet);
	}
	std::sort(minThetas.begin(),minThetas.end());
	std::reverse(minThetas.begin(),minThetas.end());
	if(minThetas.size()<number){
		for (int i = 0; i <= number-minThetas.size();i++){
			minThetas.push_back(-1);
		}
	}
	if(minThetas.size()>number){
		minThetas.erase(minThetas.begin()+number,minThetas.end());
	}
	return minThetas;
}




