#ifndef INFOGROUPA_H_
#define INFOGROUPA_H_
#include <vector>
#include <iterator>
#include<array>
#include<iostream>
#include<string>
#include "../possessionfilereader/possessionfilemaker.h"
#include "../game.h"
#include "../../helpers/functions.h"
class InfoGroupA{
	private:
		std::array<std::array<std::vector<std::array<double,3>,35>,2> dtdata;
		std::array<std::array<std::vector<std::array<double,5>>,35>,2> playerstats;
	public:
		void loadData(int, std::array<double,3>);
		void write(int mid);
		double velocity (int team, int num)
		double closestVelocity(int team, int num)
		void closestPlayers(std::vector<Player*> & players);

#endif
