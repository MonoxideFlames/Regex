#include "ShuntingYard.hpp"
unsigned char ShuntingYardParser::getInformation(int token){
	static unsigned char propertiesTable[] = {
		UNARY_POST, UNARY_POST, UNARY_POST, 20, 30, 60, 40, 50, 70, INFIX_DELIMITER, LPAREN, RPAREN 
	};
	// enum {KCLOSURE = 256, CLOSURE, OPTIONAL, ALTERNATE, CONCAT, COMP, UNION, INTERSECT, RANGE, LPAREN, RPAREN};
	if(token == STACK_BASE){
		return LPAREN;//any operations where the stack is empty will feel like they have a ( below them, so precedence issues are not a problem. 
	}
	else if(token < 256){
		return END_OR_ERROR;
	}
	else if(token < 512){
		return propertiesTable[token - 256];
	}
	else {
		return OPERAND;
	}
}
int ShuntingYardParser::next(){
	while(1){
		unsigned char properties = getInformation(currentToken);
		switch(properties){
			case OPERAND: 
			case UNARY_POST:{
				int token = currentToken;
				currentToken = lexer.consume();
				return token;
			}
			case UNARY_PRE: 
			case LPAREN: {
				stack.push(currentToken);
				currentToken = lexer.consume();
				break;
			}
			case INFIX_DELIMITER:{
				unsigned char propertiesTop = getInformation(stack.peek());
				if(stack.size() == 1){
					int token = currentToken;
					currentToken = lexer.consume();
					return token;
				}
				else if(propertiesTop == LPAREN){
					stack.pop();
					break;
				}
				else{
					return stack.pop();
				}
			}
			case RPAREN: {
				unsigned char propertiesTop = getInformation(stack.peek());
				if(propertiesTop == LPAREN){
					currentToken = lexer.consume();
					stack.pop();
					break;
				}
				else{
					return stack.pop();
				}
			}
			case END_OR_ERROR: {
				if(stack.peek() != STACK_BASE){
					return stack.pop();
				}
				return currentToken;
			}
			default: {//this is the case of an operator
				unsigned char precedence = properties >> 1;
				unsigned char associativity = properties & 1;
				
				unsigned char propertiesTop = getInformation(stack.peek());//get info about the character on top.
				if(propertiesTop == LPAREN || (propertiesTop >> 1) < precedence){//if the top is a ( or if its of lower precedence:
					stack.push(currentToken);
					currentToken = lexer.consume();
				}
				else if((propertiesTop >> 1) > precedence){ //precedece on the top is greater
					return stack.pop();//we return the operator on the stack.
				}
				else {
					if(properties & 1){//right associative:
						stack.push(currentToken);
						currentToken = lexer.consume();
					}
					else{
						int token = stack.pop();
						stack.push(currentToken);
						currentToken = lexer.consume();
						return token;
					}	
				}
			}
		}
	}
}
