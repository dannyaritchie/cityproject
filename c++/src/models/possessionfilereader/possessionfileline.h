/*
 * possessionfileline.h
 *
 *  Created on: 6 Aug 2019
 *      Author: danny
 */

#ifndef POSSESSIONFILELINE_H_
#define POSSESSIONFILELINE_H_
#include<iostream>
#include <array>
class PossessionFileLine{
private:
	int tid,startFid,endFid;
public:
	PossessionFileLine(int ptid, int pstartFid, int pendFid);
	void infoToFile(std::ostream & os);
	int getTid();
    std::array<int,2> getFrameRange();
};




#endif /* POSSESSIONFILELINE_H_ */
