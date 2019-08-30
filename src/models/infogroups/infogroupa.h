#ifndef INFOGROUPA_H_
#define INFOGROUPA_H_
#include <vector>
#include <iterator>
#include<array>
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include "../possessionfilereader/possessionfilemaker.h"
#include "../game.h"
#include "../../helpers/functions.h"
class InfoGroupA {
	private:
		std::array<std::array<std::vector<std::array<double,3>>,70>,2> dtdata;
		std::array<std::array<std::vector<std::array<double,6>>,70>,2> playerstats;
	public:
		bool loadData();
		void write(int mid);
		double velocity (int team, int num);
		double closestVelocity(int team, int num);
		void closestPlayers(std::vector<Player*> & players);
		int teamPossession(std::vector<Player*> & players, Ball * ball);
		void computeScalar(std::vector<Player*>&,Ball*,int);
};
#endif
