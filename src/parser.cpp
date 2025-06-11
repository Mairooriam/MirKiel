#include "parser.h"
#include <iostream>
namespace mirkiel {
    Expr parser::expression() {
        return equality();
    }

    Expr parser::equality() {
        Expr expr = comparison();

        while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
            Token operator_ = previous();
            Expr right = comparison();

            expr = std::make_shared<Binary>(std::make_shared<Expr>(expr), operator_,
                                            std::make_shared<Expr>(right));
        }

        return expr;
    }

    bool parser::match(std::vector<TokenType> types) {
        for (auto &&type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    bool parser::check(TokenType type) {
        if (isAtEnd())
            return false;

        return peek().getType() == type;
    }

    Token parser::advance() {
        if (!isAtEnd())
            m_current++;
        return previous();
    }

    bool parser::isAtEnd() {
        return peek().getType() == TokenType::EOD;
    }

    Token parser::peek() {
        return m_tokens[m_current];
    }

    Token parser::previous() {
        return m_tokens[m_current - 1];
    }
    Expr parser::comparison() {
        Expr expr = term();

        while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                      TokenType::LESS_EQUAL})) {
            Token op = previous();
            Expr right = term();
            expr = std::make_shared<Binary>(std::make_shared<Expr>(expr), op,
                                            std::make_shared<Expr>(right));
        }
        return expr;
    }
    Expr parser::term() {
        Expr expr = factor();

        while (match({TokenType::MINUS, TokenType::PLUS})) {
            Token op = previous();
            Expr right = factor();
            expr =
                std::make_shared<Binary>(std::make_shared<Expr>, op, std::make_shared<Expr>(right));
        }
        return expr;
    }
    Expr parser::factor() {
        Expr expr = unary();

        while (match({TokenType::SLASH, TokenType::STAR})) {
            Token op = previous();
            Expr right = unary();
            expr =
                std::make_shared<Binary>(std::make_shared<Expr>, op, std::make_shared<Expr>(right));
        }
        return expr;
    }
    Expr parser::unary() {
        if (match({TokenType::BANG, TokenType::MINUS}))
        {
            Token op = previous();
            Expr right = unary();
            return std::make_shared<Unary>(op,std::make_shared<Expr>(right));
        }
        return primary(); 
    }
    Expr parser::primary() {
        if (match({TokenType::FALSE})) return std::make_shared<Literal>(false);
        if (match({TokenType::TRUE})) return std::make_shared<Literal>(true);
        if (match({TokenType::NIL})) return std::make_shared<Literal>(nullptr);
        

        if (match({TokenType::NUMBER, TokenType::STRING}))
        {
            return std::make_shared<Literal>(previous().getLiteral());
        }

        if (match({TokenType::LEFT_PAREN}))
        {
            Expr expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
            return std::make_shared<Grouping>(std::make_shared<Expr>(expr));
        }
        
        throw error(peek(), "Expected expression.");
    }
    Token parser::consume(TokenType type, std::string message) {
        if (check(type)) return advance();
        throw error(peek(), message);
    }
    void parser::synchronize() {
        advance();

        while (!isAtEnd())
        {
            if (previous().getType() == TokenType::SEMICOLON) return;
            switch (peek().getType()){
                case TokenType::CLASS:
                case TokenType::FUN:
                case TokenType::VAR:
                case TokenType::FOR:
                case TokenType::IF:
                case TokenType::WHILE:
                case TokenType::PRINT:
                case TokenType::RETURN:
                    return;

            }
            advance();
        }
        
    }
    ParseError parser::error(Token token, const std::string& message) {
        // Report the error
        if (token.getType() == TokenType::EOD) {
            std::cerr << token.getLine() << " at end: " << message << std::endl;
        } else {
            std::cerr << token.getLine() << " at '" << token.getLexeme() << "': " << message << std::endl;
        }
        
        // Return a ParseError object that can be thrown
        return ParseError(message);
    }

}  // namespace mirkiel
