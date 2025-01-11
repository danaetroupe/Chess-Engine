#include "UserInput.h"

#include <iostream>
#include <string>

char* UserInput::GetMoves()
{
	bool valid = false;
	char moves[6];
	std::string input;
	while (!valid) 
	{
		std::cout << "Please enter your first moves: ";
		std::cin >> input;
		std::cout << std::endl;
		valid = input.size() == 2;
		valid = valid && (input[0] <= 'h' && input[0] >= 'a') || (input[0] <= 'H' && input[0] >= 'A');
		valid = valid && (input[1] <= '8' && input[1] >= '1');
	}
	moves[0] = input[0];
	moves[1] = input[1];
	moves[2] = '\0';
	valid = false;
	while (!valid)
	{
		std::cout << "Please enter your second move: " << std::endl;
		std::cin >> input;
		std::cout << std::endl;
		valid = input.size() == 2;
		valid = valid && (input[0] <= 'h' && input[0] >= 'a') || (input[0] <= 'H' && input[0] >= 'A');
		valid = valid && (input[1] <= '8' && input[1] >= '1');
	}
	moves[3] = input[0];
	moves[4] = input[1];
	moves[5] = '\0';
	return moves;
}