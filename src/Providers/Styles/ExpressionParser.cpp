#include "ExpressionParser.h"
#include <stdexcept>
#include <cctype>

namespace ExpressionParser {
    
    // Lexer implementation
    Lexer::Lexer(const std::string& expr) : input(expr), pos(0) {}
    
    char Lexer::peek() {
        if (pos >= input.length()) return '\0';
        return input[pos];
    }
    
    char Lexer::advance() {
        if (pos >= input.length()) return '\0';
        return input[pos++];
    }
    
    void Lexer::skipWhitespace() {
        while (std::isspace(peek())) {
            advance();
        }
    }
    
    Token Lexer::readNumber() {
        size_t start = pos;
        bool hasDot = false;
        
        while (std::isdigit(peek()) || peek() == '.') {
            if (peek() == '.') {
                if (hasDot) break;
                hasDot = true;
            }
            advance();
        }
        
        std::string numStr = input.substr(start, pos - start);
        double value = std::stod(numStr);
        return {TokenType::NUMBER, value, start};
    }
    
    Token Lexer::readString() {
        char quote = advance(); // consume opening quote
        size_t start = pos;
        
        while (peek() != quote && peek() != '\0') {
            advance();
        }
        
        std::string str = input.substr(start, pos - start);
        advance(); // consume closing quote
        return {TokenType::STRING, str, start};
    }
    
    Token Lexer::readBoolean() {
        size_t start = pos;
        std::string ident;
        
        while (std::isalpha(peek())) {
            ident += advance();
        }
        
        if (ident == "true") {
            return {TokenType::BOOLEAN, true, start};
        } else if (ident == "false") {
            return {TokenType::BOOLEAN, false, start};
        }
        
        // If not boolean, it's an identifier
        pos = start;
        return readIdentifier();
    }
    
    Token Lexer::readIdentifier() {
        size_t start = pos;
        
        while (std::isalnum(peek()) || peek() == '_') {
            advance();
        }
        
        std::string ident = input.substr(start, pos - start);
        
        // Check for keywords
        if (ident == "and" || ident == "&&") {
            return {TokenType::AND, {}, start};
        } else if (ident == "or" || ident == "||") {
            return {TokenType::OR, {}, start};
        } else if (ident == "not" || ident == "!") {
            return {TokenType::NOT, {}, start};
        } else if (ident == "true") {
            return {TokenType::BOOLEAN, true, start};
        } else if (ident == "false") {
            return {TokenType::BOOLEAN, false, start};
        } else {
            return {TokenType::IDENTIFIER, ident, start};
        }
    }
    
    Token Lexer::nextToken() {
        skipWhitespace();
        
        if (pos >= input.length()) {
            return {TokenType::END, {}, pos};
        }
        
        char ch = peek();
        
        switch (ch) {
            case '+':
                advance();
                return {TokenType::PLUS, {}, pos - 1};
            case '-':
                advance();
                return {TokenType::MINUS, {}, pos - 1};
            case '*':
                advance();
                return {TokenType::MULTIPLY, {}, pos - 1};
            case '/':
                advance();
                return {TokenType::DIVIDE, {}, pos - 1};
            case '%':
                advance();
                return {TokenType::MODULO, {}, pos - 1};
            case '=':
                advance();
                if (peek() == '=') {
                    advance();
                    return {TokenType::EQUAL, {}, pos - 2};
                }
                return {TokenType::EQUAL, {}, pos - 1};
            case '!':
                advance();
                if (peek() == '=') {
                    advance();
                    return {TokenType::NOT_EQUAL, {}, pos - 2};
                }
                return {TokenType::NOT, {}, pos - 1};
            case '<':
                advance();
                if (peek() == '=') {
                    advance();
                    return {TokenType::LESS_EQUAL, {}, pos - 2};
                }
                return {TokenType::LESS, {}, pos - 1};
            case '>':
                advance();
                if (peek() == '=') {
                    advance();
                    return {TokenType::GREATER_EQUAL, {}, pos - 2};
                }
                return {TokenType::GREATER, {}, pos - 1};
            case '(':
                advance();
                return {TokenType::LPAREN, {}, pos - 1};
            case ')':
                advance();
                return {TokenType::RPAREN, {}, pos - 1};
            case '"':
            case '\'':
                return readString();
            default:
                if (std::isdigit(ch)) {
                    return readNumber();
                } else if (std::isalpha(ch)) {
                    return readBoolean();
                } else if (std::isalpha(ch) || ch == '_') {
                    return readIdentifier();
                } else {
                    throw std::runtime_error("Unexpected character: " + std::string(1, ch));
                }
        }
    }
    
    // Parser implementation
    Parser::Parser(const std::string& expr) : lexer(expr) {
        nextToken();
    }
    
    void Parser::nextToken() {
        currentToken = lexer.nextToken();
    }
    
    std::unique_ptr<ASTNode> Parser::parse() {
        auto result = parseExpression();
        if (currentToken.type != TokenType::END) {
            throw std::runtime_error("Unexpected token at end of expression");
        }
        return result;
    }
    
    std::unique_ptr<ASTNode> Parser::parseExpression() {
        return parseLogicalOr();
    }
    
    std::unique_ptr<ASTNode> Parser::parseLogicalOr() {
        auto left = parseLogicalAnd();
        
        while (currentToken.type == TokenType::OR) {
            Token op = currentToken;
            nextToken();
            auto right = parseLogicalAnd();
            left = std::make_unique<ASTNode>(NodeType::BINARY_OP, op, std::move(left), std::move(right));
        }
        
        return left;
    }
    
    std::unique_ptr<ASTNode> Parser::parseLogicalAnd() {
        auto left = parseEquality();
        
        while (currentToken.type == TokenType::AND) {
            Token op = currentToken;
            nextToken();
            auto right = parseEquality();
            left = std::make_unique<ASTNode>(NodeType::BINARY_OP, op, std::move(left), std::move(right));
        }
        
        return left;
    }
    
    std::unique_ptr<ASTNode> Parser::parseEquality() {
        auto left = parseComparison();
        
        while (currentToken.type == TokenType::EQUAL || currentToken.type == TokenType::NOT_EQUAL) {
            Token op = currentToken;
            nextToken();
            auto right = parseComparison();
            left = std::make_unique<ASTNode>(NodeType::BINARY_OP, op, std::move(left), std::move(right));
        }
        
        return left;
    }
    
    std::unique_ptr<ASTNode> Parser::parseComparison() {
        auto left = parseTerm();
        
        while (currentToken.type == TokenType::LESS || currentToken.type == TokenType::LESS_EQUAL ||
               currentToken.type == TokenType::GREATER || currentToken.type == TokenType::GREATER_EQUAL) {
            Token op = currentToken;
            nextToken();
            auto right = parseTerm();
            left = std::make_unique<ASTNode>(NodeType::BINARY_OP, op, std::move(left), std::move(right));
        }
        
        return left;
    }
    
    std::unique_ptr<ASTNode> Parser::parseTerm() {
        auto left = parseFactor();
        
        while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
            Token op = currentToken;
            nextToken();
            auto right = parseFactor();
            left = std::make_unique<ASTNode>(NodeType::BINARY_OP, op, std::move(left), std::move(right));
        }
        
        return left;
    }
    
    std::unique_ptr<ASTNode> Parser::parseFactor() {
        auto left = parseUnary();
        
        while (currentToken.type == TokenType::MULTIPLY || currentToken.type == TokenType::DIVIDE || 
               currentToken.type == TokenType::MODULO) {
            Token op = currentToken;
            nextToken();
            auto right = parseUnary();
            left = std::make_unique<ASTNode>(NodeType::BINARY_OP, op, std::move(left), std::move(right));
        }
        
        return left;
    }
    
    std::unique_ptr<ASTNode> Parser::parseUnary() {
        if (currentToken.type == TokenType::MINUS || currentToken.type == TokenType::NOT) {
            Token op = currentToken;
            nextToken();
            auto operand = parseUnary();
            return std::make_unique<ASTNode>(NodeType::UNARY_OP, op, std::move(operand), nullptr);
        }
        
        return parsePrimary();
    }
    
    std::unique_ptr<ASTNode> Parser::parsePrimary() {
        switch (currentToken.type) {
            case TokenType::NUMBER:
            case TokenType::STRING:
            case TokenType::BOOLEAN: {
                Token token = currentToken;
                nextToken();
                NodeType nodeType;
                if (token.type == TokenType::NUMBER) {
                    nodeType = NodeType::NUMBER_LITERAL;
                } else if (token.type == TokenType::STRING) {
                    nodeType = NodeType::STRING_LITERAL;
                } else {
                    nodeType = NodeType::BOOLEAN_LITERAL;
                }
                return std::make_unique<ASTNode>(nodeType, token);
            }
            
            case TokenType::IDENTIFIER: {
                Token token = currentToken;
                nextToken();
                return std::make_unique<ASTNode>(NodeType::VARIABLE, token);
            }
            
            case TokenType::LPAREN: {
                nextToken(); // consume '('
                auto expr = parseExpression();
                if (currentToken.type != TokenType::RPAREN) {
                    throw std::runtime_error("Expected ')'");
                }
                nextToken(); // consume ')'
                return expr;
            }
            
            default:
                throw std::runtime_error("Unexpected token in primary expression");
        }
    }
    
    // Evaluator implementation
    Evaluator::Evaluator() {}
    
    void Evaluator::setVariable(const std::string& name, const Value& value) {
        variables.push_back({name, value});
    }
    
    Value Evaluator::evaluate(const ASTNode* node) {
        switch (node->type) {
            case NodeType::NUMBER_LITERAL:
                return node->token.value;
                
            case NodeType::STRING_LITERAL:
                return node->token.value;
                
            case NodeType::BOOLEAN_LITERAL:
                return node->token.value;
                
            case NodeType::VARIABLE: {
                const std::string& varName = std::get<std::string>(node->token.value);
                for (const auto& var : variables) {
                    if (var.first == varName) {
                        return var.second;
                    }
                }
                throw std::runtime_error("Undefined variable: " + varName);
            }
            
            case NodeType::UNARY_OP:
                return evaluateUnaryOp(node->token, evaluate(node->left.get()));
                
            case NodeType::BINARY_OP:
                return evaluateBinaryOp(node->token, evaluate(node->left.get()), evaluate(node->right.get()));
        }
        
        throw std::runtime_error("Unknown node type");
    }
    
    Value Evaluator::evaluateUnaryOp(const Token& op, const Value& operand) {
        switch (op.type) {
            case TokenType::MINUS: {
                if (std::holds_alternative<double>(operand)) {
                    return Value(-std::get<double>(operand));
                }
                throw std::runtime_error("Unary minus can only be applied to numbers");
            }
            
            case TokenType::NOT: {
                if (std::holds_alternative<bool>(operand)) {
                    return Value(!std::get<bool>(operand));
                }
                throw std::runtime_error("Logical NOT can only be applied to boolean values");
            }
            
            default:
                throw std::runtime_error("Unknown unary operator");
        }
    }
    
    Value Evaluator::evaluateBinaryOp(const Token& op, const Value& left, const Value& right) {
        // Arithmetic operations
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            double l = std::get<double>(left);
            double r = std::get<double>(right);
            
            switch (op.type) {
                case TokenType::PLUS: return Value(l + r);
                case TokenType::MINUS: return Value(l - r);
                case TokenType::MULTIPLY: return Value(l * r);
                case TokenType::DIVIDE: 
                    if (r == 0) throw std::runtime_error("Division by zero");
                    return Value(l / r);
                case TokenType::MODULO: return Value(static_cast<double>(static_cast<int>(l) % static_cast<int>(r)));
                case TokenType::LESS: return Value(l < r);
                case TokenType::LESS_EQUAL: return Value(l <= r);
                case TokenType::GREATER: return Value(l > r);
                case TokenType::GREATER_EQUAL: return Value(l >= r);
                case TokenType::EQUAL: return Value(l == r);
                case TokenType::NOT_EQUAL: return Value(l != r);
                default: break;
            }
        }
        
        // String operations
        if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
            const std::string& l = std::get<std::string>(left);
            const std::string& r = std::get<std::string>(right);
            
            switch (op.type) {
                case TokenType::PLUS: return Value(l + r); // string concatenation
                case TokenType::LESS: return Value(l < r);
                case TokenType::LESS_EQUAL: return Value(l <= r);
                case TokenType::GREATER: return Value(l > r);
                case TokenType::GREATER_EQUAL: return Value(l >= r);
                case TokenType::EQUAL: return Value(l == r);
                case TokenType::NOT_EQUAL: return Value(l != r);
                default: break;
            }
        }
        
        // Boolean operations
        if (std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right)) {
            bool l = std::get<bool>(left);
            bool r = std::get<bool>(right);
            
            switch (op.type) {
                case TokenType::AND: return Value(l && r);
                case TokenType::OR: return Value(l || r);
                case TokenType::EQUAL: return Value(l == r);
                case TokenType::NOT_EQUAL: return Value(l != r);
                default: break;
            }
        }
        
        // Mixed comparisons
        switch (op.type) {
            case TokenType::EQUAL:
                return Value(left == right);
            case TokenType::NOT_EQUAL:
                return Value(left != right);
            default:
                break;
        }
        
        throw std::runtime_error("Invalid operation between given types");
    }
    
    // Main evaluation functions
    Value evaluateExpression(const std::string& expr) {
        Parser parser(expr);
        auto ast = parser.parse();
        Evaluator evaluator;
        return evaluator.evaluate(ast.get());
    }
    
    Value evaluateExpression(const std::string& expr, const std::vector<std::pair<std::string, Value>>& vars) {
        Parser parser(expr);
        auto ast = parser.parse();
        Evaluator evaluator;
        
        for (const auto& var : vars) {
            evaluator.setVariable(var.first, var.second);
        }
        
        return evaluator.evaluate(ast.get());
    }
}