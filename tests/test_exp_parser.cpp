#include "ExpressionParser.h"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace ExpressionParser;

    
void testBasicArithmetic() {
    std::cout << "Testing basic arithmetic...\n";
    
    auto result1 = evaluateExpression("2 + 3");
    assert(std::abs(std::get<double>(result1) - 5.0) < 1e-9);
    
    auto result2 = evaluateExpression("10 - 4");
    assert(std::abs(std::get<double>(result2) - 6.0) < 1e-9);
    
    auto result3 = evaluateExpression("3 * 4");
    assert(std::abs(std::get<double>(result3) - 12.0) < 1e-9);
    
    auto result4 = evaluateExpression("15 / 3");
    assert(std::abs(std::get<double>(result4) - 5.0) < 1e-9);
    
    auto result5 = evaluateExpression("17 % 5");
    assert(std::abs(std::get<double>(result5) - 2.0) < 1e-9);
    
    std::cout << "Basic arithmetic tests passed!\n";
}

void testArithmeticPrecedence() {
    std::cout << "Testing arithmetic precedence...\n";
    
    auto result1 = evaluateExpression("2 + 3 * 4");
    assert(std::abs(std::get<double>(result1) - 14.0) < 1e-9);
    
    auto result2 = evaluateExpression("(2 + 3) * 4");
    assert(std::abs(std::get<double>(result2) - 20.0) < 1e-9);
    
    auto result3 = evaluateExpression("10 - 2 * 3");
    assert(std::abs(std::get<double>(result3) - 4.0) < 1e-9);
    
    auto result4 = evaluateExpression("20 / 4 + 1");
    assert(std::abs(std::get<double>(result4) - 6.0) < 1e-9);
    
    std::cout << "Arithmetic precedence tests passed!\n";
}

void testStringOperations() {
    std::cout << "Testing string operations...\n";
    
    auto result1 = evaluateExpression("\"Hello\" + \" World\"");
    assert(std::get<std::string>(result1) == "Hello World");
    
    auto result2 = evaluateExpression("\"abc\" < \"def\"");
    assert(std::get<bool>(result2) == true);
    
    auto result3 = evaluateExpression("\"xyz\" > \"abc\"");
    assert(std::get<bool>(result3) == true);
    
    auto result4 = evaluateExpression("\"test\" == \"test\"");
    assert(std::get<bool>(result4) == true);
    
    auto result5 = evaluateExpression("\"hello\" != \"world\"");
    assert(std::get<bool>(result5) == true);
    
    std::cout << "String operations tests passed!\n";
}

void testLogicalOperations() {
    std::cout << "Testing logical operations...\n";
    
    auto result1 = evaluateExpression("true and true");
    assert(std::get<bool>(result1) == true);
    
    auto result2 = evaluateExpression("true and false");
    assert(std::get<bool>(result2) == false);
    
    auto result3 = evaluateExpression("false or true");
    assert(std::get<bool>(result3) == true);
    
    auto result4 = evaluateExpression("false or false");
    assert(std::get<bool>(result4) == false);
    
    auto result5 = evaluateExpression("not true");
    assert(std::get<bool>(result5) == false);
    
    auto result6 = evaluateExpression("not false");
    assert(std::get<bool>(result6) == true);
    
    std::cout << "Logical operations tests passed!\n";
}

void testComparisonOperations() {
    std::cout << "Testing comparison operations...\n";
    
    auto result1 = evaluateExpression("5 > 3");
    assert(std::get<bool>(result1) == true);
    
    auto result2 = evaluateExpression("2 < 7");
    assert(std::get<bool>(result2) == true);
    
    auto result3 = evaluateExpression("4 <= 4");
    assert(std::get<bool>(result3) == true);
    
    auto result4 = evaluateExpression("6 >= 5");
    assert(std::get<bool>(result4) == true);
    
    auto result5 = evaluateExpression("3 == 3");
    assert(std::get<bool>(result5) == true);
    
    auto result6 = evaluateExpression("4 != 5");
    assert(std::get<bool>(result6) == true);
    
    std::cout << "Comparison operations tests passed!\n";
}

void testComplexExpressions() {
    std::cout << "Testing complex expressions...\n";
    
    auto result1 = evaluateExpression("(5 + 3) * 2 > 10 and true");
    assert(std::get<bool>(result1) == true);
    
    auto result2 = evaluateExpression("\"Hello\" + \" \" + \"World\" == \"Hello World\"");
    assert(std::get<bool>(result2) == true);
    
    auto result3 = evaluateExpression("not (3 < 2) or false");
    assert(std::get<bool>(result3) == true);
    
    std::cout << "Complex expressions tests passed!\n";
}

void testVariables() {
    std::cout << "Testing variables...\n";
    
    std::vector<std::pair<std::string, Value>> vars = {
        {"x", 10.0},
        {"y", 5.0},
        {"name", std::string("John")},
        {"flag", true}
    };
    
    auto result1 = evaluateExpression("x + y", vars);
    assert(std::abs(std::get<double>(result1) - 15.0) < 1e-9);
    
    auto result2 = evaluateExpression("name == \"John\"", vars);
    assert(std::get<bool>(result2) == true);
    
    auto result3 = evaluateExpression("flag and x > y", vars);
    assert(std::get<bool>(result3) == true);
    
    std::cout << "Variables tests passed!\n";
}

void testUnaryOperations() {
    std::cout << "Testing unary operations...\n";
    
    auto result1 = evaluateExpression("-5");
    assert(std::abs(std::get<double>(result1) - (-5.0)) < 1e-9);
    
    auto result2 = evaluateExpression("--3");
    assert(std::abs(std::get<double>(result2) - 3.0) < 1e-9);
    
    std::cout << "Unary operations tests passed!\n";
}

void testMixedTypeComparisons() {
    std::cout << "Testing mixed type comparisons...\n";
    
    auto result1 = evaluateExpression("5 == 5.0");
    assert(std::get<bool>(result1) == true);
    
    auto result2 = evaluateExpression("\"test\" != 123");
    assert(std::get<bool>(result2) == true);
    
    std::cout << "Mixed type comparisons tests passed!\n";
}

void testBooleanLiterals() {
    std::cout << "Testing boolean literals...\n";
    
    auto result1 = evaluateExpression("true");
    assert(std::get<bool>(result1) == true);
    
    auto result2 = evaluateExpression("false");
    assert(std::get<bool>(result2) == false);
    
    auto result3 = evaluateExpression("true == true");
    assert(std::get<bool>(result3) == true);
    
    auto result4 = evaluateExpression("true != false");
    assert(std::get<bool>(result4) == true);
    
    std::cout << "Boolean literals tests passed!\n";
}

void runAllTests() {
    std::cout << "Starting ExpressionParser tests...\n\n";
    
    try {
        testBasicArithmetic();
        std::cout << "\n";
        
        testArithmeticPrecedence();
        std::cout << "\n";
        
        testStringOperations();
        std::cout << "\n";
        
        testLogicalOperations();
        std::cout << "\n";
        
        testComparisonOperations();
        std::cout << "\n";
        
        testComplexExpressions();
        std::cout << "\n";
        
        testVariables();
        std::cout << "\n";
        
        testUnaryOperations();
        std::cout << "\n";
        
        testMixedTypeComparisons();
        std::cout << "\n";
        
        testBooleanLiterals();
        std::cout << "\n";
        
        std::cout << "All tests passed successfully!\n";
    } catch (const std::exception& e) {
        std::cout << "Test failed with error: " << e.what() << "\n";
    }
}

int main() {
    runAllTests();
    return 0;
}