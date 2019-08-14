/*
 * possessionfilemaker.cpp
 *
 *  Created on: 6 Aug 2019
 *      Author: danny
 */

#include"possessionfilemaker.h"
#include"possessionfileline.h"
#include"possessionfile.h"
#include <fstream>
#include<string>
#include<sstream>
void PossessionFileMaker::fileLoader(){
	std::ifstream inFile;
	inFile.open("teampossessions.csv");
	if (!inFile) {
			std::cout << "Unable to open file";
			exit(1);// terminate with error
		}
	std::string bin;
	std::getline(inFile, bin);
	char charbin;
	int intbin;
	int prevMid{0};
	int saveMid;
	PossessionFileLine * lineSave;
	bool lineSaveBool{true};
	while(inFile){
		bool sameMid{true};
		std::vector<PossessionFileLine*> tlines;
		while(sameMid){
			if(!inFile){
				sameMid = false;
			}
			else{
				int mid, tid, startFid, endFid;
				PossessionFileLine * tpossessionFileLine;
				if(!lineSaveBool){
					tpossessionFileLine = lineSave;
					mid = prevMid;
					lineSaveBool = true;
				}
				else{
					std::string pline;
					std::getline(inFile,pline);
					std::stringstream sline(pline);
					sline >> mid >> charbin >> intbin >> charbin >> intbin >> charbin >> intbin;
					sline >> charbin >> tid >> charbin >> startFid >> charbin >>endFid;
					PossessionFileLine * ttpossessionFileLine = new PossessionFileLine(tid,startFid,endFid);
					tpossessionFileLine = ttpossessionFileLine;
				}
				if (mid != prevMid) {
					sameMid = false;
					saveMid = prevMid;
					prevMid = mid;
					lineSave = tpossessionFileLine;
					lineSaveBool = false;
				}
				else{
					tlines.push_back(tpossessionFileLine);
					sameMid = true;
				}
			}
		}
		if (saveMid!=0){
			PossessionFile * tpossessionFile = new PossessionFile(saveMid,tlines);
			possessionFiles.push_back(tpossessionFile);
			//possessionFiles.push_back(tpossessionFile);
		}
	}

}
void PossessionFileMaker::fileWriter(){
	for(auto fileit = possessionFiles.begin(); fileit<possessionFiles.end();++fileit){
		std::string filename = "posdata/" + std::to_string((*fileit)->getMid()) + ".txt";
		std::cout << filename << std::endl;
		std::ofstream os;
		os.open(filename);
		(*fileit)->osLines(os);
		os.close();
	}
}

