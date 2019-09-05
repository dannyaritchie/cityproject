#ifndef allclosest
#define allclosest
#include <array>
#include <vector>
#include "../frame.h"
struct closeplayer {
	std::vector<std::array<double,2>> playerPressures;
	void addDist(double opposer_distance);
	findiftro  trolley;
	void addTrolley();
}
struct findiftro {
	std::vector<double> closest;
	void clearAdd(double temporary_distance);
	void keepAdd(double temporary_distance);
	double getDif();
	void passPair();
	//requres external logic of length of closest to use
}
class AllClosest {
	private:
		std::array<closeplayer,28> allPlayers;
		std::vector<Player*> arealPlayers;
		std::vector<Player*> drealPlayers;	
	public:
		void addPlayers(std::vector<Player*> & realPlayers);
		void writeAll();
}
