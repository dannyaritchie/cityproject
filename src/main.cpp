#include "models/possessionfilereader/possessionfilemaker.h"

int main(){
	PossessionFileMaker * ggg = new PossessionFileMaker();
	ggg->readFile();
	ggg->fileSpecificWriter(855206);
}
