/*
 * possessionfile.h
 *
 *  Created on: 6 Aug 2019
 *      Author: danny
 */

#ifndef POSSESSIONFILE_H_
#define POSSESSIONFILE_H_
#include"possessionfileline.h"
#include<vector>

class PossessionFile{
private:
	std::vector<PossessionFileLine*> lines;
	int mid;
public:
	PossessionFile(int pmid, std::vector<PossessionFileLine*> plines);
	int getMid();
	std::vector<PossessionFileLine*> getPosFilLin();
	void osLines(std::ostream & os);
};




#endif /* POSSESSIONFILE_H_ */
