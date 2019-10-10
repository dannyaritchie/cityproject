#include "pressureprocessor.h"

PressureProcessor::PressureProcessor(){
	previousFid = -1;
}
void PressureProcessor::addPressure(std::array<double,2> info){
	if(info[0] != previousFid+1){
		sections.push_back(section);
		section.clear();
		std::array<double,2> temp;
		for (int i = 1; i < 2; i++){
			temp[i-1] = info [i];
		}
		temp[1] = 0;
		section.push_back(temp);
		previousFid = info[0];
	}
	else{
		std::array<double,2> temp;
		for (int i = 1; i < 2; i++){
			temp[i-1] = info [i];
		}
		temp[1] = 0;
		section.push_back(temp);
		previousFid = info[0];
	}
}
int PressureProcessor::lengthThreshold(double timeLength, bool increasing, bool savePhases){
	int prevPressure = 0;
	int consecutive = 0;
	int number = 0;
	bool largeEnough = false;
	if(increasing){
		for (auto sectionsit = sections.begin();sectionsit<sections.end();++sectionsit){
			for(auto sectionit = (*sectionsit).begin();sectionit<(*sectionsit).end(); ++sectionit){
				if((*sectionit)[0] > prevPressure) {
					consecutive += 1;
				}
				else{
					if(largeEnough == true){
						number += 1;
						if(savePhases == true){
							std::vector<array<double,2> tphase;
							for(auto dit = sectionit-1-consecutive;dit<sectionit;++dit){
								tphase.push_back(*sectionit);
							}
							phases.push_back(tphase);
						}
						largeEnough = false;
					}
					consecutive = 0;
				}
				if(consecutive >= timeLengh*5){
					largeEnough = true;
				}
				prevPressure = (*sectionit)[0];
			}
		}
		return number;
	}
	else{
		for (auto sectionsit = sections.begin();sectionsit<sections.end();++sectionsit){
			for(auto sectionit = (*sectionsit).begin();sectionit<(*sectionsit).end(); ++sectionit){
				if((*sectionit)[0] < prevPressure) {
					consecutive += 1;
				}
				else{
					if(largeEnough == true){
						number += 1;
						if(savePhases == true){
							std::vector<array<double,2> tphase;
							for(auto dit = sectionit-1-consecutive;dit<sectionit;++dit){
								tphase.push_back(*sectionit);
							}
							phases.push_back(tphase);
						}
						largeEnough = false;
					}
					consecutive = 0;
				}
				if(consecutive >= timeLengh*5){
					largeEnough = true;
				}
				prevPressure = (*sectionit)[0];
			}
		}
		return number;
	}
}
int PressureProcessor::addRates(){
	for(auto phaseit = phases.begin();phaseit<phases.end();++phaseit){
		std::vector<std::array<double,u>>::iterator start = (*phaseit).begin();
		std::vector<std::array<double,u>>::iterator mid = (*phaseit).begin() + 1;
		std::vector<std::array<double,u>>::iterator end = (*phaseit).begin() + 2;
		(*phaseit)[0][1] = 0;
		(*phaseit)[(*phaseit).end() - 1][1] = 0
		while(end!=(*phaseit).end()){
			(*mid)[1] = (*end)[0] - *start[0];
			std::advance(start,1);
			std::advance(mid,1);
			std::advance(end,1);
		}
	}
}

