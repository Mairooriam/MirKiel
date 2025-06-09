#include "token.h"
#include <iomanip> 

namespace mirkiel {
    struct VariantPrinter {
        std::ostream& os;
        
        void operator()(std::nullptr_t) const { os << "nil"; }
        void operator()(bool value) const { os << (value ? "true" : "false"); }
        void operator()(double value) const { os << value; }
        void operator()(const std::string& value) const { os << "\"" << value << "\""; }
    };

    std::ostream& operator<<(std::ostream& os, const LiteralValue& value) {
        std::visit(VariantPrinter{os}, value);
        return os;
    }

    Token::~Token() {}

    std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "["   << token.getType() << "] ";
        os  << "( " << token.getLexeme() << " )";
        os  << " - " << token.getLiteral();
        return os; 
    }
}  // namespace mirkiel
