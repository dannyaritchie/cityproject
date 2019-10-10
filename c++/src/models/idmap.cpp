#include "idmap.h"
int Idmap::findid(std::array<int,2> playnum) {
	for(int i = 0; i<theMaps.size();i++){
		if (theMaps[i]==playnum){
			return i;
		}
	}
	theMaps.push_back(playnum);
	return theMaps.size()-1;
}
std::string Idmap::getid(int i){
	std::string pid = std::to_string(theMaps[i][1])+ "_" + std::to_string(theMaps[i][0]);
	return pid;
}
int Idmap::getLength(){
	return theMaps.size();
}
