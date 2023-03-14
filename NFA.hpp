//
//  NFA.hpp
//  CPPProject
//
//  Created by Adith Talupuru on 12/24/22.
//

#ifndef NFA_hpp
#include <iostream>
#define NFA_hpp
#include "Stack.h"
struct NFAState{
	enum types {
		EPSILON1, EPSILON2, CHAR, END
	};
	unsigned	long long charset[2];
	NFAState* out[2];//the next states after the 
	unsigned int id;
	unsigned char type;
	static unsigned int idCounter;
	NFAState(char type){
		this->type = type;
		id = idCounter++;
	}
	bool contains(char c);
};

struct NFA {
	Stack<NFAState*> activeStates;
	NFAState *start;
	NFAState** stateArray;
	unsigned int numStates;
	NFA();
	~NFA(){
	}
	void clear();//clears the states from the NFA.
	void print();
	char eClosure(Stack<NFAState*>& newStates);
	char simulate(char c);//1 means it ended successfully, 0 means it ended unsuccessfully (dead state), 2 means its not in either.
	void initiate();
	char addClosure(NFAState* state, bool* isActive);
	void populateStates();
};

#endif /* NFA_hpp */
