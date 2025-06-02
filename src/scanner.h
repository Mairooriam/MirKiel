#pragma once
#include <string>
#include <vector>

#include "token.h"

namespace mirkiel {
    class scanner {
    private:
        std::string m_src;
        std::vector<Token> m_tokens;
        size_t m_start = 0;
        size_t m_current = 0;
        size_t m_line = 1;
        bool isAtEnd();
        void scanToken();
        char advance();
        void addToken(TokenType type_);
        void addToken(TokenType type_, Literal literal_);
    public: 
        scanner(const std::string& src_);
        std::vector<Token> scanTokens();
        
        ~scanner();
    };
}  // namespace mirkiel
