#ifndef ShuntingYard_hpp
#define ShuntingYard_hpp
#include "RegexScanner.hpp"
#include "Stack.h"
class ShuntingYardParser {
	Stack<int> stack;
	int currentToken;
	Lexer lexer;
  	//The parity represents associativity 0 for left, 1 for right, the numerical value is its precedence.
	//A few special codes: 
	enum specialCodes {
		INFIX_DELIMITER = 248, UNARY_PRE = 249, UNARY_POST, OPERAND, LPAREN, RPAREN, END_OR_ERROR, STACK_BASE
	};
	unsigned char getInformation(int token);
	public:
	int next();
	ShuntingYardParser(const char* input): lexer(input){
		currentToken = lexer.consume();
		stack.push(STACK_BASE);
	}
};
#endif
