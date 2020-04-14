/*
 * functions.cpp
 *
 *  Created on: 1 Aug 2019
 *      Author: danny
 */
#include <cmath>
#include "functions.h"
double distance(double xa, double xb, double ya, double yb){
	return pow((pow((xa-xb),2)+pow(ya-yb,2)),0.5);
}
double distance(std::array<double,2> a, std::array<double,2> b){
	return pow((pow((a[0]-b[0]),2)+pow(a[1]-b[1],2)),0.5);
}
double magnitude(std::array<double,2> vec){
	return pow(pow(vec[0],2)+pow(vec[1],2),0.5);
}
double ballDist(double velInitial, double time){
	return velInitial*time;
}
//double ballTime(double velInitial, double distance){

//}
double distanceToMaxVel(double velMax, double accMax){
	return pow(velMax,2)/(2*accMax);
}
std::array<double,2> ballPos(std::array<double,2> initial, std::array<double,2> final,double time,double velInitial){
	double distance = ballDist(velInitial,time);
	double grad = final[0]/final[1];
	std::array<double,2> finalPos;
	finalPos[1] = initial[1]+pow(pow(distance,2)/(1+pow(grad,2)),0.5);
	finalPos[0] = initial[0]+grad*finalPos[1];
	return finalPos;
}
void closestPlayers(std::vector<Player*> & players) {
	for (auto playerita = players.begin(); playerita < players.end();++playerita){
			double closestDist = 1000;
			Player* closestPlay;
			for (auto playeritb = players.begin() ; playeritb < players.end();++playeritb){
				if((*playeritb)->getTeam()!=(*playerita)->getTeam()){
					double tdistance = distance((*playerita)->getPos()[0],(*playeritb)->getPos()[0],(*playerita)->getPos()[1],(*playeritb)->getPos()[1]);
					if(tdistance<closestDist){
						closestDist = tdistance;
						closestPlay = *playeritb;
					}
				}
			}
			(*playerita)->setClosestDist(closestDist);
			(*playerita)->setClosestPlay(closestPlay);
		}
}

void splitByType(std::vector<std::array<int,4>> in, std::vector<std::array<int,2>>& a,std::vector<std::array<int,2>> &b,std::vector<std::array<int,2>>& c){
	for (auto i : in){
		std::array<int,2> temp = {i[0],i[1]};
		if(i[2] == 0){
			a.push_back(temp);
		}
		if(i[2]==1){
			c.push_back(temp);
		}
		if(i[2]==2){
			b.push_back(temp);
		}
	}
	return;
}
			
void splitByTypeAndGroup(std::vector<std::array<int,4>> in, std::array<std::vector<std::array<int,2>>,3>& a,std::array<std::vector<std::array<int,2>>,3> &b){
	for(auto i : in){
		std::array<int,2> temp = {i[0],i[1]};
		if (i[3] = 0){
			if(i[2] == 0){
				a[0].push_back(temp);
			}
			if(i[2]==1){
				a[1].push_back(temp);
			}
			if(i[2]==2){
				a[2].push_back(temp);
			}
		}
		if (i[3] = 1){
			if(i[2] == 0){
				b[0].push_back(temp);
			}
			if(i[2]==1){
				b[1].push_back(temp);
			}
			if(i[2]==2){
				b[2].push_back(temp);
			}
		}
	}
}
std::array<std::array<double,2>,5> findMeanOfBins(std::vector<std::array<double,2>> & pressureBallDist){
	std::sort(pressureBallDist.begin(),pressureBallDist.end(),[] (const std::array<double,2> &a, const std::array<double,2> &b){
		return a[0] < b[0];
	});
	std::array<std::array<double,2>,5> means = {};
	int count{0};
	int binNumber = {1};
	int binCount = 4;
	double binSize = pressureBallDist.size()/binCount;
	for(auto i : pressureBallDist){
//		std::cout << i[0] << std::endl;
		means[0][0] += i[0];
		means[0][1] += i[1];
		if(binNumber<binCount){
			if(count>=binNumber*binSize){
				binNumber ++;
			}
		}
		means[binNumber][0] += i[0];
		means[binNumber][1] += i[1];
		count++;
	}
	for(double i = 0;i<2;i++){
		double x = pressureBallDist.size();
		means[0][i] /= x;
//		std::cout << "AAA" << x <<std::endl;
	}
	//std::cout << binSize << std::endl;
	for(int i = 1;i<4;i++){
		for(int j =0;j<2;j++){
			means[i][j] /= binSize;
		}
	}
	for(int i = 0;i<2;i++){
		double x = pressureBallDist.size()-binSize*(binCount-1);
		means[4][i] /= x;
	//	std::cout << x << std::endl;
	}
	return means;
}
double findMean(std::vector<std::array<double,2>> & pressureBallDist){
	double summedPressure{0};
	for(auto i : pressureBallDist){
		summedPressure +=i[0];
	}
	double size = pressureBallDist.size();
	//std::cout << summedPressure << std::endl;
	return summedPressure/size;
}
				
/*double timePlayerToBall(std::array<double,2> initial, std::array<double,2> final,std::array<double,2> velInitial,double velMax, double accMax,double deaccMax){
	std::array<double,2> stopPos;
	double velInitialMag = pow(pow(velInitial[0],2)+pow(velInitial[1],2),0.5);
	double stopDistance = pow(velInitialMag,2)/(2*deaccMax);
	double stopTime = magnitude(velInitial)/deaccMax;
	double grad = velInitial[0]/velInitial[1];
	stopPos[1] = initial[1]+pow(pow(stopDistance,2)/(1+pow(grad,2)),0.5);
	stopPos[0] = initial[0]+grad*stopPos[1];
	double ballDistance = distance(stopPos[0],stopPos[1],final[0],final[1]);
	if(distance>distanceToMaxVel){
		return (distanceToMaxVel(velMax,accMax)-ballDistance)/velMax + velMax/accMax;
	}
	else{return pow(2*ballDistance/accMax,0.5);


}
double playerDist(std::array<double,2> ballStart, std::array<double, 2> ballCurrent, std::array<double,2> playerStart, double time, double velInitial){

}*/



