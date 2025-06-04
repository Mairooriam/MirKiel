#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"
#include "mirkiel.h"
namespace mirkiel {
    class Scanner {
    private:
        std::string m_src;
        MirKiel* m_mirkiel;
        std::vector<Token> m_tokens;
        size_t m_start = 0;
        size_t m_current = 0;
        size_t m_line = 1;
        static const std::unordered_map<std::string, TokenType> m_keywords;
    private:
        bool isAtEnd();
        void scanToken();
        char advance();
        void addToken(TokenType type_);
        void addToken(TokenType type_, Literal literal_);
        bool match(char expected);
        char peek();
        void comment();
        void string();
        void number();
        char peekNext();
        void identifier();
    public: 
        Scanner(const std::string& src_, MirKiel* mirkiel_);
        std::vector<Token> scanTokens();
        
        ~Scanner();
    };
}  // namespace mirkiel
