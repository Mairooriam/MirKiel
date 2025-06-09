#pragma once
#include <memory>
#include <string>
#include <variant>
#include <sstream>
#include <iomanip>
#include "tokentype.h"

namespace mirkiel {
    using LiteralValue = std::variant<std::nullptr_t, bool, double, std::string>;

    class Token {
    private:
        TokenType m_type;
        std::string m_lexeme;
        LiteralValue m_literal;
        int m_line;

    public:
        Token(TokenType type, std::string lexeme, LiteralValue literal, int line)
            : m_type(type), m_lexeme(lexeme), m_literal(literal), m_line(line) {}

        ~Token();

        TokenType getType() const {
            return m_type;
        }
        const std::string& getLexeme() const {
            return m_lexeme;
        }
        const auto& getLiteral() const {
            return m_literal;
        }
        int getLine() const {
            return m_line;
        }

    private:
        friend std::ostream& operator<<(std::ostream& os, const Token& node);

    public:
    };

}  // namespace mirkiel
