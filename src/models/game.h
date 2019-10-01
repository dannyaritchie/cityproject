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
		//std::string filepath;
		double pitchx, pitchy;
		int homeNum, awayNum, mid;
		Idmap idmapd;
	public:
		Game(int, std::string);
		~Game();
		void addFrame(Frame*);
		bool readFile(bool);
	        bool storeMdata();
		//reads in Mdata for a game
		void writeFile(std::ofstream & os,int type);
		void findPhases();
		void addVelocities();
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
      		std::vector<Frame*> getFrames();
        	int getHome();
		int getX();
		int getY();
		int getMapLength();
		Idmap getMap();

};
#endif

