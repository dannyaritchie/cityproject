#include "models/game.h"
#include "models/briantable/allclosest.h"
#include "models/briantable/pressureprocessor.h"
#include "helpers/functions.h"
#include <iterator>
#include<fstream>
#include <vector>
#include<algorithm>
#include <array>
#include<cmath>


int main(){
	/*PARAMETER SETTING (ALL PARAMETERS EXPLAINED IN BODY)
	double goal_pow = atoi(arg[1]);
	double ball_pow = 	atoi(arg[2]);
	double dist_weight= 	atoi(arg[3]);
	double dist_pow =  	atoi(arg[4]);
	double vel_pow =  	atoi(arg[5]);
	double distvel_pow =  	atoi(arg[6]);
	double ranking =  	atoi(arg[7]);
	double minBallFromDefGoal =  	atoi(arg[8]);
	double maxBallFromDefGoal = 	atoi(arg[9]);
	double pressureLimit= 	atoi(arg[10]);
	double timeLimit =	atoi(arg[11]);
	double lookieLookie =	atoi(arg[12]);
	double pos_time_limit =	atoi(arg[13]);
	double frame_weighting =	atoi(arg[14]);
	double change_in_dtog =	atoi(arg[15]);
	double number_of_bins =	atoi(arg[16]);
	double distThreshold  =	atoi(arg[17]);
	double numberOfGames = atoi(arg[18]);
	std::string filePath = arg[19];
	double start_looking_distance = atoi(arg[20]);
	double looking_length = atoi(arg[21]);
	double defensive_player_velocity = atoi(arg[22]);
	double angle_threshold = 45;
	*/
	double goal_pow = 0;
	double ball_pow = 0;
	double dist_weight= 1;
	double dist_pow =  1;
	double vel_pow = 1;
	double distvel_pow = 1;
	double ranking = 0;
	double minBallFromDefGoal =  1000;
	double maxBallFromDefGoal = 9000;
	double pressureLimit= 	0;
	double timeLimit = 0.2;
	double lookieLookie = 3;
	double pos_time_limit =	5;
	double frame_weighting = 0;
	double change_in_dtog =	0;
	double number_of_bins =	3;
	double distThreshold  =	15;
	std::string filePath = "../data/temp/EHud";
	double start_looking_distance = 0;
	double looking_length = 0;
	double defensive_player_velocity = 0;
	double angle_threshold = 45;
	std::ofstream results;
	//number o phases graph
	std::ofstream numberofphasesos;
	numberofphasesos.open("../data/numberofphases.txt");
	
	std::array<double,2> parameters = {vel_pow, distvel_pow};
	//object to store information of number of phases as parameters are varied - used in numberPositionAnalysis
	std::vector<std::vector<std::vector<std::vector<std::array<int,3>>>>> allNumbers	
	//flags
	bool useGroups{false};
	bool analyse{false}; //whether to get information about phase
	bool results{false};	//whether to get results from phases
	bool numberPositionAnalysis{true} //whether number and position of phases should be analysed
	
	int minDefNum = 4;
	double minDefVel = 1;
	double ballRadius = 2000;
	int startLookingDistance = 15;
	int lookingLength = 24;
	int minFrames = 15;
	int maxPostPressTime =25;
	double playerRadius = 1500;
	int numberOfGames = 15;
	double closePressure = 0.3;
	std::string dataDestination = "../data/newdata/numbers/";
	//CREATE 2017 GAME VECTOR
	std::vector<int> midsb;
	for (auto i = 918893;i<919271;++i){
			midsb.push_back(i);
	}
	std::vector<int> bad2017games = {919068,919059,919050,919187,918899,918902,918913,918922,918967,918901, 918928,918962,918997}; // not sure why bad, error is usually that a player number was not found (i.e) a player presumed to be on pitchwas no longeron pitch. I guess this is possible from a substitution however research into 918901 found a frame where there was only 1 player.
	//though substitution does seem very unlikely as itshould be handled
	for (int i : bad2017games){
		midsb.erase(std::remove(midsb.begin(), midsb.end(), i), midsb.end());
	}
	//
	//open data files (no groups)	
//	if(!useGroups){
		std::ofstream noPossessionChangeO;	
		std::ofstream possessionChangeO;	
		std::ofstream frameJumpO;	
		noPossessionChangeO.open(dataDestination + "nopossessionchange.txt");
		possessionChangeO.open(dataDestination + "possessionchange.txt");
		frameJumpO.open(dataDestination + "framejump.txt");
//	}
	//open data files (groups)
//	else{
		std::ofstream noPossessionChangeGa;	
		std::ofstream possessionChangeGa;	
		std::ofstream frameJumpGa;	
		std::ofstream noPossessionChangeGb;	
		std::ofstream possessionChangeGb;	
		std::ofstream frameJumpGb;	
		std::ofstream noPossessionChangeGc;	
		std::ofstream possessionChangeGc;	
		std::ofstream frameJumpGc;	
		noPossessionChangeGa.open(dataDestination + "nopossessionchangeGa.txt");
		possessionChangeGa.open(dataDestination + "possessionchangeGa.txt");
		frameJumpGa.open(dataDestination + "framejumpGa.txt");
		noPossessionChangeGb.open(dataDestination + "nopossessionchangeGb.txt");
		possessionChangeGb.open(dataDestination + "possessionchangeGb.txt");
		frameJumpGb.open(dataDestination + "framejumpGb.txt");
		noPossessionChangeGc.open(dataDestination + "nopossessionchangeGc.txt");
		possessionChangeGc.open(dataDestination + "possessionchangeGc.txt");
		frameJumpGc.open(dataDestination + "framejumpGc.txt");
//	}
	//open results file
	std::ofstream usedParametersAndResults;
	usedParametersAndResults.open(dataDestination + "usedParametersAndResults.txt");
	//
	//Load game
	std::string rempathb= "/pc2014-data1/lah/data_msgpk_031219/2017/PremierLeague/";
	//
	//set which teams we are interested int
	std::vector<int> teamIDs = {3,13,36,43,8,90,31,38,11,110,1,21,4,6,20,80,57,14,35,91};//for multiple groups this should be all teams
	//
	//define counters for amount of phase (no groups)
//	if(!useGroups){
		int numberOfNormalPhases{0};
		int numberOfPossessionChangePhases{0};
		int numberOfFrameJumpPhases{0};
//	}
	//
	// or define counters for number of phases for each group
	// 3 element is team id
	// 0 element is normal phase count
	// 1 element is posssession change count
	// 2 element is frame jump count
//	else{
		std::vector<std::vector<int>> groups;
	//	for(auto i : teamIDs){
	//		std::vector<int> t = {i};
	//		groups.push_back(t);
	//	}
		std::vector<int> groupa = {3,6,1,14,43,8}; //High
		std::vector<int> groupb = {4,90,31,11,21,91,13}; //medium
		std::vector<int> groupc = {20,80,57,35,38,110,36}; //low
		groups.push_back(groupa);
		groups.push_back(groupb);
		groups.push_back(groupc);
		std::vector<std::array<int,4>> groupPhaseSizeTime;//holds howmmay of each phase a group has as well as total frames they defending
		groupPhaseSizeTime.resize(groups.size());
		for(auto i : groupPhaseSizeTime){
			for(auto j : i){
			j = 0;
			}
		}
//	}
	//
	
	//define storage for phase pressure (no groups)
//	if(!useGroups){
		std::vector<std::array<double,2>> pressureBallDist;
		std::vector<std::array<double,2>> pressureBallDistPosChange;
		std::vector<std::array<double,2>> pressureBallDistFrameJump;
//	}
	//
	//or in case of groups
//	else{
		std::vector<std::array<std::vector<std::array<double,2>>,3>> groupPhasePressureBallDist;
		groupPhasePressureBallDist.resize(groups.size());
//	}
	//

	
	for(int i = 0; i <numberOfGames;i++){ //for userset number of games
	//for(int i = 0;i < midsb.size();i++){ //for all games
		std::cout << "Match ID: " << midsb[i] << std::endl;
		Game * tgame = new Game(midsb[i], rempathb);
		//
		//Choose teams that are interesting	
		int homeID{-1}, awayID{-1};
		//
		//TESTA-Used for finding number of players in front ofi ball
		//std::array<std::array<int,12>,100> thresholds;
		//
		if(tgame->readNewFile(teamIDs, awayID, homeID)==true){//sets awayID and homeID to teams that re playing if they are in teamIDs
			//load playerpair info
			tgame->setFramesPlayersDistance(playerRadius);
			tgame->setFramesPlayersVelocity();
			tgame->setAllGoalPos();
			tgame->addVelocities();
			if(analyse){
		//		PHASE CHOICE
		//all methods return a vector of 3-arrays of doubles specifying start and length of phase, and type of phase
		//
		//Method 1
		//	When minDefNum  defenders running away at least 90 degrees from the goal with a positive velocity above minDefVel for more than minFrames frames and there has been no change in possession or frame jump a phase is defined. up untill maxPostPressTime frames are then looked at after this, if there is a possessionchange phase is given type 2, if there is a frame jump it is given type 1 and otherwise it is give type 0;
				std::vector<std::array<int,4>> frameStartLengthType;
				frameStartLengthType = tgame->getPhases(minDefNum,minDefVel,minFrames,maxPostPressTime); //METHOD 1
		//Method 2
		//uses the old method of calculating the pressure and selecting phases based on increasing pressure. to keep with compatability this caclulates pressure for every fram but still only returns phase information, although work is alrady done for getPhaseInformationOld as it just has to read of pressure
		//Details of old method
		//pressure is 1/(1+distance to ball^a) * (10^b*1/(1+distance betwen players^c) + distance dot^d/(1+distance^e))
		//		frameStartLength = tgame->getPhasesOld(); //Method 2
				
				//split frameStartLengthType into 3 vectors by type
				std::vector<std::array<int,2>> noPossessionChangePhases;
				std::vector<std::array<int,2>> possessionChangePhases;
				std::vector<std::array<int,2>> frameJumpPhases;
				//in the case we are not cared about groups
				std::array<std::vector<std::array<int,2>>,3> homePhases; //for groups but declaration must be outside of if/else statemet
				std::array<std::vector<std::array<int,2>>,3> awayPhases; //"" ""
				if(!useGroups){
					splitByType(frameStartLengthType,noPossessionChangePhases,possessionChangePhases,frameJumpPhases);		
					std::cout << "number Of Normal Phases:  " <<  noPossessionChangePhases.size() << std::endl;
					std::cout << "number Of Possession Change Phases: " << possessionChangePhases.size() << std::endl;
					std::cout << "number Of Frame Jump Phases: " << frameJumpPhases.size() << std::endl;
					numberOfNormalPhases += noPossessionChangePhases.size();
					numberOfPossessionChangePhases += possessionChangePhases.size();
					numberOfFrameJumpPhases += frameJumpPhases.size();
				}
				//in the case we are cared about specific groups
				else{
					std::array<int,2> possessionTimes = tgame->getPossessionTimes(); // gets the number of frames home and away team are in possession of the ball
					splitByTypeAndGroup(frameStartLengthType, awayPhases,homePhases);	
					std::cout << "number Of Normal Phases:  " <<  awayPhases[0].size()+homePhases[0].size() << std::endl;
					std::cout << "number Of Possession Change Phases: " << awayPhases[1].size()+homePhases[1].size() << std::endl;
					std::cout << "number Of Frame Jump Phases: " << awayPhases[2].size()+homePhases[2].size() << std::endl;
					bool foundGroup{false};
					for (int j = 0;j<groups.size();j++){
						if(!foundGroup){
							std::vector<int>::iterator it = std::find(groups[j].begin(),groups[j].end(),awayID);
							if(it!=groups[j].end()){
								foundGroup = true;
								groupPhaseSizeTime[j][0] += awayPhases[0].size();
								groupPhaseSizeTime[j][1] += awayPhases[1].size();
								groupPhaseSizeTime[j][2] += awayPhases[2].size();
								groupPhaseSizeTime[j][3] += possessionTimes[0];
							}
						}
					}
					foundGroup = false;
					for (int j = 0;j<groups.size();j++){
						if(!foundGroup){
							std::vector<int>::iterator it = std::find(groups[j].begin(),groups[j].end(),homeID);
							if(it!=groups[j].end()){
								foundGroup = true;
								groupPhaseSizeTime[j][0] += homePhases[0].size();
								groupPhaseSizeTime[j][1] += homePhases[1].size();
								groupPhaseSizeTime[j][2] += homePhases[2].size();
								groupPhaseSizeTime[j][3] += possessionTimes[1];
							}
						}
					}
				}
				//

				/*Getting frame information
				 * method returns a vector of two arrays that are the pressure and thechange in distance between the goal and the ball
				 * change in distance is calculated as from startLookingDistance after the beginning of the frame and lookingLength after that
				 * pressure is calculated for the frame at startLookingDistance as follows. find the shortest 'time' for players in ballRadius of ball and sum it for the longest numberOfAttackers. multiply this by shortest time for player closest to ball and return 1/ this for pressure
				 * MAKE SURE startLookingDistance + lookingLength < minFrames.
				 */
				//(no groups)
				if(!useGroups){
					std::vector<std::array<double,2>> temp;
					temp = tgame->getPhaseInformation(noPossessionChangePhases,startLookingDistance,lookingLength, ballRadius,0, closePressure, parameters);
					pressureBallDist.insert(pressureBallDist.begin(),temp.begin(),temp.end()); 
					temp = tgame->getPhaseInformation(possessionChangePhases,startLookingDistance,lookingLength, ballRadius,1, closePressure, parameters);
					pressureBallDistPosChange.insert(pressureBallDistPosChange.begin(),temp.begin(),temp.end()); 
					temp = tgame->getPhaseInformation(frameJumpPhases,startLookingDistance,lookingLength, ballRadius,2, closePressure, parameters);
					pressureBallDistFrameJump.insert(pressureBallDistFrameJump.begin(),temp.begin(),temp.end()); 
				}
				//
				//In case where we have groups
				else{
					std::array<std::vector<std::array<double,2>>,3> homePhasesPressureBallDist;
					std::array<std::vector<std::array<double,2>>,3> awayPhasesPressureBallDist;
					for(int i = 0; i < 3; i++){
						homePhasesPressureBallDist[i] = tgame->getPhaseInformation(homePhases[i],startLookingDistance,lookingLength,ballRadius,i, closePressure, parameters);
						awayPhasesPressureBallDist[i] = tgame->getPhaseInformation(awayPhases[i],startLookingDistance,lookingLength,ballRadius,i, closePressure, parameters);
					}
					bool foundGroupB{false};
					for (int j = 0;j<groups.size();j++){
						if(!foundGroupB){
							std::vector<int>::iterator it = std::find(groups[j].begin(),groups[j].end(),awayID);
							if(it!=groups[j].end()){
								foundGroupB = true;
								groupPhasePressureBallDist[j][0].insert(groupPhasePressureBallDist[j][0].begin(),awayPhasesPressureBallDist[0].begin(),awayPhasesPressureBallDist[0].end());
								groupPhasePressureBallDist[j][1].insert(groupPhasePressureBallDist[j][1].begin(),awayPhasesPressureBallDist[1].begin(),awayPhasesPressureBallDist[1].end());
								groupPhasePressureBallDist[j][2].insert(groupPhasePressureBallDist[j][2].begin(),awayPhasesPressureBallDist[2].begin(),awayPhasesPressureBallDist[2].end());
							}
						}
					}
					foundGroupB = false;
					for (int j = 0;j<groups.size();j++){
						if(!foundGroupB){
							std::vector<int>::iterator it = std::find(groups[j].begin(),groups[j].end(),homeID);
							if(it!=groups[j].end()){
								foundGroupB = true;
								groupPhasePressureBallDist[j][0].insert(groupPhasePressureBallDist[j][0].begin(),homePhasesPressureBallDist[0].begin(),homePhasesPressureBallDist[0].end());
								groupPhasePressureBallDist[j][1].insert(groupPhasePressureBallDist[j][1].begin(),homePhasesPressureBallDist[1].begin(),homePhasesPressureBallDist[1].end());
								groupPhasePressureBallDist[j][2].insert(groupPhasePressureBallDist[j][2].begin(),homePhasesPressureBallDist[2].begin(),homePhasesPressureBallDist[2].end());
							}
						}
					}
				}
			}
			else{
				if(numberPositionAnalysis){
					std::vector<int> minFrames;
					for (int i = 5; i<40; i+=5){
						minFrames.push_back(i);
					}
					std::vector<int> postPressTimes;
					for (int i = 5; i<40; i+=5){
						postPressTimes.push_back(i);
					}
					std::vector<double> minVels;
					for (double i = 1; i < 10; i ++){
						minVels.push_back(i/10);
					}
					std::vector<int> minDefs;
					for(int i = 1;i<9;i++){
						minDefs.push_back(i);
					}
					std::vector<std::vector<std::vector<std::vector<std::array<int,3> numbers = tgame->getAllPhases(minDefs, minVels, minFrames, postPressTimes);
					for(int i = 0;i<minDefs.size();i++){
						for(int j = 0; j< minVels.size();j++){
							for(int k = 0; k< minFrames.size();k++){
								for(int l = 0; l<postPressTimes.size();l++){
									for(int m = 0;m<3;m++){
										allNumbers[i][j][k][l][m] += numbers[i][j][k][l][m];
									}
								}
							}
						}
					}
				}
			}
					
				//

		}
		delete tgame;
	}

	if(results){
		usedParametersAndResults << "number of games " << numberOfGames << std::endl << "minimum number of defender " << minDefNum << std::endl << "minimum defender velocity " << minDefVel << std::endl << "minimum number of frames " << minFrames << std::endl << "maximum post press looking time " << maxPostPressTime << std::endl << "ball radius " << ballRadius << std::endl << "start looking distance " << startLookingDistance<< std::endl << "looking length " << lookingLength << std::endl << "player radius " << playerRadius << std::endl << "close pressure addition " << std::endl << std::endl;
		//in case of no groups
		if(!useGroups){
			usedParametersAndResults << "number of no possession change phases " << numberOfNormalPhases << std::endl << "number of possession change phases " << numberOfPossessionChangePhases<< std::endl << "number of frame jump phases " << numberOfFrameJumpPhases << std::endl;
			//std::cout << std::endl << "RESULTS" << std::endl << "Total number of no possession change phases " << numberOfNormalPhases << std::endl << "Total number of possession change phases " << numberOfPossessionChangePhases<< std::endl << "Total number of frame jump phases " << numberOfFrameJumpPhases << std::endl;
		}
		//
		//in case of groups
		else{
			for(int i = 0; i<groups.size(); i ++){
				usedParametersAndResults << "For group " << i << ":" << std::endl << "\tNumber of no possession change phases: " << groupPhaseSizeTime[i][0] << std::endl << "\tNumber of possession change phases: " << groupPhaseSizeTime[i][1]<< std::endl << "\tNumber of frame jump phases: " << groupPhaseSizeTime[i][2] << std::endl << "\tNumber of frames they were defending: " << groupPhaseSizeTime[i][3] << std::endl;
				std::cout << "For group " << i << ":" << std::endl << "\tNumber of no possession change phases: " << groupPhaseSizeTime[i][0] << std::endl << "\tNumber of possession change phases: " << groupPhaseSizeTime[i][1]<< std::endl << "\tNumber of frame jump phases: " << groupPhaseSizeTime[i][2] << std::endl << "\tNumber of frames they were defending: " << groupPhaseSizeTime[i][3] << std::endl;
			}
		}
			
		//
		
		//get means in case of no groups
		if(!useGroups){
			double meanPressurePossessionChange = findMean(pressureBallDistPosChange);
			double meanPressureFrameJump = findMean(pressureBallDistFrameJump);
			std::array<std::array<double,2>,5> meanPressuresNoPossessionChange = findMeanOfBins(pressureBallDist);	
			std::cout << "mean Pressure For No Possession Change: " << meanPressuresNoPossessionChange[0][0] << " and goal distance " << meanPressuresNoPossessionChange[0][1]<< std::endl;
			usedParametersAndResults << "mean Pressure For No Possession Change: " << meanPressuresNoPossessionChange[0][0] << " and goal distance " << meanPressuresNoPossessionChange[0][1]<< std::endl;
			std::cout << "mean Pressure For Possession Change: " << meanPressurePossessionChange << std::endl;
			usedParametersAndResults << "mean Pressure For Possession Change: " << meanPressurePossessionChange << std::endl;
			std::cout << "mean Pressure For Frame Jump: " << meanPressureFrameJump << std::endl;
			usedParametersAndResults << "mean Pressure For Frame Jump: " << meanPressureFrameJump << std::endl;
			std::cout << "mean Pressures and Goal Distances For No Possession Change: ";
			for(auto i : meanPressuresNoPossessionChange){
				for(auto j : i){
					std::cout << j << ",";
					usedParametersAndResults << j << ",";
				}
				std::cout << "\t";
				usedParametersAndResults<< "\t";
			}
			std::cout << std::endl;
			usedParametersAndResults<< std::endl;
			usedParametersAndResults.close();
		}
		//
		//in case of groups
		else{
			for(int i = 0;i<groups.size();i++){
				double meanPressurePossessionChange = findMean(groupPhasePressureBallDist[i][1]);
				double meanPressureFrameJump = findMean(groupPhasePressureBallDist[i][2]);
				std::array<std::array<double,2>,5> meanPressuresNoPossessionChange = findMeanOfBins(groupPhasePressureBallDist[i][0]);	
				std::cout << "For group " << i << ":" << "\tMean Pressure For No Possession Change: " << meanPressuresNoPossessionChange[0][0] << " and goal distance " << meanPressuresNoPossessionChange[0][1]<< std::endl;
				usedParametersAndResults << "For group " << i << ":" << "\tMean Pressure For No Possession Change: " << meanPressuresNoPossessionChange[0][0] << " and goal distance " << meanPressuresNoPossessionChange[0][1]<< std::endl;
				std::cout << "\tMean Pressure For Possession Change: " << meanPressurePossessionChange << std::endl;
				usedParametersAndResults << "\tMean Pressure For Possession Change: " << meanPressurePossessionChange << std::endl;
				std::cout << "\tMean Pressure For Frame Jump: " << meanPressureFrameJump << std::endl;
				usedParametersAndResults << "\tMean Pressure For Frame Jump: " << meanPressureFrameJump << std::endl;
				std::cout << "\tMean Pressures and Goal Distances For No Possession Change: ";
				for(auto i : meanPressuresNoPossessionChange){
					for(auto j : i){
						std::cout << j << ",";
						usedParametersAndResults << j << ",";
					}
					std::cout << " ";
					usedParametersAndResults<< " ";
				}
				std::cout << std::endl;
				usedParametersAndResults<< std::endl;
			}
			usedParametersAndResults.close();
		}
				

		//in case of no groups
		if(!useGroups){
			noPossessionChangeO << "Pressure, Change in distance between ball and goal" << std::endl;
			for(auto i : pressureBallDist){
				noPossessionChangeO << i[0] << "," << i[1] << std::endl;
			}
			possessionChangeO << "Pressure"<< std::endl;
			for(auto i : pressureBallDistPosChange){
				possessionChangeO << i[0] << std::endl;
			}
			frameJumpO << "Pressure" << std::endl;
			for(auto i : pressureBallDistFrameJump){
				frameJumpO << i[0] << std::endl;
			}
			noPossessionChangeO.close();
			possessionChangeO.close();
			frameJumpO.close();
		}
		//
		//in case of groups MUST BE EXPLICITLY CHANGED TO COPE N = number of groups != 3
		else{
		/*	noPossessionChangeO << "Pressure, Change in distance between ball and goal" << std::endl;
			possessionChangeO << "Pressure"<< std::endl;
			frameJumpO << "Pressure" << std::endl;
			for(int i = 0;i<groups.size();i++){
				for(auto j : groupPhasePressureBallDist[i][0]){
					noPossessionChangeO << j[0] << "," << j[1] << std::endl;
				}
				for(auto j : groupPhasePressureBallDist[i][2]){
					frameJumpO << j[0] << "," << j[1] << std::endl;
				}
				for(auto j : groupPhasePressureBallDist[i][1]){
					possessionChangeO << j[0] << "," << j[1] << std::endl;
				}
			}

			*/
			
			//Group 1
			noPossessionChangeGa << "Pressure, Change in distance between ball and goal" << std::endl;
			for(auto i : groupPhasePressureBallDist[0][0]){
				noPossessionChangeGa << i[0] << "," << i[1] << std::endl;
			}
			possessionChangeGa << "Pressure"<< std::endl;
			for(auto i : groupPhasePressureBallDist[0][1]){
				possessionChangeGa << i[0] << std::endl;
			}
			frameJumpGa << "Pressure" << std::endl;
			for(auto i : groupPhasePressureBallDist[0][2]){
				frameJumpGa << i[0] << std::endl;
			}
			noPossessionChangeGa.close();
			possessionChangeGa.close();
			frameJumpGa.close();
			//
			//Group 2
			noPossessionChangeGb << "Pressure, Change in distance between ball and goal" << std::endl;
			for(auto i : groupPhasePressureBallDist[1][0]){
				noPossessionChangeGb << i[0] << "," << i[1] << std::endl;
			}
			possessionChangeGb << "Pressure"<< std::endl;
			for(auto i : groupPhasePressureBallDist[1][1]){
				possessionChangeGb << i[0] << std::endl;
			}
			frameJumpGb << "Pressure" << std::endl;
			for(auto i : groupPhasePressureBallDist[1][2]){
				frameJumpGb << i[0] << std::endl;
			}
			noPossessionChangeGb.close();
			possessionChangeGb.close();
			frameJumpGb.close();
			//
			//Group 2
			noPossessionChangeGc << "Pressure, Change in distance between ball and goal" << std::endl;
			for(auto i : groupPhasePressureBallDist[2][0]){
				noPossessionChangeGc << i[0] << "," << i[1] << std::endl;
			}
			possessionChangeGc << "Pressure"<< std::endl;
			for(auto i : groupPhasePressureBallDist[2][1]){
				possessionChangeGc << i[0] << std::endl;
			}
			frameJumpGc << "Pressure" << std::endl;
			for(auto i : groupPhasePressureBallDist[2][2]){
				frameJumpGc << i[0] << std::endl;
			}
			noPossessionChangeGc.close();
			possessionChangeGc.close();
			frameJumpGc.close();
			std::cout << dataDestination << std::endl;
			
		}
	}
	if(numberPositionAnalysis){
		int defNumNum = allNumbers.size();
		int velNum = allNumbers[0].size();
		int frameNum = allNumbers[0][0].size();
		int postNum = allNumbers[0][0][0].size();
		dimensions = defNumNum*velNum*frameNum*postNum;
		std::ofstream out;
		std::string name = dataDestination;
		numbersof.open(dataDestination + "numbers.txt");
		numbersof << "[";
		for (int i = 0;i< defNumNum;i++){
			numbersof << "[";
			for(int j = 0;j<velNum;j++){
				numbersof << "[";
				for (int k = 0;k<frameNum;k++){
					numbersof << "[";
					for(int l = 0;l<postNum;l++){
						numbersof << allNumbers[i][j][k][l] << ",";
					}
					numbersof << "],";
				}
				numbersof << "],";
			}
			numbersof << "]," << std::endl;
		}
		numbersof << "]";
	}


	return 0;
}




			/* TESTA - FIND NUMBER OF ATTACKERS IN FRONT OF BALL
			for(auto j = 0; j < 100;j++){
				std::array<int,12> attackersInFrontOfBall;
				for(int i = 0; i < 12; i ++){
					attackersInFrontOfBall[i] = 0;
				}
				std::vector<Frame*> aframes = tgame->getFrames();
				for(auto frameit = aframes.begin();frameit<aframes.end();++frameit){
					if((*frameit)->setAttackersInFrontOfBall()==true){
						if((*frameit)->getAttackersInFrontOfBall(0) == 11){
							std::cout << (*frameit)->getBall()->getPos()[0] << "," << (*frameit)->getBall()->getPos()[1] << ":";
							std::vector<Player*> tplayers = (*frameit)->getPlayers();
							for (auto t : tplayers){
								if(t->getTeam() == (*frameit)->getAttacking()){
									std::cout << t->getPos()[0] << ",";
								}
							}
							std::cout << std::endl;
						}
					}
					(*frameit)->setGoalPos(tgame->getHomeSide());
					if((*frameit)->setAttackersInFrontOfBall()==true){
				//		std::cout << (*frameit)->getAttackersInFrontOfBall(0) << std::endl;
							attackersInFrontOfBall[(*frameit)->getAttackersInFrontOfBall((j-50)*50)] ++;
					}
				}
				thresholds[j] = attackersInFrontOfBall; 
			}
			for(int i = 0; i < 100; i++){
				std::cout << (i-50)*50 << ": ";
				for(int j = 0; j < 12; j++){
					std::cout << j << "-" << thresholds[i][j] << " " ;
				}
				std::cout << std::endl;
			}
			*/

		//	TESTB: find number of players whose velocity is away from goal and greater than threshold, then find number of time at least 15 consecutive frames 
		/* had over j players running over a threshold velocity of i/3
			std::array<std::array<double,12>,10> runningForwards = {0};
			std::array<std::array<double,12>,10> countt = {0};
			for(auto frameit = aframes.begin();frameit<aframes.end();++frameit){
				for(double i = 0;i<10;i++){
					for(int j = 0;j<12;j++){
						if((*frameit)->getRunningForward(i/3)>=j){
							runningForwards[i][j] ++;
						}
						else{
							if(runningForwards[i][j]>=15){
								countt[i][j] ++;
							}
							runningForwards[i][j] = 0;
						}
					}
				}
			}	
			for(double i = 0;i<10;i++){
				std::cout << i/3 << ": ";
				for(auto j : countt[i]){
					std::cout << j << ",";
				}
				std::cout << std::endl;
			}*/
			



