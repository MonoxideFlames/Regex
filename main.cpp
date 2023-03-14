#include <iostream>
#include "RegexToNFA.hpp"

int main(int argc, const char** argv){
	std::string val0;
	std::cout << "enter regex >> ";
	std::cin >> val0;
	const char* regex = val0.c_str();
	std::string val1;
	std::cout << "enter text >> ";
	std::cin >> val1;
	RegexToNFAConverter conv = RegexToNFAConverter(regex);
	NFA main; 
	conv.consume(main);
	main.populateStates();

	const char* string = val1.c_str();
	main.initiate();
	int isDead = 2;
	for(unsigned int i = 0; string[i] != 0 ; i++){
		isDead = main.simulate(string[i]);
	}
	std::cout << "State Machine Status: "<<isDead << '\n';
	main.print();
	main.clear();
	return 0;
}
