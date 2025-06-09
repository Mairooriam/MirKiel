#!/usr/bin/env python3

import os
import sys
from typing import List, Dict, Any

def generate_ast_header(output_dir: str, header_name: str, types: Dict[str, List[Dict[str, str]]]) -> None:
    """Generate a C++ header file with AST class definitions."""
    path = os.path.join(output_dir, f"{header_name}.h")
    
    with open(path, "w") as writer:
        # Header guard
        writer.write(f"// filepath: {path}\n")
        writer.write("#pragma once\n")
        
        # Includes
        writer.write("#include <memory>\n")
        writer.write("#include <string>\n")
        writer.write("#include <variant>\n")
        writer.write("#include <vector>\n")
        writer.write('#include "token.h"\n\n')
        
        # Namespace
        writer.write("namespace mirkiel {\n\n")
        
        # Forward declarations
        for struct_name in types.keys():
            writer.write(f"    struct {struct_name};\n")
        writer.write("\n")
        
        # Define the variant type
        writer.write("    using Expr = std::variant<\n")
        for i, struct_name in enumerate(types.keys()):
            comma = "," if i < len(types) - 1 else ""
            writer.write(f"        std::shared_ptr<{struct_name}>{comma}\n")
        writer.write("    >;\n\n")
        
        # Define each struct
        for struct_name, fields in types.items():
            writer.write(f"    struct {struct_name} {{\n")
            
            # Fields
            for field in fields:
                writer.write(f"        {field['type']} {field['name']};\n")
            writer.write("\n")
            
            # Constructor
            writer.write(f"        {struct_name}(")
            params = [f"{field['type']} {field['name']}" for field in fields]
            writer.write(", ".join(params))
            writer.write(")\n")
            
            # Constructor initialization list
            writer.write("            : ")
            inits = [f"{field['name']}(std::move({field['name']}))" for field in fields]
            writer.write(", ".join(inits))
            writer.write(" {}\n")
            
            writer.write("    };\n\n")
        
        # Visitor base class
        writer.write("    template<typename R>\n")
        writer.write("    class Visitor {\n")
        writer.write("    public:\n")
        for struct_name in types.keys():
            writer.write(f"        virtual R visit{struct_name}Expr(const {struct_name}& expr) = 0;\n")
        writer.write("        virtual ~Visitor() = default;\n")
        writer.write("    };\n\n")
        
        # ExprVisitor class
        writer.write("    template<typename R>\n")
        writer.write("    class ExprVisitor {\n")
        writer.write("    private:\n")
        writer.write("        Visitor<R>& visitor;\n")
        writer.write("    \n")
        writer.write("    public:\n")
        writer.write("        explicit ExprVisitor(Visitor<R>& v) : visitor(v) {}\n")
        writer.write("        \n")
        for struct_name in types.keys():
            writer.write(f"        R operator()(const std::shared_ptr<{struct_name}>& expr) {{ return visitor.visit{struct_name}Expr(*expr); }}\n")
        writer.write("    };\n\n")
        
        # Accept function
        writer.write("    template<typename R>\n")
        writer.write("    R accept(const Expr& expr, Visitor<R>& visitor) {\n")
        writer.write("        return std::visit(ExprVisitor<R>(visitor), expr);\n")
        writer.write("    }\n\n")
        
        # Close namespace
        writer.write("} // namespace mirkiel\n")
    
    print(f"Successfully generated {path}")

def main() -> None:
    """Main entry point for the AST generator."""
    if len(sys.argv) != 2:
        print("Usage: generate_ast.py <output directory>")
        sys.exit(64)
    
    output_dir = sys.argv[1]
    
    # Define expression types with their fields
    expr_types = {
        "Binary": [
            {"type": "std::shared_ptr<Expr>", "name": "left"},
            {"type": "Token", "name": "operator_"},
            {"type": "std::shared_ptr<Expr>", "name": "right"}
        ],
        "Unary": [
            {"type": "Token", "name": "operator_"},
            {"type": "std::shared_ptr<Expr>", "name": "right"}
        ],
        "Grouping": [
            {"type": "std::shared_ptr<Expr>", "name": "expression"}
        ],
        "Literal": [
            {"type": "mirkiel::Literal", "name": "value"}
        ],
        "Variable": [
            {"type": "Token", "name": "name"}
        ],
        "Assign": [
            {"type": "Token", "name": "name"},
            {"type": "std::shared_ptr<Expr>", "name": "value"}
        ],
        "Call": [
            {"type": "std::shared_ptr<Expr>", "name": "callee"},
            {"type": "Token", "name": "paren"},
            {"type": "std::vector<std::shared_ptr<Expr>>", "name": "arguments"}
        ]
    }
    
    generate_ast_header(output_dir, "expr", expr_types)

if __name__ == "__main__":
    main()