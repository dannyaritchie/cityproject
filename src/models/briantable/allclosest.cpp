#include "allclosest.h"
void closeplayer::addDistAndChange(std::array<double,2> tpair){
	std::array<double,2> temp;
	temp[0] = trolley.closest[1];
	temp[1] = trolley.getDif();
}
void findiftro::clearAdd(double tdist) {
	closest.clear();
	closest.push_back(tdist);
}
void findiftro::keepAdd(double tdist) {
	closest.erase(closest.begin());
	closest.push_back(tdist);
}
double findiftro::getDif() {
	return closest[0]-closest[2];
} 

void AllClosest::addPlayer(std::vector<Player*> & arealPlayers, std::vector<Player*> & drealPlayers, bool consecutive){
//***
//a member function to add player distances below a set threshhold to a container of 2-arrays
//also creates a differential class
	int prevfid;
	for(frameit = frameStart;frameit!=frameEnd;frameit++){
		if((*frameit)->getFid()==prevfid + 5){

		(*frameit)->getPlayerSplit(arealPlayers, drealPlayers);
		for (auto playerit = arealPlayers.begin() ; playerit < arealPlayers.end();++playerit){
			int pid = (*playerit)->getMappedPid();
			double closestDist = 20;
			for (auto playeritb = drealPlayers.begin();playeritb < drealPlayers.end();++playeritb){
				double tdistance = distance((*playerit)->getPos()[0],(*playeritb)->getPos()[0],(*playerit)->getPos()[1],(*playeritb)->getPos()[1]);
				if(tdisance<closestDist){
					int pidb = (*playeritb)->getMappedPid;
						allPlayers[i].addDist(tdistance);
				}
			}
			(*playerit)->setClosestDist(closestDist);
			(*playerit)->setClosestPlay(closestPlay);
		}
}



