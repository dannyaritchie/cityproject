#include "computeangle.h"
#include "../possessionfilereader/possessionfilemaker.h"
#include "../game.h"
#include <iostream>
#include <fstream>
void computeAngle(){
    PossessionFileMaker * maker = new PossessionFileMaker();
    maker->fileLoader();
    std::vector<PossessionFile*> possessionFiles = maker->getPossessions();
    int fid;
    std::ofstream os;
    std::string filename;
    for(auto posFilit = possessionFiles.begin(); posFilit<possessionFiles.end();posFilit++){
         Game * tgame = new Game((*posFilit)->getMid(),"../idata/");
         if (tgame->readFile() == true){
            if( tgame->storeMdata() == true){
            	std::cout<<(*posFilit)->getMid()<<std::endl;
                int home = tgame->getHome();
                std::vector<Frame*>::iterator frameit = tgame->getFrameit();
                filename = "../data/angdata/" + std::to_string((*posFilit)->getMid()) + ".txt"; 
                os.open(filename);
                for(auto lineit = (*posFilit)->getPosFilLin().begin();lineit < (*posFilit)->getPosFilLin().end(); ++lineit){
                   int tid;
                   if(home == (*lineit)->getTid()){
                       tid = 0;
                   }else{tid = 1;}
                   std::array<int, 2>  frameRange = (*lineit)->getFrameRange();
                    bool found{true};
                    while(found){
                        fid = (*frameit)->getFid(); 
                        if(fid  >= frameRange[0]&&fid<=frameRange[1]){
                           (*frameit)->computeScalar(tid);
                           os << fid << "\t" << tid << std::endl;
                           (*frameit)->writeScalar(os);
                           ++frameit;
                        }
                        else {
                            if (fid>frameRange[1]){
                                found == false;
                            }
                            else{++frameit;}
                        }
                    } 
                }
            os.close();
            }
        }
        delete tgame;
    }
    delete maker;
}


