/*
 * possessionfileline.cpp
 *
 *  Created on: 6 Aug 2019
 *      Author: danny
 */

#include"possessionfileline.h"

PossessionFileLine::PossessionFileLine(int ptid, int pstartFid, int pendFid) : tid(ptid), startFid(pstartFid),endFid(pendFid){}

void PossessionFileLine::infoToFile(std::ostream & os){
	os << tid << "\t" << startFid << "\t" << endFid << std::endl;
}

int PossessionFileLine::getTid(){
	return tid;
}

std::array<int,2> PossessionFileLine::getFrameRange(){
    std::array<int,2> frameRange = {startFid, endFid};
    return frameRange;
}
