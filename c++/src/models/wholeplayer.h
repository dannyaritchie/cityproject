/*
 * wholeplayer.h
 *
 *  Created on: 12 Aug 2019
 *      Author: danny
 */
#include <vector>
#include<array>
#ifndef WHOLEPLAYER_H_
#define WHOLEPLAYER_H_

class WholePlayer{
	private:
	int num, team;
	std::vector<std::array<double,2>> closestDistance;
	public:
	WholePlayer(int pnum, int pteam);
	int getNum();
	int getTeam();
	void addDist(std::array<double,2>);
	std::array<double,2> getClosestDistance(int);
	int tsize(){
		return closestDistance.size();
	}
};



#endif /* WHOLEPLAYER_H_ */
