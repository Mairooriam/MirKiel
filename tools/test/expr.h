// filepath: test\expr.h
#pragma once
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "token.h"

namespace mirkiel {

    struct Binary;
    struct Unary;
    struct Grouping;
    struct Literal;
    struct Variable;
    struct Assign;
    struct Call;

    using Expr = std::variant<
        std::shared_ptr<Binary>,
        std::shared_ptr<Unary>,
        std::shared_ptr<Grouping>,
        std::shared_ptr<Literal>,
        std::shared_ptr<Variable>,
        std::shared_ptr<Assign>,
        std::shared_ptr<Call>
    >;

    struct Binary {
        std::shared_ptr<Expr> left;
        Token operator_;
        std::shared_ptr<Expr> right;

        Binary(std::shared_ptr<Expr> left, Token operator_, std::shared_ptr<Expr> right)
            : left(std::move(left)), operator_(std::move(operator_)), right(std::move(right)) {}
    };

    struct Unary {
        Token operator_;
        std::shared_ptr<Expr> right;

        Unary(Token operator_, std::shared_ptr<Expr> right)
            : operator_(std::move(operator_)), right(std::move(right)) {}
    };

    struct Grouping {
        std::shared_ptr<Expr> expression;

        Grouping(std::shared_ptr<Expr> expression)
            : expression(std::move(expression)) {}
    };

    struct Literal {
        mirkiel::Literal value;

        Literal(mirkiel::Literal value)
            : value(std::move(value)) {}
    };

    struct Variable {
        Token name;

        Variable(Token name)
            : name(std::move(name)) {}
    };

    struct Assign {
        Token name;
        std::shared_ptr<Expr> value;

        Assign(Token name, std::shared_ptr<Expr> value)
            : name(std::move(name)), value(std::move(value)) {}
    };

    struct Call {
        std::shared_ptr<Expr> callee;
        Token paren;
        std::vector<std::shared_ptr<Expr>> arguments;

        Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
            : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}
    };

    template<typename R>
    class Visitor {
    public:
        virtual R visitBinaryExpr(const Binary& expr) = 0;
        virtual R visitUnaryExpr(const Unary& expr) = 0;
        virtual R visitGroupingExpr(const Grouping& expr) = 0;
        virtual R visitLiteralExpr(const Literal& expr) = 0;
        virtual R visitVariableExpr(const Variable& expr) = 0;
        virtual R visitAssignExpr(const Assign& expr) = 0;
        virtual R visitCallExpr(const Call& expr) = 0;
        virtual ~Visitor() = default;
    };

    template<typename R>
    class ExprVisitor {
    private:
        Visitor<R>& visitor;
    
    public:
        explicit ExprVisitor(Visitor<R>& v) : visitor(v) {}
        
        R operator()(const std::shared_ptr<Binary>& expr) { return visitor.visitBinaryExpr(*expr); }
        R operator()(const std::shared_ptr<Unary>& expr) { return visitor.visitUnaryExpr(*expr); }
        R operator()(const std::shared_ptr<Grouping>& expr) { return visitor.visitGroupingExpr(*expr); }
        R operator()(const std::shared_ptr<Literal>& expr) { return visitor.visitLiteralExpr(*expr); }
        R operator()(const std::shared_ptr<Variable>& expr) { return visitor.visitVariableExpr(*expr); }
        R operator()(const std::shared_ptr<Assign>& expr) { return visitor.visitAssignExpr(*expr); }
        R operator()(const std::shared_ptr<Call>& expr) { return visitor.visitCallExpr(*expr); }
    };

    template<typename R>
    R accept(const Expr& expr, Visitor<R>& visitor) {
        return std::visit(ExprVisitor<R>(visitor), expr);
    }

} // namespace mirkiel
