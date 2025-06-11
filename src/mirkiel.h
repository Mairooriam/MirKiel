#pragma once
#include <filesystem>
#include <string>
#include "utils.h"
#include "token.h"
namespace mirkiel {
    template <typename Container>
    Container readFile(const std::filesystem::path& path_);
    
    class MirKiel {
        private:
        void report(int line_, std::string where_, std::string msg_);
        void run(const std::string& src_);
        
    public:
        void error(int line_, std::string msg_);
        void error(Token token, std::string message);
        void runFile(const std::filesystem::path& path_);
        void runPrompt();

    private:
        bool m_hadError = false;

    public:
        MirKiel(/* args */);
        ~MirKiel();
    };

}  // namespace Mirkiel