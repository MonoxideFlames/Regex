#include "RegexScanner.hpp"
Lexer::Lexer(const char* str){
   input = str;
   current = 0;
	state = REGEX;
	expectingOperator = false;
}

int Lexer::consume(){
//we expect a character operand first, either a range or literal or parenthetical subexpression. Anything after an operand, we expect an operator. (The concatenate is made explicit)
	switch(state) {
		case REGEX :
			return consumeRegex();
		default:
			return consumeSet();
	}
}

int Lexer::consumeRegex(){
	removeWhitespace();
	char c = input[current];
	current++;
	
	if(expectingOperator){
		switch(c) {
			case ')' : {
				return RPAREN;
			}
			case '|' :
				expectingOperator = false;
				return ALTERNATE;
			case '*' :
				return KCLOSURE;
			case '+' :
				return CLOSURE;
			case '?' :
				return OPTIONAL;
			case '\0': {
				current--;
				return END;
			}
			default: {
				current--;//if we dont find a regex operator, do not move forward, and return the concat operator
				expectingOperator = false;
				return CONCAT;
			}
		}
	}
	else{
		int temp;
		switch(c){
			case '[' :{
				state = CHARSET;
				temp = LPAREN;
				break;
			}
			case '(' :{
				return LPAREN;
			}
			case ')' : {
				expectingOperator = true;
				return RPAREN;
			}
			case '\0': {
				current--;
				return END;
			}
			case '#': {
				temp = consumePredefSet();
				expectingOperator = true;
				break;
			}
			default: {
				current--;
				temp = consumeChar();
				expectingOperator = true;
			}
		}
		return temp;
	}
}

int Lexer::consumeSet() {
	removeWhitespace();
	char c = input[current];
	current++;
	switch(c) {
		case ']' :
			state = REGEX;
		case ')' :
			expectingOperator = true;
			return RPAREN;
		case '(' :
			return LPAREN;
		case '|' : {
			return UNION;
		}
		case '&' : {
			return INTERSECT;
		}
		case '~' : {
			return COMPLEMENT;
		}
		case '-': {
			return RANGE;
		}
		case '\0': {
			current--;
			return END;
		}
		case '#' :
			return consumePredefSet();
		default:{
			current--;
			return consumeChar();
		}
	}
}

int Lexer::consumeChar(){
	removeWhitespace();
	char c = input[current];
   current++;
	if(c == '\\'){
		c = escapeSequence();
	}
	return 768 + c;
}

char Lexer::escapeSequence(){
	char next = input[current];
	current++; // /.e -> /e.
	switch(next) {
		case 's':
			return ' ';
		case 'a':
			return '\a';
		case 'b':
			return '\b';
		case 'f':
			return '\f';
		case 'n':
			return '\n';
		case 'r':
			return '\r';
		case 't':
			return '\t';
		case 'v':
			return '\v';
		default:
			return next;   //eg \\ is backslash; and \? is question mark. It escapes the optional character postix quantifier
	}
}
int Lexer::consumePredefSet(){
	removeWhitespace();
	char c = input[current];
	current++;
	if(c > 64 && c < 91){
		c += 32;//convert c to lowercase
	}
	switch(c){
		//ALPHA = 512, UPPER, LOWER, WHITESPACE, DIGIT, HEXDIGIT, WORD, PUNCT, ALL, NONE} 
		case 'a':
			return ALPHA;
		case 'd':
			return DIGIT;
		case 'w':
			return WORD;
		case 'x':
			return HEXDIGIT;
		case 's':
			return WHITESPACE;
		case 'u':
			return UPPER;
		case 'l':
			return LOWER;
		case '!':
		case 'p':
			return PUNCT;
		case '.':
		case 'e':
			return ALL;
		case '0':
		case 'z':
			return NONE;
		default:
			return UNEXPECTED;
	}
}
void Lexer::removeWhitespace(){
   char c = input[current];
   while((c == ' ' || c == '\t') || ((c == '\r' || c == '\n') || (c == '\v' || c == '\f'))){
      current++;
      c = input[current];
   }
}
//removes whitespace in the expression. advances to the first nonwhitespace character after.
