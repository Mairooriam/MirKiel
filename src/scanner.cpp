#include "scanner.h"

#include <iostream>

#include "utils.h"
#include <cctype>
namespace mirkiel {

    const std::unordered_map<std::string, TokenType> Scanner::m_keywords = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
    };


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
            case '/': comment(); break;
            case ' ':
            case '\r':
            case '\t': break;
            case '\n': m_line++; break;
            case '"': string(); break;        
            default: 
                if(isdigit(c)){
                    number();
                if(isalpha(c)){
                    identifier();
                }
                }else{
                    m_mirkiel->error(m_line, "Unexpected character.");
                }
            break;
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
        std::string str = m_src.substr(m_start, m_current - m_start);
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
    void Scanner::comment() {
        if (match('/')) {
            while (peek() != '\n' && !isAtEnd())
                advance();
        } else {
            addToken(TokenType::SLASH);
        }
    }
    void Scanner::string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n')
                m_line++;
            advance();
        }
        if (isAtEnd()) {
            m_mirkiel->error(m_line, "Unterminated string.");
            return;
        }
        advance();
        
        //TODO: instead of only removing "" add handling for "\n" etc.
        std::string value = m_src.substr(m_start + 1, m_current - m_start - 2);
        addToken(TokenType::STRING, value);
    }
    void Scanner::number() {
        while (isdigit(peek())) advance();
        
        if (peek() == '.' && isdigit(peekNext()))
        {
            advance();

            while (isdigit(peek())) advance();
        }
        
        addToken(TokenType::NUMBER, std::stod(m_src.substr(m_start, m_current)));

    }
    char Scanner::peekNext() {
        if (m_current + 1 >= m_src.length()) return '\0';
        return m_src[m_current + 1];
    }
    void Scanner::identifier() {
        while (isalnum(peek()))  advance();
        std::string str = m_src.substr(m_start, m_current - m_start);
        
        TokenType type;
        auto it = m_keywords.find(str);
        if (it != m_keywords.end()) {
            type = it->second;
        } else {
            type = TokenType::IDENTIFIER;
        }
        
        addToken(type);
    }
    Scanner::Scanner(const std::string& src_, MirKiel* mirkiel_)
        : m_src{src_}, m_mirkiel{mirkiel_} {}
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
