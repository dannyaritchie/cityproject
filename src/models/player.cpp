#include  "player.h"
/*Player::Player(int pnum, double px, double py, int phome) : home(phome) {
	x = px;
	num = pnum;
	y = py;
	velocityBool = false;
	angControlScore = 0;
};*/
Player::Player(int pnum, double px, double py, int phome, int pmnum) : home(phome) {
	x = px;
	num = pnum;
	y = py;
	velocityBool = false;
	angControlScore = 0;
	mnum = pmnum;
	velocity = {0,0};
};

int Player::getTeam(){
	return home;
}

void Player::infoToText(std::ostream & os,int type) {
	switch(type){
	case 0:
		os << "\t" << num<< "\t" << home << "\t" << x << "\t" <<  y << "\n";
		break;
	case 1:
		os << "\t" << num<< "\t" << home << "\t" << velocity[0] << "\t" <<  velocity[1] << "\n";
	}
}
int Player::getNum(){
	return num;
}
void Player::setClosestPlay(Player* tplayer){
	closestPlay = tplayer;
}
void Player::setClosestDist(double tdist){
	closestDist = tdist;
}
Player* Player::getClosestPlay(){
	return closestPlay;
}
const double& Player::getClosestDist(){
	return closestDist;
}
int Player::getMappedPid(){
	return mnum;
}
void Player::setBallCentredPolar(std::array<double,2> tpol){
	ballCentredPolar = tpol;
}
std::array<double,2> Player::getBallCentredPolar(){
	return ballCentredPolar;
}
void Player::plusAngScore(){
	angControlScore+=1;
}
int Player::getAngScore(){
	return angControlScore;
}
