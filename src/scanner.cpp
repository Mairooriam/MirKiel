#include "scanner.h"

namespace mirkiel {
    bool scanner::isAtEnd() {
        return m_current >= m_src.length();
    }

    void scanner::scanToken() {
        char c = advance();
        switch (c)
        {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        default:
            break;
        }
    }

    char scanner::advance() {
        // TODO: add isAtEnd()
        // this might give bug? hehehehehe
        if (isAtEnd()) {
            return '\0'; 
        }
        return m_src[m_current++];
    }

    void scanner::addToken(TokenType type_) {
        addToken(type_, nullptr);
    }

    void scanner::addToken(TokenType type_, Literal literal_) {
        std::string str = m_src.substr(m_start, m_current);
    }

    scanner::scanner(const std::string& src_) : m_src{src_} {}
    std::vector<Token> scanner::scanTokens() {
        std::vector<Token> tokens; 
        
        while (!isAtEnd()) {
            m_start = m_current;
            scanToken();
        }
        
        tokens.push_back(Token(TokenType::EOD, "", nullptr, m_line));
        return tokens;  
    }

  
    scanner::~scanner() {}

}  // namespace mirkiel
