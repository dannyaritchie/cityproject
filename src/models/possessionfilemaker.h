/*
 * possessionfilemaker.h
 *
 *  Created on: 6 Aug 2019
 *      Author: danny
 */

#ifndef POSSESSIONFILEMAKER_H_
#define POSSESSIONFILEMAKER_H_

#include <iostream>
#include<vector>
#include"possessionfile.h"
class PossessionFileMaker{

private:
	std::vector<PossessionFile*> possessionFiles;
	//class to turn possesion fil for every game into seperate files for each game
public:
	void fileLoader();
	void fileWriter();
};



#endif /* POSSESSIONFILEMAKER_H_ */
