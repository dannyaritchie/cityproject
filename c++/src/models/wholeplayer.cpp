#include "wholeplayer.h"

WholePlayer::WholePlayer(int pnum, int pteam): num{pnum}, team{pteam}{};
int WholePlayer::getNum(){
	return num;
}
int WholePlayer::getTeam(){
	return team;
}
void WholePlayer::addDist(std::array<double,2> tdist){
	closestDistance.push_back(tdist);
}
std::array<double,2> WholePlayer::getClosestDistance(int i){
	return closestDistance.at(i);
}
