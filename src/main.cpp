#include "models/game.h"
#include "models/briantable/allclosest.h"

int main(){
	int distanceThreshold = 15;
	for (int mid = 1059702; mid < 1059703;mid++){
		Game * tgame = new Game(mid, "../idata");
		tgame->readNewFile();
		std::vector<Frame*> aframes = tgame->getFrames();
		tgame->addVelocities();
		AllClosest * tallClosest = new AllClosest(distanceThreshold, tgame->getMap(),tgame->getMapLength());
		tallClosest->openStreams(mid,tgame->getMap());
		int previousAttacking = 0;
		int previousFid = -1;
		for(auto frameit = aframes.begin();frameit<aframes.end();++frameit){
			std::vector<Player*> players = (*frameit)->getPlayers();
		//	for(auto playerit = players.begin(); playerit < players.end();++playerit){
		//		std::cout << (*playerit)->getPos()[0]<< std::endl;
		//	}
			tallClosest->addPlayers(frameit, previousFid, previousAttacking);
			previousAttacking = (*frameit)->getAttacking();
			previousFid = (*frameit)->getFid();			
		}
		tallClosest->closeStreams();
		delete tallClosest;
	}
	return 0;
}

