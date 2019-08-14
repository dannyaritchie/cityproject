/*
 * possessionfileline.h
 *
 *  Created on: 6 Aug 2019
 *      Author: danny
 */

#ifndef POSSESSIONFILELINE_H_
#define POSSESSIONFILELINE_H_
#include<iostream>

class PossessionFileLine{
private:
	int tid,startFid,endFid;
public:
	PossessionFileLine(int ptid, int pstartFid, int pendFid);
	void infoToFile(std::ostream & os);
	int getTid();
};




#endif /* POSSESSIONFILELINE_H_ */
