/*
 * functions.h
 *
 *  Created on: 1 Aug 2019
 *      Author: danny
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <array>
#include<vector>
#include"../models/game.h"
double distance(double xa,double xb,double,double);
//returns distance between two points
double magnitude(std::array<double,2> vec);
//returns magnitude of given vector
double ballDist(double velInitial, double time);
//returns distance a ball will travel as function of time
double ballTime(double velInitial, double distance);
//assumes constant velocity
double distanceToMaxVel(double velMax, double accMax);
std::array<double,2> ballPos(std::array<double,2> initial, std::array<double,2> final,double time,double velInitial);
void closestPlayers(std::vector<Player*> & player);
//returns position of ball after a time t given its end point, start point, initial velocity.
//uses ballDist
/*double timePlayerToBall(std::array<double,2> initial, std::array<double,2> final,double velInitial,double velMax, double accMax,double deaccMax);
//returns time for a player to get to a position given startpoint, endpoint,initial velocity, max velocity and max acceleration
double playerDist(std::array<double,2> ballStart, std::array<double,2> ballEnd, std::array<double,2> playerStart, double time, double velInitial);
*/
#endif /* FUNCTIONS_H_ */
