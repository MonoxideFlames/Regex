#ifndef RegexScanner_hpp
#define RegexScanner_hpp
#include <iostream>
class Lexer {
   const char* input;
   int current;
   bool expectingOperator;
	char state;
	public:
	enum states {
REGEX = 0, CHARSET = 1
	};
	//all our symbols are made so that all meaningful ranges are 256 in size. 0 - 255 are for control tokens, 256 - 511 are for operators, 512 - 767 are for predefined charset codes, 768 - 1023 are for literals.
	enum control {
		END = 0, UNEXPECTED
	};
	enum ops_delim {
		KCLOSURE = 256, CLOSURE = 257, OPTIONAL = 258, ALTERNATE = 259, CONCAT = 260, COMPLEMENT = 261, UNION = 262, INTERSECT = 263, RANGE = 264,REGEX_SEPARATOR = 265, LPAREN = 266, RPAREN = 267
	};	
	enum predefCharsets {
		ALPHA = 512, UPPER, LOWER, WHITESPACE, DIGIT, HEXDIGIT, WORD, PUNCT, ALL, NONE, STARTLINE, ENDLINE
	};
	//any literal char is represented by 768 + its ascii value.
private:
	int consumeRegex();
	int consumeSet();
	char escapeSequence();
	int consumePredefSet();
	int consumeChar();
	void removeWhitespace();
public:
	Lexer(const char* str);
	int consume();
};
#endif
