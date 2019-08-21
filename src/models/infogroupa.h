#ifndef INFOGROUPA_H_
#define INFOGROUPA_H_
#include <vector>
#include <iterator>
#include<array>
#include "../possessionfilereader/possessionfilemaker.h"
#include "../game.h"
#include "../../helpers/functions.h"
class InfoGroupA{
	private:
		std::array<std::array<std::vector<std::array<double,3>,35>,2> dtdata;
		std::array<std::array<std::vector<std::array<double,4>>,35>,2> playerstats;
	public:
		void adddt(int, std::array<double,3>);

#endif
