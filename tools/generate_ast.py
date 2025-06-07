import os
import sys

def generate_header(output_path, types):
    # Create directory if it doesn't exist
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    with open(output_path, 'w') as f:
        f.write("#pragma once\n")
        f.write("#include <memory>\n")
        f.write("#include <string>\n")
        f.write("#include <variant>\n")
        f.write("#include <vector>\n")
        f.write("#include \"token.h\"\n\n")
        f.write("namespace mirkiel {\n\n")
        
        # Forward declarations
        for type_name in types:
            class_name = type_name.split(':')[0].strip()
            f.write(f"    struct {class_name};\n")
        f.write("\n")
        
        # Write variant declaration
        f.write("    using Expr = std::variant<\n")
        for i, type_name in enumerate(types):
            class_name = type_name.split(':')[0].strip()
            comma = "," if i < len(types) - 1 else ""
            f.write(f"        std::shared_ptr<{class_name}>{comma}\n")
        f.write("    >;\n\n")
        
        # Generate each class
        for type_def in types:
            # Split only on the first colon
            parts = type_def.split(':', 1)
            class_name = parts[0].strip()
            fields_str = parts[1].strip()
            fields = [field.strip() for field in fields_str.split(',')]
            
            f.write(f"    struct {class_name} {{\n")
            
            # Fields
            for field in fields:
                f.write(f"        {field};\n")
            f.write("\n")
            
            # Constructor
            f.write(f"        {class_name}(")
            params = []
            for field in fields:
                # Split on the last space to get type and name
                parts = field.rsplit(' ', 1)
                if len(parts) == 2:
                    type_name, var_name = parts
                    params.append(f"{type_name} {var_name}")
            f.write(", ".join(params))
            f.write(")\n")
            f.write("            : ")
            
            # Constructor initializers
            initializers = []
            for field in fields:
                parts = field.rsplit(' ', 1)
                if len(parts) == 2:
                    _, var_name = parts
                    initializers.append(f"{var_name}(std::move({var_name}))")
            f.write(", ".join(initializers))
            f.write(" {}\n")
            
            f.write("    };\n\n")
        
        # Add visitor base class
        f.write("    template<typename R>\n")
        f.write("    class Visitor {\n")
        f.write("    public:\n")
        
        # Add visit methods for each type
        for type_def in types:
            class_name = type_def.split(':', 1)[0].strip()
            f.write(f"        virtual R visit{class_name}Expr(const {class_name}& expr) = 0;\n")
        
        f.write("        virtual ~Visitor() = default;\n")
        f.write("    };\n\n")
        
        # Add ExprVisitor class
        f.write("    template<typename R>\n")
        f.write("    class ExprVisitor {\n")
        f.write("    private:\n")
        f.write("        Visitor<R>& visitor;\n")
        f.write("    \n")
        f.write("    public:\n")
        f.write("        explicit ExprVisitor(Visitor<R>& v) : visitor(v) {}\n")
        f.write("        \n")
        
        # Add operator() overloads for each type
        for type_def in types:
            class_name = type_def.split(':', 1)[0].strip()
            f.write(f"        R operator()(const std::shared_ptr<{class_name}>& expr) {{ return visitor.visit{class_name}Expr(*expr); }}\n")
        
        f.write("    };\n\n")
        
        # Add accept function
        f.write("    template<typename R>\n")
        f.write("    R accept(const Expr& expr, Visitor<R>& visitor) {\n")
        f.write("        return std::visit(ExprVisitor<R>(visitor), expr);\n")
        f.write("    }\n\n")
        
        f.write("} // namespace mirkiel\n")

def main():
    if len(sys.argv) != 2:
        print("Usage: generate_ast.py <output_directory>")
        sys.exit(64)
    
    output_dir = sys.argv[1]
    
    types = [
        "Binary : std::shared_ptr<Expr> left, Token operator_, std::shared_ptr<Expr> right",
        "Unary : Token operator_, std::shared_ptr<Expr> right",
        "Grouping : std::shared_ptr<Expr> expression",
        "Literal : mirkiel::Literal value",
        "Variable : Token name",
        "Assign : Token name, std::shared_ptr<Expr> value",
        "Call : std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments"
    ]
    
    generate_header(os.path.join(output_dir, "expr.h"), types)

if __name__ == "__main__":
    main()