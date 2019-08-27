/*
 * possessionfilemaker.cpp
 *
 *  Created on: 6 Aug 2019
 *      Author: danny
 */
#include<algorithm>
#include"possessionfilemaker.h"
#include"possessionfileline.h"
#include"possessionfile.h"
#include <fstream>
#include<string>
#include<sstream>

void PossessionFileMaker::gameFrameMap(){
    std::string filename = "../idata/matchstartandend.csv";
    std::ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        std::cout << "no matchstart file";
    }
    char charbin;
    int intbin;
    int startFrame;
    int mid;
    std::string line;
    std::getline(inFile, line);
    while(inFile){
        for (int i =0;i<4;i++){
            std::getline(inFile, line);
            if(i==0){
                std::stringstream lineStream(line);
                lineStream>>mid>>charbin>>intbin>>charbin>>startFrame;
                std::array<int,2> temp;
                temp[0] = mid;
                temp[1] = startFrame;
                matchStarts.push_back(temp);
            }
        }
    }
    inFile.close();
    std::cout << matchStarts.size() << std::endl;
}
 


bool PossessionFileMaker::readFile(){
     gameFrameMap();
	std::ifstream inFile;
	inFile.open("../idata/teampossessions.csv");
	if (!inFile) {
			std::cout << "Unable to open possession file";
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
    int matchStart;
    int i=0;
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
                    i++;
                    if (matchStart == -1){
                        std::cout << "Error - could not offset frames for game: " << mid << std::endl;
                    }
                    startFid = startFid - matchStart;
                    endFid = endFid - matchStart;
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
	return true;
}
void PossessionFileMaker::fileWriter(){
	for(auto fileit = possessionFiles.begin(); fileit<possessionFiles.end();++fileit){
		std::string filename = "../data/posdata/" + std::to_string((*fileit)->getMid()) + ".txt";
		std::ofstream os;
		os.open(filename);
		(*fileit)->osLines(os);
		os.close();
	}
}

int PossessionFileMaker::findMatchStart(int mid){
    int matchStart;
    try{
        std::for_each(matchStarts.begin(), matchStarts.end(), [mid, &matchStart](std::array<int,2> temp)
        {
            if(temp[0]==mid){
                matchStart =  temp[1];
                throw 0;
             }
        });
    }
    catch(const int x){
        return matchStart;
    }
    return -1;
}
std::vector<PossessionFile*> PossessionFileMaker::getPosFiles(){
    return possessionFiles
}
