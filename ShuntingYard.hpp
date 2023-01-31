#ifndef ShuntingYard_hpp
#define ShuntingYard_hpp
#include "RegexScanner.hpp"
class Stack {
	unsigned int topIndex;
	unsigned int capacity;
	int* arr;
	void resize(unsigned int newCap) {
		int* newArr = new int[newCap];
		unsigned int newTop = newCap - capacity + topIndex;
		for(unsigned int i = topIndex, j = newTop; i != capacity; i++, j++){
			newArr[j] = arr[i];
		}

		topIndex = newTop;
		capacity = newCap;
		delete[] arr;
		arr = newArr;
	}
	public:
	unsigned int size(){
		return capacity - topIndex; 
	}
	void push(int p){
		if(topIndex == 0){
			resize(capacity * 2);
		}
		topIndex--;
		arr[topIndex] = p;
	}
	int pop(){
		if(topIndex > 8 && (topIndex == 3 * capacity / 4)) {
			resize(capacity/4);
		}
		if(capacity == topIndex){
			return arr[topIndex];
		}
		return arr[topIndex++];
	}
	int top(){
		return arr[topIndex];
	}

	Stack(){
		capacity = 8;
		topIndex = 8;
		arr = new int[8];
	}
	~Stack(){
		delete[] arr;
	}
//Add stack methods
};
class ShuntingYardParser {
	Stack stack;
	int currentToken;
	Lexer lexer;
  	//The parity represents associativity 0 for left, 1 for right, the numerical value is its precedence.
	//A few special codes: 
	enum specialCodes {
		UNARY_PRE = 249, UNARY_POST, OPERAND, LPAREN, RPAREN, END_OR_ERROR, STACK_BASE
	};
	unsigned char getInformation(int token);
	public:
	int nextToken();
	ShuntingYardParser(const char* input): lexer(input){
		currentToken = lexer.consume();
		stack.push(STACK_BASE);
	}
};
#endif
