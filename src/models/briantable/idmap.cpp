#include "idmap.h"

int Idmap::findid(std::array<double,2> playnum) {
	for(int i = 0; i<theMaps.size();i++){
		if (theMaps[i]==playnum){
			return i;
		}
	}
	return -1;
}

void Idmap::addid(std::array<double,2> playnum){
	theMaps.push_back(playnum);
}
