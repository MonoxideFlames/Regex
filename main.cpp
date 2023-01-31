#include <iostream>
#include "RegexScanner.hpp"
#include "ShuntingYard.hpp"

void printToken(int token){
	const char* str;
	char string[] = {0,0,0,0};
	static const char* table[] = {
		"$", "err", "*", "+", "?", "||", ".", "~","|", "&", "-", "(", ")","Aa","AZ", "az","spc", "09", "0f", "A#", "!?/%", "ALL", "NONE"
	};
	if(token < 256){
		str = table[token];
	}
	else if(token < 512){
		str = table[token - 254];
	}
	else if(token < 768){
		str = table[token - 512 + 13];
	}
	else{
		char c = (token - 768);
		string[0] = c;
		str = &(string[0]);
	}
	std::cout << '{' << str << '}';
}

int main(int argc, const char** argv){
	const char* regex;
	std::string input;
	if(argc != 2){
		std::cout << "unknown arguments, please try again:\n";
		std::cin >> input;
		regex = input.c_str();
	}
	else{
		regex = argv[1];	
	}
	ShuntingYardParser parser = ShuntingYardParser(regex);
	int token = parser.nextToken();
	while(token >= 256){
		printToken(token);
		token = parser.nextToken();
	}
	printToken(token);
	std::cout << '\n';
	
	Lexer lexer = Lexer(regex);
	token = lexer.consume();
	while(token >= 256){
		printToken(token);
		token = lexer.consume();
	}
	printToken(token);
	std::cout << '\n';
	return 0;
}
