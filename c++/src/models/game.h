#ifndef GAME_H_
#define GAME_H_
#include <vector>
#include "frame.h"
#include"wholeplayer.h"
#include "idmap.h"
class Game {
	private:
		std::vector<WholePlayer*> playersGame;
		std::vector<Frame*> frames;
		std::vector<Frame*> homeFrames;
		std::vector<Frame*> awayFrames;
		std::string filename;
		char homeSide;
		//std::string filepath;
		double pitchx, pitchy;
		int homeNum, awayNum, mid;
		Idmap idmapd;
	public:
		Game(int, std::string);
		~Game();
		void addFrame(Frame*);
		bool readFile(bool);
		bool readNewFile(std::vector<int>,int&,int&);
	        bool storeMdata();
		//reads in Mdata for a game
		void writeFile(std::ofstream & os,int type);
		void findPhases();
		void addVelocities();
		void addBVelocities();
		void setClosestPlayers();
		void writeClosestPlayers(int game);
		std::vector<double> calcVoronoi();
		void writeVoronoi();
		void addClosestPLayer();
		//records distance to closest player for every player
		void splitFrames();
		//reads in correct possession file
		//splits frames into two vectors, one for home in possession and one for attack in possession
		//Issue:possesion file frame id
		void getScalar();
      		std::vector<Frame*>  getFrames();
        	int getHome();
		int getX();
		int getY();
		int getMapLength();
		char getHomeSide();
		Idmap getMap();
		std::array<int,2> output(std::ofstream & names, std::vector<int> prevDone);
		void setFramesPlayersDistance(double radius);
		void setFramesPlayersVelocity();
		void setAllGoalPos();
		std::vector<std::array<double,2>> getPhaseInformation(std::vector<std::array<int,2>> startSizes, int startLookingDistance, int lookingLength, double radius, int type, double closePressure, std::array<double, 7> parameters, double playerRadius);
		std::vector<std::array<int,4>> getPhases(int minDefA, double minDefVel, int minFrames, int postPressTime,bool errorMsg);
		std::array<int,2> getPossessionTimes();
		std::vector<std::vector<std::vector<std::vector<std::array<int,3>>>>> getAllPhases(std::vector<int> defenders, std::vector<double> velocities, std::vector<int> minimumFrames, std::vector<int> postPressTimes);
		std::vector<std::vector<std::vector<std::vector<std::array<int,3>>>>> getAllPhasesB(std::vector<int> defenders, std::vector<double> velocities, int minimumFrames, std::vector<int> postPressTimes, std::vector<int> lengthBins);
		std::vector<double> getPhasePosition(std::vector<std::array<int,2>> startSizes);
		std::vector<std::vector<std::vector<std::array<std::vector<std::array<int,2>>,3>>>> getBinnedPosition(std::vector<int> defenders, std::vector<double> velocities, std::vector<int> minimumFrames, int postPressTime);
		std::vector<std::vector<std::array<int,2>>> lengthSort(std::vector<std::array<int,2>> lengths, std::vector<int> bins);

};
#endif

