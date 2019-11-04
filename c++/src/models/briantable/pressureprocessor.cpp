#include<algorithm>
#include "pressureprocessor.h"
#include <iostream>
#include <iterator>
#include "../../STATS/runningstats.h"

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
void PressureProcessor::calcPressure(bool write, bool success, bool usuccess){
	if(!write){
		goalDistributions.clear();
		goalDistributions.resize(bins.size());
	}
	if(usuccess){
		for (auto phaseit = uPhases.begin();phaseit<uPhases.end();++phaseit){
			std::array<double,2> summedPressure;
			for(int i = 0;i<2;i++){
				summedPressure[i] = 0;
			}
			int count = (*phaseit).size();
			for (auto pointit = (*phaseit).begin();pointit<(*phaseit).end();++pointit){
				for(int i = 0;i<2;i++){
					summedPressure[i] += (*pointit)[1]/count + i;
				}
				count --;
			}
			if(write){
				for(int i = 0;i<2;i++){
					os[i] << summedPressure[i] << "," << (*phaseit)[(*phaseit).size()-1][3] << ",0" << std::endl;
				}
			}
			else{
				std::array<double,2> info = {summedPressure[0], (*phaseit)[(*phaseit).size()-1][3]};
				getInTheBin(info);
			}
		}
	}
	if(success){
		for (auto phaseit = sPhases.begin();phaseit<sPhases.end();++phaseit){
			std::array<double,2> summedPressure;
			for(int i = 0;i<2;i++){
				summedPressure[i] = 0;
			}
			int count = (*phaseit).size();
			for (auto pointit = (*phaseit).begin();pointit<(*phaseit).end();++pointit){
				for(int i = 0;i<2;i++){
					summedPressure[i] += (*pointit)[1]/count + i;
				}
				count --;
			}
			if(write){
				for(int i = 0;i<2;i++){
					os[i] << summedPressure[i] << "," << (*phaseit)[(*phaseit).size()-1][3] << ",0" << std::endl;
				}
			}
			else{
				std::array<double,2> info = {summedPressure[0], (*phaseit)[(*phaseit).size()-1][3]};
				getInTheBin(info);
			}
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

void PressureProcessor::getInTheBin(std::array<double,2> rubbish){
	if(goalDistributions.size()!=bins.size()){
		goalDistributions.resize(bins.size());
	}
	std::vector<std::vector<double>>::iterator distrit = goalDistributions.begin();
	bool stop{true};
	int count = 0;
	if(bins[1]!=99999999){
		for (auto binit = bins.begin();binit<bins.end();++binit){
			if (rubbish[0]<(*binit)){
				(*distrit).push_back(rubbish[1]);
				return;
			}
			else{++distrit;}
		}
		std::cout << "value higher than top bin" << std::endl;
		return;
	}
	else{distrSorter.push_back(rubbish);}
}

void PressureProcessor::setBins(std::vector<double> tbins){
	bins = tbins;
	return;
}
void PressureProcessor::setBins(){
	bins = {0,99999999};
	return;
}

std::vector<std::array<double,6>> PressureProcessor::getStats(){
	std::vector<double>::iterator binit = bins.begin();
	std::vector<std::array<double,6>> info;
	for (auto distrit = goalDistributions.begin();distrit<goalDistributions.end();++distrit){
		std::cout <<"P <  " << (*binit) << ": ";
		RunningStats statcalc;
		for (auto rubbishit = (*distrit).begin(); rubbishit<(*distrit).end();++rubbishit){
			statcalc.Push((*rubbishit));
		}		
		std::cout << "me: "<< statcalc.Mean() << ",";
		std::cout << "std: " << statcalc.StandardDeviation() << ",";
		std::cout << "kurt: "<< statcalc.Kurtosis() << "," ;
		std::cout << "sk: "<< statcalc.Skewness() << ",";
		std::array<double, 6> tempinfo = {(*binit),(*distrit).size(),statcalc.Mean(),statcalc.StandardDeviation(),statcalc.Kurtosis(),statcalc.Skewness()};
		++binit;
		info.push_back(tempinfo);
	}
	return info;
}

void PressureProcessor::printBinSize(){
	std::vector<double>::iterator binit = bins.begin();
	for (auto it = goalDistributions.begin(); it<goalDistributions.end(); ++it){
		std::cout << "<" << (*binit) << ": " << (*it).size() << std::endl;
		++binit;
	}
	for(auto it = bins.begin();it<bins.end();++it){
		std::cout << (*it) << std::endl;
	}
}
void PressureProcessor::autoBins(int n){
	bins.clear();
	goalDistributions.clear();
	std::sort(distrSorter.begin(),distrSorter.end(),[] (const std::array<double,2> &a, const std::array<double,2> &b){
		return a[0] < b[0];
	});
	int binLength = distrSorter.size()/(1+n);
	std::cout <<"U" << binLength << std::endl;
	for(int i = 1; i < n;i++){
		std::vector<double> binedVec;
		bins.push_back((*(distrSorter.begin()+i*binLength))[0]);
		for(auto it = distrSorter.begin() + (i-1)*binLength;it<distrSorter.begin()+i*binLength;++it){
			binedVec.push_back((*it)[1]);
		}
		goalDistributions.push_back(binedVec);
	}
}	
