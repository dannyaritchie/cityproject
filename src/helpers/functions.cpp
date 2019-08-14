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



