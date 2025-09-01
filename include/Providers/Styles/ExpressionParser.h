#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>

namespace ExpressionParser {
    using Value = std::variant<double, std::string, bool>;

    enum class TokenType {
        NUMBER,
        STRING,
        BOOLEAN,
        IDENTIFIER,
        PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
        AND, OR, NOT,
        EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
        LPAREN, RPAREN,
        END
    };
    
    struct Token {
        TokenType type;
        Value value;
        size_t pos;
    };
    
    enum class NodeType {
        NUMBER_LITERAL,
        STRING_LITERAL,
        BOOLEAN_LITERAL,
        BINARY_OP,
        UNARY_OP,
        VARIABLE
    };
    
    struct ASTNode {
        NodeType type;
        Token token;
        std::unique_ptr<ASTNode> left;
        std::unique_ptr<ASTNode> right;
        
        ASTNode(NodeType t, const Token& tok) : type(t), token(tok) {}
        ASTNode(NodeType t, const Token& tok, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r) 
            : type(t), token(tok), left(std::move(l)), right(std::move(r)) {}
    };
    
    class Lexer {
    private:
        std::string input;
        size_t pos;
        
    public:
        Lexer(const std::string& expr);
        Token nextToken();
        void skipWhitespace();
        Token readNumber();
        Token readString();
        Token readBoolean();
        Token readIdentifier();
        char peek();
        char advance();
    };
    
    class Parser {
    private:
        Lexer lexer;
        Token currentToken;
        
        void nextToken();
        std::unique_ptr<ASTNode> parseExpression();
        std::unique_ptr<ASTNode> parseLogicalOr();
        std::unique_ptr<ASTNode> parseLogicalAnd();
        std::unique_ptr<ASTNode> parseEquality();
        std::unique_ptr<ASTNode> parseComparison();
        std::unique_ptr<ASTNode> parseTerm();
        std::unique_ptr<ASTNode> parseFactor();
        std::unique_ptr<ASTNode> parseUnary();
        std::unique_ptr<ASTNode> parsePrimary();
        
    public:
        Parser(const std::string& expr);
        std::unique_ptr<ASTNode> parse();
    };
    
    class Evaluator {
    private:
        std::vector<std::pair<std::string, Value>> variables;
        
    public:
        Evaluator();
        void setVariable(const std::string& name, const Value& value);
        Value evaluate(const ASTNode* node);
        Value evaluateBinaryOp(const Token& op, const Value& left, const Value& right);
        Value evaluateUnaryOp(const Token& op, const Value& operand);
    };
    
    // ==========================
    Value evaluateExpression(const std::string& expr);
    Value evaluateExpression(const std::string& expr, const std::vector<std::pair<std::string, Value>>& vars);
    // ==========================
}
