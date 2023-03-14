#include "RegexToNFA.hpp"
static void set_toState(StackOperand& a){
	NFAState *start = new NFAState(NFAState::CHAR), *end = new NFAState(NFAState::END);//create a start and end state
	start->charset[0] = a.charset[0];//copy the set into the start transition set.
	start->charset[1] = a.charset[1];
	start->out[0] = end;//set a transition to end.
	a.start = start;//set each value in the fragment to package the state machine.
	a.end = end;
	a.type = StackOperand::STATE;
}
//will cause problems if you send charsets with multiple characters.
static char rangeType(unsigned long long a0, unsigned long long a1, unsigned long long b0, unsigned long long b1){
	if(a1 == 0 && b1 == 0){
		return 0;
	}
	if(a1 == 0 && b0 == 0){
		return 1;
	}
	if(a0 == 0 && b1 == 0){
		return 2;
	}
	if(a0 == 0 && b0 == 0){
		return 3;
	}
}
static void set_range(StackOperand& a, StackOperand& b){
	unsigned long long p, q;
	char type = rangeType(a.charset[0], a.charset[1], b.charset[0], b.charset[1]);
	switch(type){
		case 0:{
			if(b.charset[0] < a.charset[0]) {
				p = b.charset[0];
				q = a.charset[0];
			}
			else{
				p = a.charset[0];
				q = b.charset[0];
			}
			p |= p << 1;
			p |= p << 2;
			p |= p << 4;
			p |= p << 8;
			p |= p << 16;
			p |= p << 32;

			q |= q >> 1;
			q |= q >> 2;
			q |= q >> 4;
			q |= q >> 8;
			q |= q >> 16;
			q |= q >> 32;
			a.charset[0] = p & q;
			break;
		}
		case 1: {
			a.charset[0] |= a.charset[0] << 1;
			a.charset[0] |= a.charset[0] << 2;
			a.charset[0] |= a.charset[0] << 4;
			a.charset[0] |= a.charset[0] << 8;
			a.charset[0] |= a.charset[0] << 16;
			a.charset[0] |= a.charset[0] << 32;
		
			a.charset[1] = b.charset[1];
			a.charset[1] |= a.charset[1] >> 1;
			a.charset[1] |= a.charset[1] >> 2;
			a.charset[1] |= a.charset[1] >> 4;
			a.charset[1] |= a.charset[1] >> 8;
			a.charset[1] |= a.charset[1] >> 16;
			a.charset[1] |= a.charset[1] >> 32;
			break; 
		}
		case 2:{
			a.charset[1] |= a.charset[1] >> 1;
			a.charset[1] |= a.charset[1] >> 2;
			a.charset[1] |= a.charset[1] >> 4;
			a.charset[1] |= a.charset[1] >> 8;
			a.charset[1] |= a.charset[1] >> 16;
			a.charset[1] |= a.charset[1] >> 32;
		
			a.charset[0] = b.charset[0];
			a.charset[0] |= a.charset[0] << 1;
			a.charset[0] |= a.charset[0] << 2;
			a.charset[0] |= a.charset[0] << 4;
			a.charset[0] |= a.charset[0] << 8;
			a.charset[0] |= a.charset[0] << 16;
			a.charset[0] |= a.charset[0] << 32;
		 	break;		 
		}
		case 3:{
			if(b.charset[1] < a.charset[1]) {
				p = b.charset[1];
				q = a.charset[1];
			}
			else{
				p = a.charset[1];
				q = b.charset[1];
			}
			p |= p << 1;
			p |= p << 2;
			p |= p << 4;
			p |= p << 8;
			p |= p << 16;
			p |= p << 32;

			q |= q >> 1;
			q |= q >> 2;
			q |= q >> 4;
			q |= q >> 8;
			q |= q >> 16;
			q |= q >> 32;
			a.charset[1] = p & q;
			break;
		}
	}	
}
static StackOperand predefCharset(int tok){
	static unsigned long long charsetLookup[24] = {
		0x0000'0000'0000'0000, 0x07ff'fffe'07ff'fffe, //alpha
		0x0000'0000'0000'0000, 0x0000'0000'07ff'fffe, //upper
		0x0000'0000'0000'0000, 0x07ff'fffe'0000'0000, //lower
	 	0x0000'0001'0000'3e00, 0x0                  , //whitespace
		0x03ff'0000'0000'0000, 0x0000'0000'0000'0000, //digit
		0x03ff'0000'0000'0000, 0x07e0'0000'07e0'0000, //hexdigit
		0x03ff'0000'0000'0000, 0x07ff'fffe'87ff'fffe, //word
	   0xfc00'fffe'0000'0000, 0x7800'0001'f800'0001, //punct
		0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff, //all
		0x0                  , 0x0                  , //none
		0x2402               , 0x0                  , //Start line
		0x2401               , 0x0                    //EndLine
	};
	StackOperand operand;
	operand.type = StackOperand::SET;
	operand.charset[0] = charsetLookup[2*(tok - 512)];
	operand.charset[1] = charsetLookup[2*(tok - 512) + 1];//set teh charset
	return operand;
}
static StackOperand singleCharacter(int tok){
	StackOperand charset;
	charset.type = StackOperand::SET;
	charset.charset[0] = 0;//set the set to 0
	charset.charset[1] = 0;
	tok -= 768;//get the character
	if(tok >= 64){
		charset.charset[1] = 1ll << (tok - 64);
	}
	else{
		charset.charset[0] = 1ll << (tok);
	}
	return charset;
}

static void state_concatenate(StackOperand& a, StackOperand& b, unsigned int& numStates){
	if(a.type != StackOperand::STATE){
		set_toState(a);
		numStates+=2;
	}
	if(b.type != StackOperand::STATE){
		set_toState(b);
		numStates+=2;
	}
	a.end->type = NFAState::EPSILON1;//make a former end state a connecting state to the start of b.
	a.end->out[0] = b.start;
	a.end = b.end;//set the end state of a to the end state of b
}

static void state_closure(StackOperand& a, unsigned int& numStates){
	if(a.type != StackOperand::STATE){
		set_toState(a);
		numStates+=2;
	}
	a.end->type = NFAState::EPSILON2;
	a.end->out[0] = a.start;
	NFAState* end = new NFAState(NFAState::END);
	numStates++;
	a.end->out[1] = end;
	a.end = end;
}

static void state_optional(StackOperand& a, unsigned int& numStates){
	if(a.type != StackOperand::STATE){
		set_toState(a);
		numStates+=2;
	}
	NFAState* start = new NFAState(NFAState::EPSILON2);//make a new start state of type epsilon 2
	numStates++;
	start->out[0] = a.start;//set its out states to teh start of the state machine, and the end.
	start->out[1] = a.end;
	a.start = start;//make it the new start state of a
}

static void state_kclosure(StackOperand& a, unsigned int& numStates){
	if(a.type != StackOperand::STATE){
		set_toState(a);
		numStates+=2;
	}
	NFAState* start = new NFAState(NFAState::EPSILON2);
	NFAState* end = new NFAState(NFAState::END);
	numStates+=2;
	a.end->type = NFAState::EPSILON2;//turn end state to epsion 2
	a.end->out[0] = a.start;//connect it to the new end, and the original start
	a.end->out[1] = end;

	start->out[0] = a.start;//have 2 transitions to the start and the end state.
	start->out[1] = end;

	a.start = start;//make a new start and end state.
	a.end = end;
}

static void state_alternate(StackOperand& a, StackOperand& b, unsigned int& numStates){
	if(a.type != StackOperand::STATE){
		set_toState(a);
		numStates+=2;
	}
	if(b.type != StackOperand::STATE){
		set_toState(b);
		numStates+=2;
	}
	NFAState* start = new NFAState(NFAState::EPSILON2);
	NFAState* end = new NFAState(NFAState::END);
	numStates += 2;
	start->out[0] = a.start;//connect the next nodes as epsilon transitions to the first node.
	start->out[1] = b.start;
	a.end->type = NFAState::EPSILON1;//take the current end nodes and add thenew end to their ending states.
	b.end->type = NFAState::EPSILON1;
	a.end->out[0] = end;
	b.end->out[0] = end;

	a.start = start;//add new start and end states
	a.end = end;
}
static void set_union(StackOperand& a, StackOperand& b){
	a.charset[0] |= b.charset[0];
	a.charset[1] |= b.charset[1];
}
static void set_intersection(StackOperand& a, StackOperand& b){
	a.charset[0] &= b.charset[0];
	a.charset[1] &= b.charset[1];
}
static void set_complement(StackOperand& a){
	a.charset[0] = ~a.charset[0];
	a.charset[1] = ~a.charset[1];
}

void RegexToNFAConverter::consume(NFA& nfa){
	int token = parser.next();
	if(token == Lexer::END){
		return;
	}
	while(token > 255 && token != Lexer::REGEX_SEPARATOR){
		if(token > 767){
			stack.push(singleCharacter(token));
		}
		else if(token > 511){
			stack.push(predefCharset(token));
		}
		else{		
			switch(token){
				case Lexer::KCLOSURE:
					state_kclosure(stack[0], nfa.numStates);
					break;
				case Lexer::CLOSURE:
					state_closure(stack[0], nfa.numStates);
					break;
				case Lexer::OPTIONAL:
					state_optional(stack[0], nfa.numStates);
					break;
				case Lexer::ALTERNATE:
					state_alternate(stack[1], stack[0], nfa.numStates);
					stack.pop();
					break;
				case Lexer::CONCAT:
					state_concatenate(stack[1], stack[0], nfa.numStates);
					stack.pop();
					break;
				case Lexer::UNION:
					set_union(stack[1], stack[0]);
					stack.pop();
					break;
				case Lexer::RANGE:
					set_range(stack[1], stack[0]);
					stack.pop();
					break;
				case Lexer::INTERSECT:
					set_intersection(stack[1], stack[0]);
					stack.pop();
					break;
				case Lexer::COMPLEMENT:
					set_complement(stack[0]);
					break;
			}
		}
		token = parser.next();
	}
	if(stack[0].type == StackOperand::SET){
		set_toState(stack[0]);
		nfa.numStates = 2;
	}
	nfa.start = stack[0].start;
}
