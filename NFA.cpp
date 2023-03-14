//
//  NFA.cpp
//  CPPProject
//
//  Created by Adith Talupuru on 12/24/22.
//
#include "NFA.hpp"
static void printState(NFAState* s){
	static const char* lookup[4] = {
		"EPSILON1", "EPSILON2", "CHAR", "END"
	};
	std::cout << "{ id: " << s->id << ", type: " << lookup[s->type];
	if(s->type == NFAState::CHAR){
		std::cout << ", transitions: [";
		for(char c = 0; c < 127; c++){
			if(s->contains(c)){
				std::cout << c << ' ';
			}
		}
		std::cout << "] -> " << s->out[0]->id;
	}
	if(s->type == NFAState::EPSILON1 || s->type == NFAState::EPSILON2){
		std::cout << ", transitions: " << s->out[0]->id;
	}
	if(s->type == NFAState::EPSILON2){
		std::cout << " " << s->out[1]->id;
	}
	std::cout << "}\n";
}
void NFA::print(){
	if(numStates == 0){
		std::cout << "EMPTY NFA\n";
		return;
	}
	std::cout << "Start: " << start->id << '\n';
	for(unsigned int i = 0; i < numStates; i++){
		printState(stateArray[i]);	
	}
}
unsigned int NFAState::idCounter = 0;

bool NFAState::contains(char c){
	unsigned long long test;
	if(type != CHAR) return false;
	if(c < 64){
		test = 1ll << c;
		if(charset[0] == (charset[0] | test)){
			return true;
		}
		return false;
	}
	else if(c >= 64) {
		test = 1ll << (c - 64);
		if(charset[1] == (charset[1] | test)){ 
			return true;
		}
		return false;
	}

	return false;
}
static void populationRoutine(bool* marked, NFAState* s, NFAState** array){
	if(!marked[s->id]){
		marked[s->id] = true;
		array[s->id] = s;
		switch(s->type){
			case NFAState::EPSILON2:
				populationRoutine(marked, s->out[1], array);
			case NFAState::EPSILON1:
			case NFAState::CHAR:
				populationRoutine(marked, s->out[0], array);
		}
	}
}

void NFA::populateStates(){
	if(numStates == 0){
		return;
	}
	stateArray = new NFAState*[numStates];
	bool marked[numStates];
	for(unsigned int i = 0; i < numStates; i++)
		marked[i] = false;
	populationRoutine(marked, start, stateArray);
}

void NFA::clear(){
	for(unsigned int i = 0; i < numStates; i++){
		delete stateArray[i];//deletes every element in the vector
	}
	delete[] stateArray;
	stateArray = nullptr;
}

char NFA::addClosure(NFAState* state, bool* isActive){
	if(!isActive[state->id]) {
		activeStates.push(state);
		isActive[state->id] = true;

		bool endStateAdded = false;
		switch(state->type){
			case NFAState::EPSILON2:
				endStateAdded = addClosure(state->out[1], isActive) || endStateAdded;
			case NFAState::EPSILON1:
				endStateAdded = addClosure(state->out[0], isActive) || endStateAdded;
				return endStateAdded;
			case NFAState::END:
				return true;
			case NFAState::CHAR:
				return false;
		}
	}
}
char NFA::eClosure(Stack<NFAState*>& newStates){
	bool isActive[numStates];
	for(unsigned int i = 0; i < numStates; i++){
		isActive[i] = 0;
	}
	bool endStateAdded = false;
	while(newStates.size() != 0){
		NFAState* state = newStates.pop();
		endStateAdded = addClosure(state, isActive) || endStateAdded;//add the closure of each state on newStates onto old States.
	}
	if(endStateAdded){
		return 1;//end state reached
	}
	else if(activeStates.size() == 0){
		return 0;//automaton died
	}
	else{
		return 2;//no rejection or acceptance
	}
}

char NFA::simulate(char c){
	//we take whats on oldStates, and transfer everything's character transitions to the new states. The thompson construction guarantees one transition per state.
	Stack<NFAState*> newStates;
	while(activeStates.size() != 0) {
		NFAState* t = activeStates.pop();
		if(t->contains(c)){
			newStates.push(t->out[0]);//if there is a transition, we push the next state onto old states.
		}
	}
	if(newStates.size() == 0){
		return 0;//automaton died
	}
	return eClosure(newStates);
}

void NFA::initiate(){
	Stack<NFAState*> newStates;
	newStates.push(start);
	eClosure(newStates);
}

NFA::NFA(){
	start = nullptr;
	stateArray = nullptr;
	numStates = 0;
}
