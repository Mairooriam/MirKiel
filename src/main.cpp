
#include <iostream>
#include "utils.h"
#include "mirkiel.h"
//TODO: at page 77.
    int main(int argc, char const *argv[]) {
        mirkiel::MirKiel mirkiel;
        if (argc > 2) {
            std::cout << "Usage: MirKiel [script]" << std::endl;
            util::systemExit(64);
        } else if (argc == 2) {
            mirkiel.runFile(argv[1]);
        } else {
            mirkiel.runPrompt();
        }
    
        std::cout << "Hello wordl";
        return 0;
    }

