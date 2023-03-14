#ifndef RegexToNFA_hpp
#define RegexToNFA_hpp

#include "Stack.h"
#include "NFA.hpp"
#include "ShuntingYard.hpp"
struct StackOperand {
	char type;
	enum types {
		STATE = 0, SET
	};
	union {
		unsigned long long charset[2];
		struct {
			NFAState *start, *end;
		};
	};
};

struct RegexToNFAConverter {
	ShuntingYardParser parser;

	void consume(NFA& nfa);
	Stack<StackOperand> stack;
	RegexToNFAConverter(const char* str): parser(str){}
};
#endif
