#include "pressureprocessor.h"
#include <iostream>
#include <iterator>

PressureProcessor::PressureProcessor(){
	previousFid = -1;
}
void PressureProcessor::addPressure(std::array<double,4> info){
	if(info[0] != previousFid+1){
		sections.push_back(section);
		section.clear();
		std::array<double,4> temp;
		for (int i = 1; i < 4; i++){
			temp[i] = info [i];
		}
		temp[0] = 0;
		section.push_back(temp);
		previousFid = info[0];
	}
	else{
		std::array<double,4> temp;
		for (int i = 1; i < 4; i++){
			temp[i] = info [i];
		}
		temp[0] = 0;
		section.push_back(temp);
		previousFid = info[0];
	}
}
void PressureProcessor::addFinalPressure(){
	sections.push_back(section);
}
std::array<int,2> PressureProcessor::lengthThreshold(double timeLength, double size, bool increasing, bool savePhases, bool lookieLookie){
	int prevPressure = 0;
	int prevPrevPressure = 0;
	int prevAttacking = -1;
	int consecutive = 0;
	std::array<int,2> number = {0,0};
	bool largeEnough = false;
	bool possessionChange = false;
	if(increasing){
		for (auto sectionsit = sections.begin();sectionsit<sections.end();++sectionsit){
			for(auto sectionit = (*sectionsit).begin();sectionit<(*sectionsit).end(); ++sectionit){
				if(lookieLookie == true){
					prevPressure = std::min(prevPressure,prevPrevPressure);
				}
				if((*sectionit)[1] > prevPressure&&(*sectionit)[2]==prevAttacking) {
					consecutive += 1;
				}
				else{
					if((*sectionit)[1]>prevPressure&&largeEnough == true){
						number[1] += 1;
						if(savePhases == true){
							std::vector<std::array<double,4>> tphase;
							for(auto dit = sectionit-1-consecutive;dit<sectionit;++dit){
								tphase.push_back(*sectionit);
							}
							sPhases.push_back(tphase);
						}
						largeEnough = false;
					}
					if((*sectionit)[1]<=prevPressure && largeEnough == true){
						number[0] += 1;
						if(savePhases == true){
							std::vector<std::array<double,4>> tphase;
							for(auto dit = sectionit-1-consecutive;dit<sectionit;++dit){
								tphase.push_back(*sectionit);
							}
							uPhases.push_back(tphase);
						}
						largeEnough = false;
					}
					consecutive = 0;
				}
				if(consecutive >= timeLength*5&&(*sectionit)[1]>size){
					largeEnough = true;
				}
				prevPressure = (*sectionit)[1];
				prevPrevPressure = prevPressure;
				prevAttacking = (*sectionit)[2];

			}
		}
		return number;
	}
/*	else{
		for (auto sectionsit = sections.begin();sectionsit<sections.end();++sectionsit){
			for(auto sectionit = (*sectionsit).begin();sectionit<(*sectionsit).end(); ++sectionit){
				if((*sectionit)[0] < prevPressure) {
					consecutive += 1;
				}
				else{
					if(largeEnough == true){
						number += 1;
						if(savePhases == true){
							std::vector<std::array<double,2>> tphase;
							for(auto dit = sectionit-1-consecutive;dit<sectionit;++dit){
								tphase.push_back(*sectionit);
							}
							phases.push_back(tphase);
						}
						largeEnough = false;
					}
					consecutive = 0;
				}
				if(consecutive >= timeLength*5&&(*sectionit)[0]<size){
					largeEnough = true;
				}
				prevPressure = (*sectionit)[0];
			}
		}
		return number;
	}*/
}
void PressureProcessor::addRates(){
	for(auto phaseit = sPhases.begin();phaseit<sPhases.end();++phaseit){
		std::vector<std::array<double,4>>::iterator start = (*phaseit).begin();
		std::vector<std::array<double,4>>::iterator mid = (*phaseit).begin() + 1;
		std::vector<std::array<double,4>>::iterator end = (*phaseit).begin() + 2;
		(*phaseit)[0][0] = 0;
		(*phaseit)[(*phaseit).size() - 1][0] = 0;
		while(end!=(*phaseit).end()){
			(*mid)[0] = (*end)[1] - (*start)[1];
			std::advance(start,1);
			std::advance(mid,1);
			std::advance(end,1);
		}
	}
}
void PressureProcessor::calcPressure(){
	for (auto phaseit = sPhases.begin();phaseit<sPhases.end();++phaseit){
		std::array<double,10> summedPressure;
		for(int i = 0;i<10;i++){
			summedPressure[i] = 0;
		}
		int count = (*phaseit).size();
		for (auto pointit = (*phaseit).begin();pointit<(*phaseit).end();++pointit){
			for(int i = 0;i<10;i++){
				summedPressure[i] += (*pointit)[1]/count + i;
			}
			count --;
		}
		for(int i = 0;i<10;i++){
			os[i] << summedPressure[i] << "," << (*phaseit)[(*phaseit).size()-1][3] << std::endl;
		}
	}
}
void PressureProcessor::openStreams(){
	for (int i = 0;i<10;i++){
		os[i].open("../data/phasepressure/distancetogoal_" + std::to_string(i) + "_.txt");
	}
}
void PressureProcessor::closeStreams(){
	for (int i = 0;i<10;i++){
		os[i].close();
	}
}
void PressureProcessor::clearPhases(){
	sPhases.clear();
	uPhases.clear();
	sections.clear();
	section.clear();	
}
