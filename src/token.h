#pragma once
#include <memory>
#include <string>
#include <variant>


#include "tokentype.h"
namespace mirkiel
{
    using Literal = std::variant<std::nullptr_t, bool, double, std::string>;
    
    class Token
    {
    private:
        const TokenType m_type;
        const std::string m_lexeme;
        const Literal m_literal;
        const int m_line;
    public:
        Token(TokenType type, 
              std::string lexeme,
              Literal literal,
              int line)
            : m_type(type), 
              m_lexeme(std::move(lexeme)), 
              m_literal(std::move(literal)), 
              m_line(line) {}
              

        ~Token() = default;
        
        TokenType getType() const { return m_type; }
        const std::string& getLexeme() const { return m_lexeme; }
        const auto& getLiteral() const { return m_literal; }
        int getLine() const { return m_line; }
        
        Token(const Token&) = delete;
        Token& operator=(const Token&) = delete;
        Token(Token&&) = delete;
        Token& operator=(Token&&) = delete;
    private:
        friend std::ostream& operator<<(std::ostream& os, const Token& node);   
    public:
        Token();
        ~Token();
    };
    

    
} // namespace mirkiel
