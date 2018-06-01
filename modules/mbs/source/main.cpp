#include <iostream>

#include <mbs/MbsException.h>
#include <mbs/MbsProject.h>

int main(int argc, char *argv[]){
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " <path/to/mbs.yml> <path/to/output.cmake>" << std::endl;
        return -1;
    }

    try{
        mbs::MbsProject project(argv[1]);
        project.outputToCmake(argv[2]);
    }catch(mbs::MbsException &e){
        std::cerr << "Failed to process mbs project: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}