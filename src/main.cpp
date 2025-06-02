
#include <iostream>
#include "utils.h"
#include "mirkiel.h"
//TODO: at page 77.
    int main(int argc, char const *argv[]) {
        mirkiel::MirKiel mirkiel;
        if (argc > 1) {
            std::cout << "Usage: MirKiel [script]" << std::endl;
            util::systemExit(64);
        } else if (argc == 1) {
            mirkiel.runFile(argv[0]);
        } else {
            mirkiel.runPromt();
        }
    
        std::cout << "Hello wordl";
        return 0;
    }

