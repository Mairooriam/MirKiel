#pragma once
#include "token.h"
#include <vector>
#include "expr.h"

namespace mirkiel
{
    class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& message) : std::runtime_error(message) {}
    };


    class parser
    {
    private:
        std::vector<Token> m_tokens;
        size_t m_current = 0;

    private:

        ParseError error(Token token, const std::string& message);
        
        Expr expression();
        Expr equality();
        bool match(std::vector<TokenType> types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        Expr comparison();
        Expr term();
        Expr factor();
        Expr unary();
        Expr primary();
        Token consume(TokenType type, std::string message);
        void synchronize();
    public:
        parser(const std::vector<Token>& tokens) 
        : m_tokens(tokens){}
        ~parser();
    };
    

} // namespace mirkiel
