#ifndef allclosest
#define allclosest
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include "../frame.h"
#include "../idmap.h"
struct findiftro {
	std::array<std::vector<double>,28> closest;
	std::array<int,28> consecutive;
	void addDist(double temporary_distance, int pid);
	std::array<double,2> passPair(int pid);
	//requres external logic of length of closest to use
};
struct frameInfo {
	int fid;
	int attacking;
	std::vector<std::array<double,2>> framePlayerPressures;
};
struct closeplayer {
	std::ofstream playerPressures;
	findiftro  trolley;
	frameInfo tempInfo;
	void addDist(double opposer_distance,int pid);
	std::array<double,2> pullPair(int pid);
	std::array<double,2> pullWritePassPair(int pid);
	void writePair(std::array<double,2> pair);
};
class AllClosest {
	private:
		std::array<closeplayer,28> allPlayers;
		std::vector<Player*> homePlayers;
		std::vector<Player*> awayPlayers;	
	public:
		void addPlayers(std::vector<Frame*>::iterator currentFrameit,int previousFid, int prevAttackingTeamid);
		void closeStreams();
		void openStreams(int matchid, Idmap id_mappings);
};
#endif
