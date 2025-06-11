#include <iostream>
#include <memory>
#include <string>
#include "expr.h"
#include "token.h"

using namespace mirkiel;

// A simple AST printer visitor
class AstPrinter : public Visitor<std::string> {
public:
    std::string print(const Expr& expr) {
        return accept(expr, *this);
    }

    std::string visitBinaryExpr(const Binary& expr) override {
        return parenthesize(expr.operator_.getLexeme(), {expr.left, expr.right});
    }

    std::string visitUnaryExpr(const Unary& expr) override {
        return parenthesize(expr.operator_.getLexeme(), {expr.right});
    }

    std::string visitGroupingExpr(const Grouping& expr) override {
        return parenthesize("group", {expr.expression});
    }

    std::string visitLiteralExpr(const Literal& expr) override {
        if (std::holds_alternative<double>(expr.value)) {
            return std::to_string(std::get<double>(expr.value));
        } 
        else if (std::holds_alternative<std::string>(expr.value)) {
            return std::get<std::string>(expr.value);
        }
        else if (std::holds_alternative<bool>(expr.value)) {
            return std::get<bool>(expr.value) ? "true" : "false";
        }
        else {
            return "nil";
        }
    }

    std::string visitVariableExpr(const Variable& expr) override {
        return expr.name.getLexeme();
    }

    std::string visitAssignExpr(const Assign& expr) override {
        return parenthesize("=", {
            std::make_shared<Expr>(std::make_shared<Variable>(expr.name)),
            expr.value
        });
    }

    std::string visitCallExpr(const Call& expr) override {
        std::vector<std::shared_ptr<Expr>> args = {expr.callee};
        for (const auto& arg : expr.arguments) {
            args.push_back(arg);
        }
        return parenthesize("call", args);
    }

private:
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
        std::string result = "(" + name;

        for (const auto& expr : exprs) {
            result += " ";
            result += accept(*expr, *this);
        }

        result += ")";
        return result;
    }
};

// int main() {
//     auto literal123 = std::make_shared<Literal>(123.0);
//     auto literal4567 = std::make_shared<Literal>(45.67);
    
//     // Create the unary expression -123
//     auto unary = std::make_shared<Unary>(
//         Token(TokenType::MINUS, "-", nullptr, 1),
//         std::make_shared<Expr>(literal123)
//     );
    
//     // Create the grouping (45.67)
//     auto grouping = std::make_shared<Grouping>(
//         std::make_shared<Expr>(literal4567)
//     );
    
//     auto binary = std::make_shared<Binary>(
//         std::make_shared<Expr>(unary),
//         Token(TokenType::STAR, "*", nullptr, 1),
//         std::make_shared<Expr>(grouping)
//     );
    
//     auto expression = std::make_shared<Expr>(binary);

//     AstPrinter printer;
//     std::cout << "AST: " << printer.print(*expression) << std::endl;
//     // Should print: (* (- 123) (group 45.67))

//     return 0;
// }