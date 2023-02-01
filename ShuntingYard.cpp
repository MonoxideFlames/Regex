#include "ShuntingYard.hpp"
unsigned char ShuntingYardParser::getInformation(int token){
	static unsigned char propertiesTable[] = {
		UNARY_POST, UNARY_POST, UNARY_POST, 10, 20, 50, 30, 40, 60, LPAREN, RPAREN
	};
	// enum {KCLOSURE = 256, CLOSURE, OPTIONAL, ALTERNATE, CONCAT, COMP, UNION, INTERSECT, RANGE, LPAREN, RPAREN};
	if(token == STACK_BASE){
		return LPAREN;//any operations where the stack is empty will feel like they have a '(' below them, so precedence issues are not a problem. 
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
int ShuntingYardParser::nextToken(){
	while(1){
		unsigned char properties = getInformation(currentToken);
		switch(properties){
			case OPERAND://either case has the same procedure.
			case UNARY_POST:{
				int token = currentToken;
				currentToken = lexer.consume();
				return token;
			}
			case UNARY_PRE: //either case is exactly the same. Always push and get the next token
			case LPAREN: {
				stack.push(currentToken);
				currentToken = lexer.consume();
				break;
			}
			case RPAREN: {
				unsigned char propertiesTop = getInformation(stack.top());
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
				if(stack.top() != STACK_BASE){
					return stack.pop();
				}
				return currentToken;
			}
			default: {//this is the case of an operator
				unsigned char precedence = properties >> 1;
				unsigned char associativity = properties & 1;
				
				unsigned char propertiesTop = getInformation(stack.top());//get info about the token on top.
				if(propertiesTop == LPAREN || ((propertiesTop >> 1) < precedence)){//parentheses can be stacked upon. If it has less precedence, add it.
					stack.push(currentToken);
					currentToken = lexer.consume();
				}
				else if((propertiesTop >> 1) > precedence){ //precedece on the top is greater
					return stack.pop();//we return the operator on the stack.
				}
				else {
					if(properties & 1){
						//right associative:
						stack.push(currentToken);
						currentToken = lexer.consume();
					}
					else{
						//left associative:
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
