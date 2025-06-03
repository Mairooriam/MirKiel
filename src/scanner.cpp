#include "scanner.h"

#include <iostream>

#include "utils.h"

namespace mirkiel {
    bool Scanner::isAtEnd() {
        return m_current >= m_src.length();
    }

    void Scanner::scanToken() {
        char c = advance();
        switch (c) {
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
            case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
            case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
            case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
            case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
            case '/':
                if (match('/')) {
                    while (peek() != '\n' && !isAtEnd())
                        advance();
                } else {
                    addToken(TokenType::SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t': break;
            case '\n': m_line++; break;
            default: std::cout << "Nothing yet. Scanner.cpp line 23." << "\n"; break;
        }
    }

    char Scanner::advance() {
        // TODO: add isAtEnd()
        // this might give bug? hehehehehe
        // if (isAtEnd()) {
        //     return '\0';
        // }
        return m_src[m_current++];
    }

    void Scanner::addToken(TokenType type_) {
        addToken(type_, nullptr);
    }

    void Scanner::addToken(TokenType type_, Literal literal_) {
        std::string str = m_src.substr(m_start, m_current);
        m_tokens.emplace_back(Token(type_, str, literal_, m_line));
    }

    bool Scanner::match(char expected) {
        if (isAtEnd())
            return false;
        if (m_src[m_current] != expected)
            return false;

        m_current++;
        return true;
    }
    char Scanner::peek() {
        if (isAtEnd())
            return '\0';
        return m_src[m_current];
    }
    Scanner::Scanner(const std::string& src_) : m_src{src_} {}
    std::vector<Token> Scanner::scanTokens() {
        while (!isAtEnd()) {
            m_start = m_current;
            scanToken();
        }

        m_tokens.emplace_back(Token(TokenType::EOD, "", nullptr, m_line));
        return m_tokens;
    }

    Scanner::~Scanner() {}

}  // namespace mirkiel
