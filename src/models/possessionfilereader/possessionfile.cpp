
#include "possessionfile.h"
#include<vector>

PossessionFile::PossessionFile(int pmid, std::vector<PossessionFileLine*> plines){
	mid = pmid;
	lines = plines;
}
int PossessionFile::getMid(){
	return mid;
}
void PossessionFile::osLines(std::ostream & os){
	for(auto lineit = lines.begin();lineit<lines.end();++lineit){
		(*lineit)->infoToFile(os);
	}
}
std::vector<PossessionFileLine*>& PossessionFile::getPosFilLin(){
    return lines;
}
