#include "models/game.h"
#include"models/possessionfilereader/possessionfilemaker.h"
#include <iostream>
#include <fstream>
int main() {
	//std::ofstream os;
	//os.open("velocities.txt");

	PossessionFileMaker * files = new PossessionFileMaker();
	files->fileLoader();
	std::cout << "yay" << std::endl;
	files->fileWriter();
/*	for (int i = 803161;i< 1049290;i++){
		Game* testgame = new Game(i);
		if (testgame->readFile() == true){
				testgame->setClosestPlayers();
				testgame->writeClosestPlayers(i);
			}
		delete testgame;
        }
*/	

	//testgame->storeMdata();
	//testgame->splitFrames();
	//testgame->getScalar();

	//testgame->findPhases();
	//testgame->setClosestPlayers();
	//testgame->addVelocities();
	//testgame->writeFile(os,1);
	std::cout << "yay" << std::endl;
	return 0;
}
